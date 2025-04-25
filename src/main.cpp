// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <vector>
#include <chrono>
#include <iostream>
#include <random>

#include "my_vector.h"
#include "my_array.h"

template<typename Vector>
void run_benchmarks(const char* name) {
    using namespace std::chrono;
    const size_t test_size = 100'000;
    std::mt19937 gen(42);
    std::uniform_int_distribution<int> dist(1, 1000);

    // Benchmark 1: Sequential push_back
    {
        Vector v;
        auto start = high_resolution_clock::now();
        for (size_t i = 0; i < test_size; ++i) {
            v.push_back(dist(gen));
        }
        auto end = high_resolution_clock::now();
        std::cout << name << " push_back: "
                  << duration_cast<microseconds>(end-start).count() << " μs\n";
    }

    // Benchmark 2: Random insertions
    {
        Vector v;
        auto start = high_resolution_clock::now();
        for (size_t i = 0; i < test_size; ++i) {
            v.insert(v.begin() + (i % (v.size()+1)), dist(gen));
        }
        auto end = high_resolution_clock::now();
        std::cout << name << " random insert: "
                  << duration_cast<microseconds>(end-start).count() << " μs\n";
    }

    // Benchmark 3: Iteration
    {
        Vector v(test_size, 42);
        volatile int sum = 0; // Prevent optimization
        auto start = high_resolution_clock::now();
        for (const auto& x : v) {
            sum += x;
        }
        auto end = high_resolution_clock::now();
        std::cout << name << " iteration: "
                  << duration_cast<microseconds>(end-start).count() << " μs\n";
    }

    // Benchmark 4: Copy construction
    {
        Vector v(test_size, 42);
        auto start = high_resolution_clock::now();
        Vector v2 = v;
        auto end = high_resolution_clock::now();
        std::cout << name << " copy construct: "
                  << duration_cast<microseconds>(end-start).count() << " μs\n";
    }

    // Benchmark 5: Sorting
    {
        Vector v;
        for (size_t i = 0; i < test_size; ++i) {
            v.push_back(dist(gen));
        }
        auto start = high_resolution_clock::now();
        std::sort(v.begin(), v.end());
        auto end = high_resolution_clock::now();
        std::cout << name << " sorting: "
                  << duration_cast<microseconds>(end-start).count() << " μs\n";
    }
}

int main() {
    run_benchmarks<std::vector<int>>("std::vector");
    run_benchmarks<MyVector<int>>("MyVector");
    return 0;
}