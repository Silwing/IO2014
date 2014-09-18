#include "SingleItemStreams.hpp"
#include "FStreams.hpp"
#include "BufferedStreams.hpp"
#include "MMappedStreams.hpp"
#include <iostream>
#include <cstdio>
#include <string>
#include <sstream>
#include <chrono>
#include <thread>
#include <fstream>
#include <cmath>


template<typename IN, typename OUT>
void test(IN* in, OUT* out, int k, int n, string testName) {
    //if ((sizeof(in) != k) || (sizeof(out) !=k)) {
    //    throw "Die in a fire. Deleting /system32";
    //}
    n = (int) ceil(n / k);
    ofstream results;
    results.open("results.dat", ios::app);

    for (int l=0; l<k; l++) {
        out[l]->create();
    }
    auto t1 = std::chrono::high_resolution_clock::now(); 
    for (int j=0; j<n; j++) {
        for (int i=0; i<k; i++) {
            out[i]->write(j); 
        }
    }
    auto t2 = std::chrono::high_resolution_clock::now(); 
    results << testName << "\t" << n << "\t" << k << "\t" << std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count();
    for (int l=0; l<k; l++) {
        out[l]->close();
        in[l]->open();
    }
    t1 = std::chrono::high_resolution_clock::now(); 
    for (int j=0; j<n; j++) {
        for (int i=0; i<k; i++) {
            int a = in[i]->readNext();
            if(a != j) {
                std::cout << "Failed reading element " << j << ". Read: " << a << endl;
                return;
            } 
        }
    }
    t2 = std::chrono::high_resolution_clock::now(); 
    results << "\t" << std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count() << endl;
    for (int i=0; i<k; i++) {
        if(!in[i]->endOfStream()) {
            std::cout << "Failed endOfStream at stream " << i << endl;
        }
    }
    results.close(); 
    //std::cout << testName << " finished" << endl;
}

void start(int k, int n, int b) {
    // std::cout << "type,n,k,out,in" << endl;
    SingleItemInputStream<int>* siis[k];
    SingleItemOutputStream<int>* sios[k];
    for (int i=0; i<k; i++) {
        ostringstream out;
        out << "data/test_siis_" << i << ".dat";
        string name = out.str();
        siis[i] = new SingleItemInputStream<int>(name);
        sios[i] = new SingleItemOutputStream<int>(name);
    }
    test(siis, sios, k, n, "SI");


    FInputStream<int>* fis[k];
    FOutputStream<int>* fos[k];
    for (int i=0; i<k; i++) {
        ostringstream out;
        out << "data/test_fs_" << i << ".dat";
        string name = out.str();
        fis[i] = new FInputStream<int>(name);
        fos[i] = new FOutputStream<int>(name);
    }
    test(fis, fos, k, n, "FS");

    BufferedInputStream<int>* bis[k];
    BufferedOutputStream<int>* bos[k];
    for (int i=0; i<k; i++) {
        ostringstream out;
        out << "data/test_bs_" << i << ".dat";
        string name = out.str();
        bis[i] = new BufferedInputStream<int>(name, b);
        bos[i] = new BufferedOutputStream<int>(name, b);
    }
    test(bis, bos, k, n, "BF");

    MMappedInputStream<int>* mmis[k];
    MMappedOutputStream<int>* mmos[k];
    for (int i=0; i<k; i++) {
        ostringstream out;
        out << "data/test_mms_" << i << ".dat";
        string name = out.str();
        mmis[i] = new MMappedInputStream<int>(name, b);
        mmos[i] = new MMappedOutputStream<int>(name, b);
    }
    test(mmis, mmos, k, n, "MM");

}