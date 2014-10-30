#pragma once

#include "Node.hpp"

template <typename E, int P, int m>
class AbstractStorage {
	public:
		virtual ~AbstractStorage() {
			
		}
	
		virtual Node<E, m> readNode(const unsigned int index) = 0;
		virtual void writeNode(Node<E, m>& node) = 0;
		
		virtual void readPage(unsigned int node, unsigned int index, E* dest) = 0;
		virtual void readBlock(unsigned int node, E* dest) = 0;
		
		virtual void writePage(unsigned int node, unsigned int index, E* src) = 0;
		virtual void writeBlock(unsigned int node, E* src) = 0;
		
		
		inline void readPage(Node<E, m> node, unsigned int index, E* dest) {
			readPage(node.getId(), index, dest);
		}
		
		inline void readBlock(Node<E, m> node, E* dest) {
			readBlock(node.getId(), dest);
		}
		
		inline void writePage(Node<E, m> node, unsigned int index, E* src) {
			writePage(node.getId(), index, src);
		}
		
		inline void writeBlock(Node<E, m> node, E* src) {
			writeBlock(node.getId(), src);
		}
};
