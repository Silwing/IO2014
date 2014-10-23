#pragma once

class Exception {
	private:
		const char* msg;
		const char* type;
	
	public:
		Exception(const char* msg, const char* type);
		virtual ~Exception() { }
		virtual const char* getMsg();
		virtual const char* getType();
};

class IllegalArgumentException : public Exception {
	
	public:
		IllegalArgumentException(const char* msg);
};
