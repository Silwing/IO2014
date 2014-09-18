#include "HeapSort.hpp"

HeapSort::HeapSort() {
    
}

void HeapSort::shiftRight(vector<int>& data, int low, int high)
{
    int root = low;
    while ((root*2)+1 <= high)
    {
        int leftChild = (root * 2) + 1;
        int rightChild = leftChild + 1;
        int swapIdx = root;
        /*Check if root is less than left child*/
        if (data[swapIdx] < data[leftChild])
        {
            swapIdx = leftChild;
        }
            /*If right child exists check if it is less than current root*/
        if ((rightChild <= high) && (data[swapIdx] < data[rightChild]))
        {
            swapIdx = rightChild;
        }
            /*Make the biggest element of root, left and right child the root*/
        if (swapIdx != root)
        {
            int tmp = data[root];
            data[root] = data[swapIdx];
            data[swapIdx] = tmp;
            root = swapIdx;
            /*Keep shifting right and ensure that swapIdx satisfies
            heap property aka left and right child of it is smaller than
            itself*/
        }
        else
        {
            break;
        }
    }
    return;
}

void HeapSort::heapify(vector<int>& data, int low, int high)
{
    /*Start with middle element. Middle element is chosen in
    such a way that the last element of array is either its
    left child or right child*/
    int midIdx = (high - low -1)/2;
    while (midIdx >= 0)
    {
        shiftRight(data, midIdx, high);
        --midIdx;
    }
    return;
}

void HeapSort::sort(vector<int>& elements)
{
    /*This will put max element in the index 0*/
    heapify(elements, 0, elements.size() - 1);
    int high = elements.size() - 1;
    while (high > 0)
    {
        /*Swap max element with high index in the array*/
        int tmp = elements[high];
        elements[high] = elements[0];
        elements[0] = tmp;
        --high;
        /*Ensure heap property on remaining elements*/
        shiftRight(elements, 0, high);
    }
    return;
}