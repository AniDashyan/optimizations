#include <iostream>
#include "optimizations.hpp"

int main() {
    compiler_optimizations(10'000'000, 5);
    compiler_optimizations(1'000'000, 10);
    compiler_optimizations(10000, 100);
    return 0;
}