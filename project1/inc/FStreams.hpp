#pragma once

#include "AbstractInputStream.hpp"
#include "AbstractOutputStream.hpp"
#include <string>
#include <cstdio>

using namespace std;

template<typename E>
class FInputStream : public AbstractInputStream<E> {
private: 
	string file; 
	FILE* fd;
	E next; 
public: 
	FInputStream(string file);
	~FInputStream();

	void open();
	E readNext();
	bool endOfStream();
};

template<typename E>
class FOutputStream : public AbstractOutputStream<E> {
private: 
	string file; 
	FILE* fd; 
public: 
	FOutputStream(string file);
	~FOutputStream();

	void create();
	void write(E e);
	void close(); 
};
