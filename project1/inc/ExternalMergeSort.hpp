#pragma once

#include <string>
#include "AbstractInputStream.hpp"
#include "AbstractOutputStream.hpp"

using namespace std;

class StreamFactory {
	public:
		virtual ~StreamFactory() {
		};
		
		virtual AbstractInputStream<int>* getInputStream(string path) = 0;
		virtual AbstractOutputStream<int>* getOutputStream(string path) = 0;
};

class ExternalMergeSort {
	private:
		string input;
		StreamFactory* fac;
		int n, m, d;
		
	
	public:
		ExternalMergeSort(string input, StreamFactory* fac, int n, int m, int d);
		~ExternalMergeSort();
		
		void sort(string output);
};
