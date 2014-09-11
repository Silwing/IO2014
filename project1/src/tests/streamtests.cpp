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
    }
    for (int j=0; j<n; j++) {
        for (int i=0; i<k; i++) {
            out[i]->write(j); 
        }
    }
    for (int l=0; l<k; l++) {
        out[l]->close();
        in[l]->open();
    }
    for (int j=0; j<n; j++) {
        for (int i=0; i<k; i++) {
            int a = in[i]->readNext();
            if(a != j) {
                std::cout << "Failed reading element " << j << ". Read: " << a << endl;
                return;
            } 
        }
    }
    for (int i=0; i<k; i++) {
        if(!in[i]->endOfStream()) {
            std::cout << "Failed endOfStream at stream " << i << endl;
        }
    }
    std::cout << testName << " finished" << endl;
}

void start() {
    int k = 8; int n = 100000; int b = 1024;
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


    FInputStream<int>* fis[k];
    FOutputStream<int>* fos[k];
    for (int i=0; i<k; i++) {
        ostringstream out;
        out << "data/test_fs_" << i << ".dat";
        string name = out.str();
        fis[i] = new FInputStream<int>(name);
        fos[i] = new FOutputStream<int>(name);
    }
    test(fis, fos, k, n, "Test F");

    BufferedInputStream<int>* bis[k];
    BufferedOutputStream<int>* bos[k];
    for (int i=0; i<k; i++) {
        ostringstream out;
        out << "data/test_bs_" << i << ".dat";
        string name = out.str();
        bis[i] = new BufferedInputStream<int>(name, b);
        bos[i] = new BufferedOutputStream<int>(name, b);
    }
    test(bis, bos, k, n, "Test Buf");

    MMappedInputStream<int>* mmis[k];
    MMappedOutputStream<int>* mmos[k];
    for (int i=0; i<k; i++) {
        ostringstream out;
        out << "data/test_mms_" << i << ".dat";
        string name = out.str();
        mmis[i] = new MMappedInputStream<int>(name, b);
        mmos[i] = new MMappedOutputStream<int>(name, b);
    }
    test(mmis, mmos, k, n, "Test MM");

}