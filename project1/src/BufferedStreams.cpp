#include "BufferedStreams.hpp"
#include <fcntl.h>
#include <unistd.h>
#include <cstdio>

template <typename E>
BufferedInputStream<E>::BufferedInputStream(string file, int size) {
    this->buffer = new E[size];
    this->file = file;
    this->size = size;
};

template <typename E>
BufferedInputStream<E>::~BufferedInputStream() {
    delete this->buffer;
};

template <typename E>
BufferedInputStream<E>::BufferedInputStream() {
    
}

template <typename E>
void BufferedInputStream<E>::open() {
	fd = ::open(file.c_str(), O_RDONLY);
	//TODO: what if file does not exists?
    fillBuffer();
};

template <typename E>
void BufferedInputStream<E>::fillBuffer() {
    elementsInBuffer = ::read(fd, buffer, size * sizeof(E)) / sizeof(E);
    index = 0;
}

template <typename E>
E BufferedInputStream<E>::readNext() {
    E res = buffer[index++];
    if (index == size) fillBuffer();
    return res;
}

template <typename E>
bool BufferedInputStream<E>::endOfStream() {
    return elementsInBuffer < size && elementsInBuffer <= index;
}


template <typename E>
BufferedOutputStream<E>::BufferedOutputStream(string file, int size) {
    this->buffer = new E[size];
    this->file = file;
    this->size = size;
}

template <typename E>
BufferedOutputStream<E>::~BufferedOutputStream() {
    delete buffer;
}

template <typename E>
void BufferedOutputStream<E>::create() {
    fd = ::open(file.c_str(), O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
    index = 0;
}

template <typename E>
void BufferedOutputStream<E>::write(E e) {
    buffer[index++] = e;
    if (index == size) {
        ::write(fd, buffer, sizeof(E) * size);
        index = 0;
    }
}

template <typename E>
void BufferedOutputStream<E>::close() {
    if (index > 0) {
        ::write(fd, buffer, sizeof(E) * index);
    }
    ::close(fd);
}



template class BufferedInputStream<int>;
template class BufferedOutputStream<int>;
