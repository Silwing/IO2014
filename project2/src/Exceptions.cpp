#include "Exceptions.hpp"

Exception::Exception(const char* msg, const char* type) : msg(msg), type(type){
}

const char* Exception::getType() {
	return type;
}

const char* Exception::getMsg() {
	return msg;
}


IllegalArgumentException::IllegalArgumentException(const char* msg) : Exception(msg, "IllegalArgumentException") {
	
}
