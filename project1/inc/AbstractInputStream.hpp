#pragma once

template <typename E>
class AbstractInputStream {
protected:
	int remaining;

public:

    virtual ~AbstractInputStream() {

	}

    virtual void open() = 0;
	virtual E readNext() = 0;
	virtual bool endOfStream() = 0;
};
