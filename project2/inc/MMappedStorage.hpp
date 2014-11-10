#pragma once

#include "AbstractStorage.hpp"
#include "Exceptions.hpp"
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <cstdio>

template<typename E, int P, int m>
class MMappedStorage : public AbstractStorage<E, P, m> {
private:
    int fd;
    
    
public:
    MMappedStorage(const char* path) {
        if (P * sizeof(E) % sysconf(_SC_PAGE_SIZE) != 0) {
            throw new IllegalStateException("The page size has to match the systems page size");
        }
        fd = open(path, O_RDWR | O_CREAT | O_TRUNC | O_NONBLOCK, S_IRUSR | S_IWUSR);
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
        mmap(dest, P * size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, offset);
    }
    
    void readBlock(unsigned int node, E* dest) {
        
    }
    
    void writePage(unsigned int node, unsigned int index, E* src) {
        size_t size = sizeof(Node<E, m>);
        munmap(src, P * size);
    }
    
    void writeBlock(unsigned int node, E* src) {
        
    }
};