#pragma once

#include <cstdio>
#include "Exceptions.hpp"

template<typename E, int m>
class Node {
	private:
		unsigned int id;
		unsigned int childrenSizes[m];
		
	public:
		Node() {
			for (int i = 0; i < m; i++) {
				childrenSizes[i] = 0;
			}
		}
		
		Node(unsigned int id) : id(id) {
			for (int i = 0; i < m; i++) {
				childrenSizes[i] = 0;
			}
		}
		
		~Node() {
		}
		
		unsigned int getId() {
			return id;
		}
		
		unsigned int getParent() {
			return (id - 1) / m;
		}
		
		unsigned int getChild(unsigned int child) {
			return id*m + 1 + child;
		}
		
		unsigned int getSiblingNumber() {
			return (id - 1) % m;
		}
		
		unsigned int getSizeOf(unsigned int siblingNumber) {
			return childrenSizes[siblingNumber];
		}
		
		void setSizeOf(unsigned int siblingNumber, unsigned int size) {
			if (siblingNumber >= m) {
				throw new IllegalArgumentException("Bad argument for Node::setSizeOf");
			}
			childrenSizes[siblingNumber] = size;
		}
};
