#include "SingleItemStreams.hpp"
#include <unistd.h>
#include <cstdio>
#include <fcntl.h>


#if defined(_WIN32) || defined(WIN32)
#define OS_WIN
#endif

template<typename E>
SingleItemInputStream<E>::SingleItemInputStream(string file) {
	this->file = file;
	fd = 0;
}

template<typename E>
SingleItemInputStream<E>::SingleItemInputStream() {
	//default constructors for arrays
}

template<typename E>
SingleItemInputStream<E>::~SingleItemInputStream() {
	if (fd != 0)
		::close(fd);
}

template<typename E>
void SingleItemInputStream<E>::open() {
	fd = ::open(file.c_str(), O_RDONLY);
	//TODO: what if file does not exists?
	eof = ::read(fd, &next, sizeof(E)) == 0;
}

template<typename E>
E SingleItemInputStream<E>::readNext() {
	E res = next;
	eof = ::read(fd, &next, sizeof(E)) == 0;
	return res;
}

template<typename E>
bool SingleItemInputStream<E>::endOfStream() {
	return eof;
}


template<typename E>
SingleItemOutputStream<E>::SingleItemOutputStream(string file) {
	this->file = file;
}

template<typename E>
SingleItemOutputStream<E>::SingleItemOutputStream() {
	//default contructor for array
}

template<typename E>
SingleItemOutputStream<E>::~SingleItemOutputStream() {
}

template<typename E>
void SingleItemOutputStream<E>::create() {

	#ifdef OS_WIN
	fd = ::open(file.c_str(), O_WRONLY | O_CREAT | O_NONBLOCK);
	#else
	fd = ::open(file.c_str(), O_WRONLY | O_CREAT | O_TRUNC | O_NONBLOCK, S_IRUSR | S_IWUSR);
	#endif

};

template<typename E>
void SingleItemOutputStream<E>::write(E e) {
	::write(fd, &e, sizeof(e));
}

template<typename E>
void SingleItemOutputStream<E>::close() {
	::close(fd);
}

template class SingleItemInputStream<int>;
template class SingleItemOutputStream<int>;


AbstractInputStream<int>* SingleItemStreamFactory::getInputStream(string path) {
	return new SingleItemInputStream<int>(path);
}

AbstractOutputStream<int>* SingleItemStreamFactory::getOutputStream(string path) {
	return new SingleItemOutputStream<int>(path);
}

string SingleItemStreamFactory::getInfo() {
	return "SingleItemStream";
}
