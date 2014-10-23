#pragma once

#include <climits>
#include <vector>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include "Node.hpp"

#define EMPTY_TREE UINT_MAX

using namespace std;

template <typename E, int P, int m>
class ExternalHeap {
	private:
		E mergeBuffer[(2 * m + 1) * P];
		unsigned int mergeHeap[m];
		vector<E> insertBuffer;
		
		unsigned int lastLeaf;
		unsigned int lastRecordInRoot;
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
				
				lastRecordInRoot = m * P;
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
				h = lastRecordInRoot;
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
				lastRecordInRoot = r - k;
				unsigned int pageStartsAt = lastRecordInRoot / P * P;
				for (int i = 0; i < lastRecordInRoot % P; i++) {
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
		
	public:
		ExternalHeap(AbstractStorage<E, P, m>* storage) : storage(storage) {
			lastLeaf = EMPTY_TREE;
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
			
		}
};
