#include "BufferedStreams.hpp"
#include <fcntl.h>
#include <unistd.h>
#include <cstdio>

template <typename E>
BufferedInputStream<E>::BufferedInputStream(string file, int size) {
    this->buffer = new E[size];
    this->file = file;
    this->size = size;
    fd = 0;
};

template <typename E>
BufferedInputStream<E>::BufferedInputStream() {
    //default constructor. Used for creating arrays
}

template <typename E>
BufferedInputStream<E>::~BufferedInputStream() {
    delete this->buffer;
    if (fd != 0)
		::close(fd);
};

template <typename E>
void BufferedInputStream<E>::open() {
	fd = ::open(file.c_str(), O_RDONLY);
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
    fd = ::open(file.c_str(), O_WRONLY | O_CREAT | O_TRUNC | O_NONBLOCK, S_IRUSR | S_IWUSR);
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

BufferedStreamFactory::BufferedStreamFactory(int size) : size(size) {
	
}

AbstractInputStream<int>* BufferedStreamFactory::getInputStream(string path) {
	return new BufferedInputStream<int>(path, size);
}

AbstractOutputStream<int>* BufferedStreamFactory::getOutputStream(string path) {
	return new BufferedOutputStream<int>(path, size);
}

string BufferedStreamFactory::getInfo() {
	return "BufferedStreams\t" + to_string(size);
}
