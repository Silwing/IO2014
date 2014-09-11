#include "MMappedStreams.hpp"
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <cstdio>
#include <sys/stat.h>

template<typename E>
MMappedInputStream<E>::MMappedInputStream(string file, size_t size) {
	if (size * sizeof(E) % sysconf(_SC_PAGE_SIZE) != 0) {
		throw "Baaad size. Have to be modulo pagesize (4096 on most systems)";
	}
	
	this->file = file;
	this->size = size;
	fd = 0;
}

template<typename E>
MMappedInputStream<E>::MMappedInputStream() {
	//Default constructor for arrays
}

template<typename E>
MMappedInputStream<E>::~MMappedInputStream() {
	if (buffer != NULL)
		munmap((void*) buffer, size * sizeof(E));
	if (fd != 0)
		::close(fd);
}

template<typename E>
void MMappedInputStream<E>::open() {
	fdOffset = 0;
	bufferOffset = 0;
	buffer = NULL;
	fd = ::open(file.c_str(), O_RDONLY);
	struct stat st;
	fstat(fd, &st);
	remaining = st.st_size / sizeof(E);
	readNextChunk();
}

template<typename E>
void MMappedInputStream<E>::readNextChunk() {
	if (buffer != NULL) {
		munmap((void*) buffer, size * sizeof(E));
	}
	buffer = (E*) mmap(NULL, size * sizeof(E), PROT_READ, MAP_PRIVATE, fd, fdOffset);
	fdOffset += size * sizeof(E);
	bufferOffset = 0;
}

template<typename E>
E MMappedInputStream<E>::readNext() {
	if (bufferOffset == size) {
		readNextChunk();
	}
	remaining--;
	return buffer[bufferOffset++];
}

template<typename E>
bool MMappedInputStream<E>::endOfStream() {
	return remaining == 0;
}


template<typename E>
MMappedOutputStream<E>::MMappedOutputStream(string file, int size) {
	if (size * sizeof(E) % sysconf(_SC_PAGE_SIZE) != 0) {
		throw "Baaad size. Have to be modulo pagesize (4096 on most systems)";
	}
	
	this->file = file;
	this->size = size;
}

template<typename E>
MMappedOutputStream<E>::~MMappedOutputStream() {
}

template<typename E>
void MMappedOutputStream<E>::create() {
	fdOffset = 0;
	bufferOffset = 0;
	buffer = NULL;
	fd = ::open(file.c_str(), O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
	mapNextChunk();
}

template<typename E>
void MMappedOutputStream<E>::write(E e) {
	buffer[bufferOffset++] = e;
	if (bufferOffset == size) mapNextChunk();
}

template<typename E>
void MMappedOutputStream<E>::close() {
	munmap((void*) buffer, size * sizeof(E));
	::ftruncate(fd, fdOffset - (size - bufferOffset) * sizeof(E)); 
	::close(fd);
}

template<typename E>
void MMappedOutputStream<E>::mapNextChunk() {
	if (buffer != NULL) {
		munmap((void*) buffer, size * sizeof(E));
	}
	
	//TODO: wtf? Do we really have to expand the file and use an IO for that?
	lseek(fd, fdOffset + size * sizeof(E) - 1, SEEK_SET);
	::write(fd, "", 1);
	
	
	buffer = (E*) mmap(NULL, size * sizeof(E), PROT_READ | PROT_WRITE, MAP_SHARED, fd, fdOffset);
	fdOffset += size * sizeof(E);
	bufferOffset = 0;
}


template class MMappedInputStream<int>;
template class MMappedOutputStream<int>;
