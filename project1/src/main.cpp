#include "SingleItemStreams.hpp"
#include "FStreams.hpp"
#include "BufferedStreams.hpp"
#include "MMappedStreams.hpp"
#include "DWayMergeSorter.hpp"
#include "streamtests.hpp"
#include <iostream>
#include <cstdio>
#include <vector>
#include "QuickSort.hpp"
#include <fstream>

#if defined(_WIN32) || defined(WIN32)
#define OS_WIN
#endif

using namespace std;

void test(AbstractInputStream<int>* in, AbstractOutputStream<int>* out, string testName) {
    int size = 1024 * 1024 * 256;
    out->create();
	for (int i = 0; i < size; i++) {
        out->write(i);
    }
    out->close();
    
	in->open();
    for (int i = 0; i < size; i++) {
        if (in->endOfStream()) {
            printf("%s failed with endOfStream at %d\n", testName.c_str(), i);
            return;
        }
		int j = in->readNext();
        if (i != j) {
            printf("%s failed at element %d. Got %d\n", testName.c_str(), i, j);
            return;
        }
    }
    if (!in->endOfStream()) {
        printf("%s failed at endOfStream\n", testName.c_str());
    } else {
    //    printf("%s succeed\n", testName.c_str());
    }
}

void testSorting() {
    vector<int> elements;
    for (int i = 20; i > 10; i--) {
		elements.push_back(rand() % 100);
	}
    
    printf("Before:\n");
    for (vector<int>::iterator it = elements.begin(); it != elements.end(); it++) {
		printf("%d ", *it);
	}
    printf("\n");
    
    QuickSort sorter;
    sorter.sort(elements);
    
    printf("After:\n");
    for (vector<int>::iterator it = elements.begin(); it != elements.end(); it++) {
		printf("%d ", *it);
	}
    printf("\n");
}
    
void experiment(int n, int k, int b){
    ofstream results; 
    results.open("results.dat", ios::trunc);
    results << "type\tn\tk\tout\tin" << endl;
    results.close(); 
    for (int i = 1; i<=k; i=i*2) {
        for (int j = 1024; j<=b; j=j*2) {
            start(i, n, j); 
        }
    }
}

int main(int argc, char** argv) {
    char flag = *argv[1]; 
    switch (flag) {
        default: 
            std::cout << flag << endl; 
        case 'd':
			testSorting();
            break; 
        case 't':
            if(argc == 5){
                int k = std::stoi(argv[2]);
                int n = std::stoi(argv[3]);
                int b = std::stoi(argv[4]);
                // start(k, n, b);
                experiment(n, k, b);
            } else {
                std::cout << "Wrong number of arguments, dummy";
                exit(EXIT_FAILURE);
            }
            break;
    }
}
