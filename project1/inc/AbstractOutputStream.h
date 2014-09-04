#pragma once

template <class E> class AbstractOutputStream  {
public:
    AbstractOutputStream();
    ~AbstractOutputStream();

	virtual void create() = 0; 
	virtual void close() = 0; 

	virtual void write(E e) = 0;

};