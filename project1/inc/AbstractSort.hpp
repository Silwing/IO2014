#pragma once
#include <vector>

using namespace std;

class AbstractSort {
	public:
		virtual ~AbstractSort() {
			
		}
		
		virtual void sort(vector<int>& elements) = 0;
};
