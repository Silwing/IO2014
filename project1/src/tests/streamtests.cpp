#include "SingleItemStreams.hpp"
#include "FStreams.hpp"
#include "BufferedStreams.hpp"
#include "MMappedStreams.hpp"
#include <iostream>
#include <cstdio>
#include <string>
#include <sstream>
#include <thread>
#include <fstream>
#include <cmath>
#include "Timer.hpp"

namespace experiments {
	Reporter* rep = new StdReporter();
	
	template<typename IN, typename OUT>
	void test(IN* in, OUT* out, int k, int n, string testName) {
		
		int i;
		/* Test writing */
		{
			Timer t(rep, "Writing with " + testName + "\t" + to_string(n) + "\t" + to_string(k));
			
			//create streams
			for (i = 0; i < k; i++)
				out[i]->create();
			
			//write n elements
			for (i = 0; i < n; i++)
				out[i % k]->write(i);
			
			//close streams
			for (i = 0; i < k; i++)
				out[i]->close();
		}
		
		/* Test reading */
		{
			Timer t(rep, "Reading with " + testName + "\t" + to_string(n) + "\t" + to_string(k));
			for (i = 0; i < k; i++) {
				in[i]->open();
			}
			
			for (i = 0; i < n; i++) {
				if (in[i % k]->endOfStream()) {
					fprintf(stderr, "%s, stream %i hit endofstream to soon\n", testName.c_str(), i % k);
				} else {
					in[i % k]->readNext();
				}
			}
			
			for (i = 0; i < k; i++) {
				if (!in[i]->endOfStream()) {
					fprintf(stderr, "%s, stream %i did not hit endofstream\n", testName.c_str(), i);
				}
			}
		}
		
		for (int i=0; i<k; i++) {
			delete out[i];
			delete in[i];
		}
	}

	void start(int k, int n, StreamFactory* fac) {
		AbstractInputStream<int>* in[k];
		AbstractOutputStream<int>* out[k];
		for (int i=0; i<k; i++) {
			string name = "tmp/data" + to_string(i);
			in[i] = fac->getInputStream(name);
			out[i] = fac->getOutputStream(name);
		}
		test(in, out, k, n, fac->getInfo());
	}

	
};
