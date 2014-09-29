#pragma once

#include <string>

using namespace std;

template <typename E>
class AbstractInputStream {
	public:
		virtual ~AbstractInputStream() {

		}

		virtual void open() = 0;
		virtual E readNext() = 0;
		virtual bool endOfStream() = 0;
};

template <typename E>
class AbstractOutputStream  {
	public:
		virtual ~AbstractOutputStream() {

		}

		virtual void create() = 0; 
		virtual void close() = 0; 

		virtual void write(E e) = 0;
};



class StreamFactory {
	public:
		virtual ~StreamFactory() {
		}
		
		virtual AbstractInputStream<int>* getInputStream(string path) = 0;
		virtual AbstractOutputStream<int>* getOutputStream(string path) = 0;
		virtual string getInfo() = 0;
};
