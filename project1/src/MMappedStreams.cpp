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
	buffer = NULL;
	fdOffset = 0;
}

template<typename E>
MMappedInputStream<E>::~MMappedInputStream() {
}

template<typename E>
void MMappedInputStream<E>::open() {
	fd = ::open(file.c_str(), O_RDONLY);
	struct stat st;
	fstat(fd, &st);
	remaining = st.st_size / sizeof(E);
	printf("Remaining: %d\n", remaining);
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




template class MMappedInputStream<int>;
template class MMappedOutputStream<int>;
