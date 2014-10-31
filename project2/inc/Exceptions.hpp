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

class EmptyTreeException : public Exception {
	public:
		EmptyTreeException(const char* msg);
};

class NotImplementedYetException : public Exception {
    public:
        NotImplementedYetException(const char* msg);
};

class IllegalStateException : public Exception {
    public:
        IllegalStateException(const char* msg);
};
