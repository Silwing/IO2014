#pragma once
#include "AbstractSort.hpp"

class HeapSort : public AbstractSort {
    private:
        void shiftRight(vector<int>& a, int l, int h);
        void heapify(vector<int>& a, int l, int h);

    public: 
        HeapSort();
        void sort(vector<int>& elements);

};