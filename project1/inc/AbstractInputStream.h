#pragma once

template <class E> class AbstractInputStream {
public:
    AbstractInputStream();
    ~AbstractInputStream();

    virtual void open() = 0;
	virtual void close() = 0;

	virtual E readNext() = 0; //something other than int? Templates???
	virtual bool endOfStream() = 0;

};