#include "DWayMergeSorter.hpp"
#include <algorithm>
#include <cstdio>

template <typename E>
StreamHeader<E>::StreamHeader(AbstractInputStream<E>* in) {
	top = in->readNext();
	done = false;
	this->in = in;
}

template <typename E>
StreamHeader<E>::~StreamHeader() {
	
}

template <typename E>
E StreamHeader<E>::next() {
	E res = top;
	done = in->endOfStream();
	if (!done)
		top = in->readNext();
	return res;
}

template <typename E>
bool StreamHeader<E>::operator < (StreamHeader h) {
	return top > h.top;
}

template <typename E>
DWayMergeSorter<E>::DWayMergeSorter(vector<AbstractInputStream<E>* > ins, AbstractOutputStream<E>* out) {
	for (typename vector<AbstractInputStream<E> *>::iterator it = ins.begin(); it != ins.end(); it++) {
		AbstractInputStream<E>* in = *it;
		StreamHeader<E> header(in);
		this->ins.push_back(header);
	}
	this->out = out;
}

template <typename E>
DWayMergeSorter<E>::~DWayMergeSorter() {
	
}

template <typename E>
void DWayMergeSorter<E>::merge() {
	while(ins.size() != 0) {
		make_heap(ins.begin(), ins.end());
		
		StreamHeader<E> header = ins.front();
		pop_heap(ins.begin(), ins.end());  //element is moved to back, therefore
		ins.pop_back();                    //POP IT
		
		out->write(header.next());
		if (!header.done) {
			ins.push_back(header);
			push_heap(ins.begin(), ins.end());
		}
	}
}

template class StreamHeader<int>;
template class DWayMergeSorter<int>;
