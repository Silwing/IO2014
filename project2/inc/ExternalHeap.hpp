#pragma once

#include <climits>
#include <vector>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include "Node.hpp"
#include "Pair.hpp"

#define EMPTY_TREE UINT_MAX

using namespace std;

template <typename E, int P, int m>
class ExternalHeap {
	private:
		E mergeBuffer[(m + 2) * P];
		vector<Pair<unsigned int, unsigned int> > mergeHeap;
		vector<E> insertBuffer;

		unsigned int lastLeaf;
		unsigned int rootSize;
		E rootPageBuffer[P];

		AbstractStorage<E, P, m>* storage;

		void emptyInsertBuffer() {
			for (int i = 0; i < P * m; i ++) {
				pop_heap(insertBuffer.begin(), insertBuffer.end() - i);
			}
			if (lastLeaf == EMPTY_TREE) {
				//Root case
				lastLeaf = 0;
				Node<E, m> node(lastLeaf);
				storage->writeNode(node);
				storage->writeBlock(node, &insertBuffer[0]);

				rootSize = m * P;
				for (int i = 0; i < P; i++) {
					rootPageBuffer[i] = insertBuffer[m*P-P + i];
				}
				storage->writeNode(node);
			} else {
				Node<E, m> node(++lastLeaf);

				for (int i = 0; i < insertBuffer.size(); i++) {
					mergeBuffer[i + P] = insertBuffer[i];
				}

				siftUp(node, P*m);
			}
			insertBuffer.clear();
		}

		void siftUp(Node<E, m> node, unsigned int size) {
            if (node.getId() == 0) throw new IllegalStateException("Root cannot be sifted up");
			Node<E, m> parent = storage->readNode(node.getParent());
			Node<E, m> grandParent;
			unsigned int r, h, k, parentSize, index = 0;
			if (parent.getId() == 0) {
				//parent is root
				parentSize = rootSize;
			} else {
				unsigned int gp = parent.getParent();
				unsigned int sn = parent.getSiblingNumber();
				grandParent = storage->readNode(gp);
				parentSize = grandParent.getSizeOf(sn);
			}
            h = parentSize;

			storage->readPage(parent, 0, &mergeBuffer[0]);
			E pivot = mergeBuffer[0];
			r = size + h;
			for (int i = 0; i < size; i++) {
				if (mergeBuffer[i+P] >= pivot) {
					h = r - i;
					break;
				}
			}

            if (h == parentSize) {
                parent.setSizeOf(node.getSiblingNumber(), size);
                storage->writeNode(node);
                storage->writeNode(parent);
                storage->writeBlock(node, &mergeBuffer[P]);
                return;
            }

			k = h < P * m ? r - h : r - P * m;

			while(index < (k + P - 1)/P) {
				qsort(&mergeBuffer, P + size, sizeof(E), compare);
				storage->writePage(node, index++, &mergeBuffer[0]);

				if (index < (k + P - 1)/P)
					storage->readPage(parent, index, &mergeBuffer[0]);
			}

            for (int i = index * P; i < parentSize; i += P) {
                storage->readPage(parent, i/P, &mergeBuffer[P + size - k + i]);
            }

			if (parent.getId() == 0) {
				storage->writeBlock(parent, &mergeBuffer[P]);
				rootSize = r - k;
				unsigned int pageStartsAt = (rootSize - 1) / P * P;
				for (int i = 0; i < P; i++) {
					rootPageBuffer[i] = mergeBuffer[P + pageStartsAt + i];
				}
			} else {
				grandParent.setSizeOf(parent.getSiblingNumber(), r - k);
				storage->writeNode(grandParent);
			}
			parent.setSizeOf(node.getSiblingNumber(), k);
			storage->writeNode(node);
			storage->writeNode(parent);
			if (parent.getId() != 0) {
				siftUp(parent, r - k);
			}
		}

        inline void rebalance(unsigned int nodeId, unsigned int* nodeSize) {
            Node<E, m> node = storage->readNode(nodeId);
            rebalance(node, nodeSize);
        }

