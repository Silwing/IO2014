#include "tests/sortingtest.hpp"
#include "AbstractStreams.hpp"
#include "Timer.hpp"
#include "ExternalMergeSort.hpp"

namespace experiments {
	void sort(StreamFactory* fac, string input, string output, int m, int d, int n) {
		StdReporter rep;
		Timer t(&rep, "External sort with " + fac->getInfo() + "\t" + to_string(n) + "\t" + to_string(m) + "\t" + to_string(d));
		ExternalMergeSort sorter(input, fac, n, m, d); //TODO -1 means to the end and should not be part of the constructor
		sorter.sort(output);
	}
}
