#include "ExternalMergeSort.hpp"
#include "QuickSort.hpp"
#include "DWayMergeSorter.hpp"
#include <queue>

ExternalMergeSort::ExternalMergeSort(string input, StreamFactory* fac, int n, int m, int d) {
	this->input = input;
	this->fac = fac;
	this->n = n;
	this->m = m;
	this->d = d;
}

ExternalMergeSort::~ExternalMergeSort() {
	
}

void ExternalMergeSort::sort(string output) {
	int c = 0;
	AbstractInputStream<int>* in = fac->getInputStream(input);
	queue<string> paths;
	QuickSort sorter;
	
	//split
	in->open();
	int i = 0;
	vector<int> elements;
	while(!in->endOfStream() && (i < n || n == -1)) {
		elements.push_back(in->readNext());
		i++;
		if (elements.size() == m || i == n || in->endOfStream()) {
			sorter.sort(elements);
			string path = "tmp/" + to_string(c++); //TODO
			paths.push(path);
			AbstractOutputStream<int>* out = fac->getOutputStream(path);
			out->create();
			for (vector<int>::iterator it = elements.begin(); it != elements.end(); it++) {
				out->write(*it);
			}
			out->close();
			delete out;
			elements.clear();
		}
	}
	
	delete in;
	
	//merge
	while (paths.size() > 0) {
		vector<AbstractInputStream<int>* > ins;
		for (int i = 0; i < d && paths.size() > 0; i++) {
			string path = paths.front();
			AbstractInputStream<int>* in = fac->getInputStream(path);
			in->open();
			ins.push_back(in);
			paths.pop();
		}
		
		AbstractOutputStream<int>* out;
		if (paths.size() == 0) {
			out = fac->getOutputStream(output);
		} else {
			string path = "tmp/" + to_string(c++);
			paths.push(path);
			out = fac->getOutputStream(path);
		}
		out->create();
		DWayMergeSorter<int> dwms(ins, out);
		dwms.merge();
		out->close();
		
		delete out;
		for (vector<AbstractInputStream<int>* >::iterator it = ins.begin(); it != ins.end(); it++) {
			delete *it;
		}
	}
	
}
