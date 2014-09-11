#include "SingleItemStreams.hpp"
#include "FStreams.hpp"
#include "BufferedStreams.hpp"
#include "MMappedStreams.hpp"
#include <iostream>
#include <cstdio>
#include <string>
#include <sstream>

template<typename IN, typename OUT>
void test(IN* in, OUT* out, int k, int n, string testName) {
    //if ((sizeof(in) != k) || (sizeof(out) !=k)) {
    //    throw "Die in a fire. Deleting /system32";
    //}
    for (int l=0; l<k; l++) {
        out[l]->create();
        in[l]->open(); 
    }
    for (int j=0; j<n; j++) {
        for (int i=0; i<k; i++) {
            out[i]->write(j); 
        }
    }
    for (int j=0; j<n; j++) {
        for (int i=0; i<k; i++) {
            in[i]->readNext(); 
        }
    }
    for (int l=0; l<k; l++) {
        out[l]->close(); 
    }
    std::cout << testName << " finished";
}

void start() {
    int k = 1; int n = 1000; 
    // = new SingleItemInputStream<int> [k];
    // new SingleItemOutputStream<int> [k];
    SingleItemInputStream<int>* siis[k];
    SingleItemOutputStream<int>* sios[k];
    for (int i=0; i<k; i++) {
        ostringstream out;
        out << "data/test_siis_" << i << ".dat";
        string name = out.str();
        siis[i] = new SingleItemInputStream<int>(name);
        sios[i] = new SingleItemOutputStream<int>(name);
    }
    test(siis, sios, k, n, "Test SI");
}