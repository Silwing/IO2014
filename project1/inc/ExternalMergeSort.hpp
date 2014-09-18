#pragma once

#include <string>
#include "AbstractInputStream.hpp"
#include "AbstractOutputStream.hpp"

using namespace std;

class StreamFactory {
	public:
		virtual ~StreamFactory() {
		};
		
		AbstractInputStream<int> getInputStream(string path);
		AbstractOutputStream<int> getOutputStream(string path);
};

class ExternalMergeSort {
	private:
		string input;
		StreamFactory fac;
		int n, m, d;
		
	
	public:
		ExternalMergeSort(string input, StreamFactory fac, int n, int m, int d);
		~ExternalMergeSort();
		
		void sort(string output);
}
