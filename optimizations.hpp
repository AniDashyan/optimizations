#ifndef OPTIMIZATIONS_H
#define OPTIMIZATIONS_H

#include <chrono>
#include <vector>
#include <iostream>

void compiler_optimizations(int size, int iterations) {
    std::vector<int> arr(size);
    for (int i = 0; i < size; ++i) arr[i] = i;

    long long final_sum = 0;
    auto total_time = 0.0;
    for (int iter = 0; iter < iterations; ++iter) {
        long long sum = 0;

        auto start = std::chrono::steady_clock::now();

        for (int i = 0; i < size; ++i) {
            sum += arr[i];
        }

        auto end = std::chrono::steady_clock::now();

        final_sum = sum;
        total_time = std::chrono::duration_cast<std::chrono::duration<double>>(end - start).count();
    }
    
    std::cout << "Array Size: " << size << "\n";
    std::cout << "Iterations: " << iterations << "\n";
    std::cout << "Final Sum: " << final_sum << "\n";
    std::cout << "Average Time: " << (total_time / iterations) << " seconds\n";
}
#endif