		void rebalance(Node<E, m> node, unsigned int* nodeSize) {
            unsigned int nodeId = node.getId();
            if (node.getId() == lastLeaf) {
                if (*nodeSize == 0) lastLeaf--;
                return;
            }

            if(node.getChild(0) >= lastLeaf) {
                Node<E, m> leaf(lastLeaf);
                Node<E, m> parent = storage->readNode(leaf.getParent());

                unsigned int x = parent.getSizeOf(leaf.getSiblingNumber());
                unsigned int s = x + *nodeSize;

                storage->readBlock(node, &mergeBuffer[P]);
                if (s > P*m) {
                    //Steal from last leaf
                    unsigned int pleaseSteal = P*m - *nodeSize;
                    if ((x - pleaseSteal) % P == 0) {
                        for (int i = (x - pleaseSteal) / P; i < (x + P - 1) / P; i++) {
                            storage->readPage(leaf, i , &mergeBuffer[P + *nodeSize + i * P]);
                        }
                    } else {
                        unsigned int lastPage = ((x + P - 1)/P - 1);
                        unsigned int firstPage = (x - pleaseSteal) / P;
                        for (int i = firstPage, j = 0; i <= lastPage; i++, j++) {
                            storage->readPage(leaf, i, &mergeBuffer[P + *nodeSize + j * P]);
                        }
                        //align
                        for (int i = 0; i < pleaseSteal; i++) {
                            mergeBuffer[P + *nodeSize + i] = mergeBuffer[P + *nodeSize + i + (x - pleaseSteal) % P];
                        }
                    }

                    *nodeSize = P*m;
                    qsort(&mergeBuffer[P], *nodeSize, sizeof(E), compare);
                    parent.setSizeOf(leaf.getSiblingNumber(), x - pleaseSteal);
                    storage->writeNode(parent);
                    if (nodeId == 0) {
                        storage->writeBlock(node, &mergeBuffer[P]);
                        //root block is full. Just use the last page as rootPageBuffer
                        for (int i = 0; i < P; i++) {
                            rootPageBuffer[i] = mergeBuffer[P*m + i];
                        }
                    } else {
                        siftUp(node, *nodeSize);
                    }
                } else {
                    //Steal everything from lastLeaf and make new last leaf.
                    for (int i = 0; i < (x - 1)/P + 1; i++) {
                        storage->readPage(leaf, i, &mergeBuffer[P + *nodeSize + i * P]);
                    }
                    qsort(&mergeBuffer[P], s, sizeof(E), compare);
                    lastLeaf--;
                    *nodeSize += x;
                    if (nodeId == 0) {
                        storage->writeBlock(node, &mergeBuffer[P]);
                        //root block is not full - but when adding Pm/2, last page is still the same
                    } else {
                        if (*nodeSize < P*m/2 && nodeId != lastLeaf)
                            rebalance(node, nodeSize);
                        siftUp(node, s);
                    }
                    
                }
                return;
            }

            //move elements
            storage->readBlock(nodeId, mergeBuffer);
            for (int i = 0; i < P * m / 2; i++) {
                mergeBuffer[i + P*m / 2] = mergeBuffer[i];
            }
            storage->writeBlock(nodeId, mergeBuffer);

            //read children
			for (int i = 0; i < m; i++) {
				int c = node.getChild(i);
				if (c > lastLeaf) {
					break;
				}
				unsigned int size = node.getSizeOf(i);
				unsigned int pageIndex = (size - 1) / P;
				mergeHeap.push_back(Pair<unsigned int, unsigned int> (i, size));
				storage->readPage(c, pageIndex, &mergeBuffer[i*P]);
			}

            //compare function for heap
			auto buf = mergeBuffer; //this is for use of closure
			auto cmp = [buf](Pair<unsigned int, unsigned int> p1, Pair<unsigned int, unsigned int>p2) {
                if (p1.snd == 0) return true; //these cases does not mean a lot
                if (p2.snd == 0) return false;
				E e1 = buf[p1.fst * P + (p1.snd - 1) % P];
				E e2 = buf[p2.fst * P + (p2.snd - 1) % P];
                return e1 < e2;
			};

            //initial heap
			make_heap(mergeHeap.begin(), mergeHeap.end(), cmp);

            //read the first P*m/2 elements from heap
            for (int i = 0; i < P * m / 2; i++) {
                Pair<unsigned int, unsigned int> top = mergeHeap[0];
                int n = mergeBuffer[top.fst * P + (top.snd - 1) % P];
                mergeBuffer[P*m + P - i % P - 1] = n;
                if ((i+1) % P == 0) {
                    //print last page
                    /*fprintf(stderr, "Merged page:");
                    for (int j = 0; j < P; j++) {
                        fprintf(stderr, " %d", mergeBuffer[P*m + j]);
                    }
                    fprintf(stderr, "\n");*/
                    unsigned int offset = m/2 - i/P - 1;
                    storage->writePage(nodeId, offset, &mergeBuffer[P*m]);
                }
                top.snd--;
                if (top.snd == 0 && node.getChild(top.fst) == lastLeaf) {
                    lastLeaf--;
                    pop_heap(mergeHeap.begin(), mergeHeap.end(), cmp);
                    mergeHeap.pop_back();
                } else {
                    if ((top.snd) % P == 0 && top.snd != 0) {
                        storage->readPage(node.getChild(top.fst), (top.snd - 1) / P, &mergeBuffer[top.fst*P]);
                    }
                    mergeHeap[0] = top;
                    pop_heap(mergeHeap.begin(), mergeHeap.end(), cmp);
                    push_heap(mergeHeap.begin(), mergeHeap.end(), cmp);
                }

            }

            *nodeSize += P*m / 2;

            for (int i = 0; i < mergeHeap.size(); i++) {
                Pair<unsigned int, unsigned int> pair = mergeHeap[i];
                //fprintf(stderr, "Size of node %d: %d\n", pair.fst, pair.snd);
                node.setSizeOf(pair.fst, pair.snd);
            }
            mergeHeap.clear();
            storage->writeNode(node);
            // if we just rebalanced the root we update rootPageBuffer
            // since we might have changed the root so the last page looks differently
            // not necessary since we expect P*m/2 to be an even amount of pages
            
            for (int i = 0; i < m; i++) {
                unsigned int child = node.getChild(i);
                if (child >= lastLeaf) break;
                unsigned int childSize = node.getSizeOf(i);
                if (childSize < P*m/2) {
                    rebalance(child, &childSize);
                    //node.setSizeOf(i, childSize);
                }
            }
            //storage->writeNode(node);
		}

