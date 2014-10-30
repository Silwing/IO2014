#pragma once

#include "AbstractStorage.hpp"
#include <cstdio>
#include <unistd.h>
#include <fcntl.h>

using namespace std;

template <typename E, int P, int m>
class SimpleStorage : public AbstractStorage<E, P, m> {
	private:
		int fd;
	
	public:
		SimpleStorage(const char* path) {
			fd = open(path, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
		}
		
		Node<E, m> readNode(const unsigned int index) {
			size_t size = sizeof(Node<E, m>);
			size_t offset = size + P * m * sizeof(E);
			Node<E, m> node;
			lseek(fd, index*offset, SEEK_SET);
			read(fd, (void*) &node, size);
			return node;
		}
		
		void writeNode(Node<E, m>& node) {
			size_t size = sizeof(Node<E, m>);
			size_t offset = size + P * m * sizeof(E);
			lseek(fd, node.getId()*offset, SEEK_SET);
			write(fd, (void*) &node, size);
		}
		
		void readPage(unsigned int node, unsigned int index, E* dest) {
			size_t size = sizeof(Node<E, m>);
			size_t offset = size + P * m * sizeof(E);
			lseek(fd, node*offset + size + index * P * sizeof(E), SEEK_SET);
			read(fd, (void*) dest, P * sizeof(E)); 
		}
		
		void readBlock(unsigned int node, E* dest) {
			size_t size = sizeof(Node<E, m>);
			size_t offset = size + P * m * sizeof(E);
			lseek(fd, node*offset + size, SEEK_SET);
			read(fd, (void*) dest, P * m * sizeof(E)); 
		}
		
		void writePage(unsigned int node, unsigned int index, E* src) {
			size_t size = sizeof(Node<E, m>);
			size_t offset = size + P * m * sizeof(E);
			lseek(fd, node*offset + size + index * P * sizeof(E), SEEK_SET);
			write(fd, (void*) src, P * sizeof(E)); 
		}
		
		void writeBlock(unsigned int node, E* src) {
			size_t size = sizeof(Node<E, m>);
			size_t offset = size + P * m * sizeof(E);
			lseek(fd, node*offset + size, SEEK_SET);
			write(fd, (void*) src, P * m * sizeof(E)); 
		}
};
