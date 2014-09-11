#include "FStreams.hpp"
#include <unistd.h>

#if defined(_WIN32) || defined(WIN32)
#define OS_WIN
#endif

template<typename E>
FInputStream<E>::FInputStream(string file) {
	this->file = file; 
}

template<typename E>
FInputStream<E>::~FInputStream() {

}

template<typename E>
void FInputStream<E>::open(){
	fd = fopen(file.c_str(), "r");
	fread(&next, sizeof(E), 1, fd);
}

template<typename E>
E FInputStream<E>::readNext() {
	E res = next; 
	fread(&next, sizeof(E), 1, fd);
	return res; 
}

template<typename E>
bool FInputStream<E>::endOfStream(){
	return feof(fd) != 0;
}

template<typename E>
FOutputStream<E>::FOutputStream(string file) {
	this->file = file; 
}

template<typename E>
FOutputStream<E>::~FOutputStream(){

}

template<typename E>
void FOutputStream<E>::create() {
	fd = fopen(file.c_str(), "w");
}

template<typename E>
void FOutputStream<E>::write(E e){
	fwrite(&e, sizeof(e), 1, fd);
}

template<typename E>
void FOutputStream<E>::close(){
	fclose(fd); 
}

template class FOutputStream<int>;
template class FInputStream<int>;
