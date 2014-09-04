#include "SingleItemStreams.h"
#include <unistd.h>
#include <cstdio>
#include <fcntl.h>

template<typename E>
SingleItemInputStream<E>::SingleItemInputStream(string file) {
	this->file = file;
};

template<typename E>
SingleItemInputStream<E>::~SingleItemInputStream() {

}

template<typename E>
void SingleItemInputStream<E>::open() {
	fd = ::open(file.c_str(), O_RDONLY);
	//TODO: what if file does not exists?
	eof = ::read(fd, &next, sizeof(E)) == 0;
};

template<typename E>
E SingleItemInputStream<E>::readNext() {
	E res = next;
	eof = ::read(fd, &next, sizeof(E)) == 0;
	return res;
};

template<typename E>
bool SingleItemInputStream<E>::endOfStream() {
	return eof;
};


template<typename E>
SingleItemOutputStream<E>::SingleItemOutputStream(string file) {
	this->file = file;
};

template<typename E>
SingleItemOutputStream<E>::~SingleItemOutputStream() {
};

template<typename E>
void SingleItemOutputStream<E>::create() {
	fd = ::open(file.c_str(), O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
};

template<typename E>
void SingleItemOutputStream<E>::write(E e) {
	::write(fd, &e, sizeof(e));
};

template<typename E>
void SingleItemOutputStream<E>::close() {
	::close(fd);
};

template class SingleItemInputStream<int>;
template class SingleItemOutputStream<int>;
