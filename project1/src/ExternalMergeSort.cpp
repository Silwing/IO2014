#include "ExternalMergeSort.hpp"

ExternalMergeSort::ExternalMergeSort(string input, StreamFactory fac, int n, int m, int d) {
	this->input = input;
	this->fac = fac;
	this->n = n;
	this->m = m;
	this->d = d;
}

ExternalMergeSort::~ExternalMergeSort() {
	
}

void ExternalMergeSort::sort(string output) {
	AbstractInputStream<int> in = fac.getInputStream(input);
	Queue<string> paths;
	QuickSort sorter;
	
	//split
	in.open();
	int i = 0;
	vector<int> elements;
	while(!in.endOfStream() && i <= n) {
		elements.push_back(in.readNext());
		i++;
		if (elements.size() == m || i == n || in.endOfStream()) {
			sorter.sort(elements);
			string path = ""; //TODO
			paths.push(path);
			AbstractOutputStream<int> out = fac.getOutputStream(path);
			out.create();
			for (vector<int>::iterator it = elements.begin(); it != elements.end(); it++) {
				out.write(*it);
			}
			out.close();
			elements.clear();
		}
	}
	
	//merge
	
}
