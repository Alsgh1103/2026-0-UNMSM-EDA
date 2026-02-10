#include <iostream>
#include "util.h"
#include "pointers.h"
#include "containers/array.h"
#include "containers/lists.h"

// g++ -std=c++17 -Wall -g -pthread main.cpp DemoDoubleList.cpp -o main
void DemoDoubleList();

int main() {
    std::cout << "Hello EDA-UNMSM!" << std::endl;
    // DemoUtil();
    // DemoPointers1();
    // DemoArray();
    // DemoLists();
    // DemoThreads();
    DemoDoubleList();
    return 0;
}