#include "QuickSort.hpp"

QuickSort::QuickSort() {
	
}

void QuickSort::sort(vector<int>& elements) {
	quicksort(elements, 0, elements.size() - 1);
}

void QuickSort::quicksort(vector<int>& data, int low, int high)
{
    if (low >= high) return;

    int p = partition(data, low, high);

    quicksort(data, low, p-1);
    quicksort(data, p+1, high);
}

void QuickSort::swap(vector<int>& data, int i, int j) {
    int tmp = data[i];
    data[i] = data[j];
    data[j] = tmp;
}

int QuickSort::partition(vector<int>& data, int low, int high)
{
    int p = low;
    for (int i = p+1; i <= high; ++i) {
        if (data[i] < data[p]) {
            swap(data, i, p);
            if (i != p+1) {
                swap(data, i, p+1);
            }
            p = p + 1;
        }
    }

    return p;
}

