#pragma once
#include "AbstractInputStream.hpp"
#include "AbstractOutputStream.hpp"
#include <string>
#include <sys/types.h>

using namespace std;

template <typename E>
class MMappedInputStream : public AbstractInputStream<E> {
	private:
		size_t size;
		string file;
		int fd;
		off_t fdOffset;
		int bufferOffset;
		E* buffer;
		int remaining;
		
		void readNextChunk();
	
	public:
		MMappedInputStream(string file, size_t size);
		~MMappedInputStream();
		
		void open();
		E readNext();
		bool endOfStream();
};

template <typename E>
class MMappedOutputStream : public AbstractOutputStream<E> {
	private:
	
	public:
		MMappedOutputStream(string file, int size);
		~MMappedOutputStream();
		
		void create();
		void write(E e);
		void close();
};
