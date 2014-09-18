#pragma once
#include "AbstractSort.hpp"

class QuickSort : public AbstractSort {
	private:
		int partition(vector<int>& a, int l, int r);
		void quicksort(vector<int>& a, int l, int r);
		void swap(vector<int>& data, int i, int j);
	
	public:
		QuickSort();
		void sort(vector<int>& elements);
};
