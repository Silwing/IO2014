#pragma once
#include <ctime>
#include <chrono>
#include <string>

using namespace std;
using namespace std::chrono;

class Reporter {
	public:
		virtual ~Reporter() { }
		virtual void report(string trial, long ms) = 0;
	
};

class StdReporter : public Reporter {
	public:
		void report(string trial, long ms);
};

class Timer {
	private:
		high_resolution_clock::time_point start;
		string name;
		Reporter* rep;
	
	public:
		Timer(Reporter* rep);
		Timer(Reporter* rep, string name);
		~Timer();
};
