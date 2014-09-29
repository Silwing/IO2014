#pragma once
#include <vector>
#include "AbstractStreams.hpp"

using namespace std;

template <typename E>
class StreamHeader {
	public:
		bool done;
		StreamHeader(AbstractInputStream<E>* in);
		~StreamHeader();
		
		E next();
		E top;
		bool operator < (StreamHeader<E> sh);
	
	private:
		AbstractInputStream<E>* in;
};

template <typename E>
class DWayMergeSorter {
	private:
		vector<StreamHeader<E> > ins;
		AbstractOutputStream<E> *out;
		
	
	public:
		DWayMergeSorter(vector<AbstractInputStream<E>* > ins, AbstractOutputStream<E>* out);
		~DWayMergeSorter();
		
		void merge();
};
