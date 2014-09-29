#pragma once

#include <string>
#include "AbstractStreams.hpp"

using namespace std;

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
