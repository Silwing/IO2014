#pragma once
#include "AbstractStreams.hpp"
#include <string>

using namespace std;

template <typename E>
class BufferedInputStream : public AbstractInputStream<E> {
private:
    string file;
    E* buffer;
    int index;
    int size;
    int fd;
    int elementsInBuffer;

    void fillBuffer();

public:
    BufferedInputStream(string file, int size);
    BufferedInputStream();
    ~BufferedInputStream();

    void open();
    E readNext();
    bool endOfStream();
};

template <typename E>
class BufferedOutputStream : public AbstractOutputStream<E> {
private:
    string file;
    E* buffer;
    int index;
    int size;
    int fd;

public:
    BufferedOutputStream(string file, int size);
    ~BufferedOutputStream();

    void create();
    void write(E e);
    void close();

};

class BufferedStreamFactory : public StreamFactory {
private:
	int size;
	
public:
	BufferedStreamFactory(int size);
	AbstractInputStream<int>* getInputStream(string path);
	AbstractOutputStream<int>* getOutputStream(string path);
	
	string getInfo();
	
};
