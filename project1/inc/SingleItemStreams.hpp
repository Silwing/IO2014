#pragma once

#include "AbstractStreams.hpp"
#include <string>

using namespace std;

template<typename E>
class SingleItemInputStream : public AbstractInputStream<E> {
private:
	string file;
	int fd;
	E next;
	bool eof;

public:
	SingleItemInputStream(string file);
	SingleItemInputStream(); 
	~SingleItemInputStream();

	void open();
	E readNext();
	bool endOfStream();	
};

template<typename E>
class SingleItemOutputStream : public AbstractOutputStream<E> {
private:
	string file;
	int fd;

public:
	SingleItemOutputStream(string file);
	SingleItemOutputStream(); 
	~SingleItemOutputStream();

	void create();
	void write(E e);
	void close();
};

class SingleItemStreamFactory : public StreamFactory {
	
public:
	AbstractInputStream<int>* getInputStream(string path);
	AbstractOutputStream<int>* getOutputStream(string path);
	
	string getInfo();
};
