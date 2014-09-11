#pragma once

template <typename E>
class AbstractOutputStream  {
public:

    virtual ~AbstractOutputStream() {

	}

	virtual void create() = 0; 
	virtual void close() = 0; 

	virtual void write(E e) = 0;

};
