#include "Timer.hpp"


void StdReporter::report(string trial, long ms) {
	printf("%s\t%li\n", trial.c_str(), ms);
}

Timer::Timer(Reporter* rep) : Timer(rep, "") {
}

Timer::Timer(Reporter* rep, string name) {
	this->name = name;
	this->rep = rep;
	start = high_resolution_clock::now();
}

Timer::~Timer() {
	milliseconds span = duration_cast<milliseconds>(high_resolution_clock::now() - start);
	rep->report(name, span.count());
}
