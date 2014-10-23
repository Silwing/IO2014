#pragma once

#include "Pair.hpp"
#include <cstdio>

template<typename E, int m>
class Node {
	private:
		unsigned int id;
		unsigned int lastRecords[m];
		
	public:
		Node() {
			for (int i = 0; i < m; i++) {
				lastRecords[i] = 0;
			}
		}
		
		Node(unsigned int id) : id(id) {
			for (int i = 0; i < m; i++) {
				lastRecords[i] = 0;
			}
		}
		
		~Node() {
		}
		
		unsigned int getId() {
			return id;
		}
		
		inline static unsigned int getParent(unsigned int id) {
			return (id - 1) / m;
		}
		
		unsigned int getParent() {
			return getParent(id);
		}
		
		unsigned int getChildren(unsigned int child) {
			return id*m + 1 + child;
		}
		
		unsigned int getSiblingNumber() {
			return (id-1)*((id - 1) / m * m);
		}
		
		unsigned int getSizeOf(unsigned int siblingNumber) {
			return lastRecords[siblingNumber];
		}
		
		void setSizeOf(unsigned int siblingNumber, unsigned int size) {
			lastRecords[siblingNumber] = size;
		}
};