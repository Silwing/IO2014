#include "Test.h"
#include <iostream>

Test::Test(int i) {
    std::cout << "Hello ";
}

Test::~Test() {
    std::cout << "world!\n";   
}