	public:
		ExternalHeap(AbstractStorage<E, P, m>* storage) : storage(storage) {
			lastLeaf = EMPTY_TREE;
			rootSize = 0;
		}

		static int compare(const void* p1, const void* p2) {
			E e1 = *(E*) p1;
			E e2 = *(E*) p2;
			if (e1 < e2) return -1;
			if (e2 == e1) return 0;
			return 1;
		}

		unsigned int getRootSize() {
            return rootSize;
		}

		void insert(E e) {
            fprintf(stderr, "inserting %d\n", e);
			insertBuffer.push_back(e);
			push_heap(insertBuffer.begin(), insertBuffer.end());
			if (insertBuffer.size() == P * m) emptyInsertBuffer();
		}

		E deleteMax() {
            E res;
			if (insertBuffer.size() == 0 && rootSize == 0) {
				throw new EmptyTreeException("You cannot delete an element from an empty tree");
			} else if (rootSize == 0) {
				res = insertBuffer[0];
				pop_heap(insertBuffer.begin(), insertBuffer.end());
				insertBuffer.pop_back();
				return res; //DO NOT TRY TO REBALANCE EMPTY TREE!!
			} else if (insertBuffer.size() == 0) {
				rootSize--;
				res = rootPageBuffer[rootSize % P];
			} else {
				E e1 = insertBuffer[0];
				E e2 = rootPageBuffer[(rootSize - 1) % P];
				//fprintf(stderr, "RootPageBuffer:");
				//for (int i = 0; i < P; i++) {
				//	fprintf(stderr, " %d", rootPageBuffer[i]);
				//}
				//fprintf(stderr, "\n");
				//fprintf(stderr, "ib[0]: %d, pb[0]: %d\n", e1, e2);
				if (e1 > e2) {
					res = e1;
					pop_heap(insertBuffer.begin(), insertBuffer.end());
					insertBuffer.pop_back();
				} else {
					rootSize--;
					res = e2;
				}
			}


			if (rootSize % P == 0 && rootSize != 0) {
				storage->readPage(0, rootSize / P - 1, rootPageBuffer);
            }
            if (rootSize < P * m / 2) {
                rebalance(0, &rootSize);
            }

			return res;
		} 
};
