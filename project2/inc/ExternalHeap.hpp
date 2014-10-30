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
		E mergeBuffer[(2 * m + 1) * P];
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
			Node<E, m> parent = storage->readNode(node.getParent());
			Node<E, m> grandParent;
			unsigned int r, h, k, index = 0;
			if (parent.getId() == 0) {
				//parent is root
				h = rootSize;
			} else {
				unsigned int gp = parent.getParent();
				unsigned int sn = parent.getSiblingNumber();
				grandParent = storage->readNode(gp);
				h = grandParent.getSizeOf(sn);
			}
			
			storage->readPage(parent, 0, &mergeBuffer[0]);
			E pivot = mergeBuffer[0];
			r = size + h;
			for (int i = 0; i < m * P; i++) {
				if (mergeBuffer[i+P] >= pivot) {
					h = r - i;
					break;
				}
			}
			
			k = h < P * m ? r - h : r - P * m;
				
			while(index < k/P) {
				qsort(&mergeBuffer, P + size, sizeof(E), compare);
				storage->writePage(node, index++, &mergeBuffer[0]);	
				
				if (index < k/P)
					storage->readPage(parent, index, &mergeBuffer[0]);
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
		
		void rebalance(int nodeId) {
			if (nodeId == lastLeaf) return;
			
			Node<E, m> node = storage->readNode(nodeId);
			for (int i = 0; i < m; i++) {
				int c = node.getChild(i);
				if (c > lastLeaf) {
					break;
				}
				unsigned int size = node.getSizeOf(i);
				unsigned int pageIndex = (size - 1) / P * P - 1;
				unsigned int pageSize = (size - 1) % P;
				mergeHeap.push_back(Pair<unsigned int, unsigned int> (i, pageSize));
				storage->readPage(c, pageIndex, &mergeBuffer[i*P]);
			}
			
			auto buf = mergeBuffer;
			printf("MergeBuffer:");
			for (int i = 0; i < P*m; i++) {
				printf(" %d", buf[i]);
			}
			printf("\n");
			
			auto cmp = [buf](Pair<unsigned int, unsigned int> p1, Pair<unsigned int, unsigned int>p2) {
				E e1 = buf[p1.fst * P + p1.snd];
				E e2 = buf[p2.fst * P + p2.snd];
				printf("Comparing %d and %d: %d\n", e1, e2, e1 < e2);
				if (e1 > e2) {
					printf("returning -1\n");
					return -1;
				} 
				if (e1 == e2) {
					printf("returning 0\n");
					return 0;
				}
				printf("returning 1\n");
				return 1;
			};
			
			printf("MergeHeap:");
			for (int i = 0; i < m; i++) {
				printf(" (%d,%d)", mergeHeap[i].fst, mergeHeap[i].snd); 
			}
			printf("\n");
			
			make_heap(mergeHeap.begin(), mergeHeap.end(), cmp);
			
			printf("MergeHeap:");
			for (int i = 0; i < m; i++) {
				printf(" (%d,%d)", mergeHeap[i].fst, mergeHeap[i].snd); 
			}
			printf("\n");
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
	
		void insert(E e) {
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
				printf("RootPageBuffer:");
				for (int i = 0; i < P; i++) {
					printf(" %d", rootPageBuffer[i]);
				}
				printf("\n");
				printf("ib[0]: %d, pb[0]: %d\n", e1, e2);
				if (e1 > e2) {
					res = e1;
					pop_heap(insertBuffer.begin(), insertBuffer.end());
					insertBuffer.pop_back();
				} else {
					rootSize--;
					res = e2;
				}
			}
			
			
			if (rootSize % P == 0) {	
				storage->readPage(0, rootSize / P - 1, rootPageBuffer);
				if (rootSize < P * m / 2) {
					rebalance(0);
				}
			}
			
			return res;
		}
};
