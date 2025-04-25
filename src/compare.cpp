#include "../include/my_vector.h"
#include "../include/my_array.h"

#include <vector>
#include <array>
#include <chrono>
#include <iostream>
#include <fstream>
#include <numeric>
#include <string>

using us = std::chrono::microseconds;

template<typename F>
long long time_us(F fn) {
    auto t0 = std::chrono::high_resolution_clock::now();
    fn();
    auto t1 = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<us>(t1 - t0).count();
}

template <size_t N>
void bench_myarray(std::ofstream& csv, int runs) {
    std::vector<int> tmp(N);
    std::iota(tmp.begin(), tmp.end(), 0);

    for (int run = 1; run <= runs; ++run) {
        {
            auto t_my = time_us([&]() {
                MyArray<int, N> a;
            });
            csv << "MyArray" << N << ",ctor_default," << N << "," << run << "," << t_my << "\n";
        }

        {
            auto t_my = time_us([&]() {
                MyArray<int, N> a{ tmp.begin(), tmp.begin() + N };
            });
            csv << "MyArray" << N << ",ctor_init_list," << N << "," << run << "," << t_my << "\n";
        }

        {
            MyArray<int, N> a(tmp.begin(), tmp.begin() + N);
            auto t_my = time_us([&]() {
                long long s = 0;
                for (size_t i = 0; i < N; ++i) s += a[i];
            });
            csv << "MyArray" << N << ",operator[]," << N << "," << run << "," << t_my << "\n";
        }

        {
            MyArray<int, N> a(tmp.begin(), tmp.begin() + N);
            auto t_my = time_us([&]() {
                long long s = 0;
                for (size_t i = 0; i < N; ++i) s += a.at(i);
            });
            csv << "MyArray" << N << ",at," << N << "," << run << "," << t_my << "\n";
        }

        {
            MyArray<int, N> a(tmp.begin(), tmp.begin() + N);
            auto t_my = time_us([&]() {
                long long s = 0;
                for (auto const& x : a) s += x;
            });
            csv << "MyArray" << N << ",iterate," << N << "," << run << "," << t_my << "\n";
        }

        {
            MyArray<int, N> a(tmp.begin(), tmp.begin() + N);
            auto t_my = time_us([&]() {
                MyArray<int, N> b(a);
            });
            csv << "MyArray" << N << ",copy_ctor," << N << "," << run << "," << t_my << "\n";
        }

        {
            MyArray<int, N> a(tmp.begin(), tmp.begin() + N);
            auto t_my = time_us([&]() {
                MyArray<int, N> b(std::move(a));
            });
            csv << "MyArray" << N << ",move_ctor," << N << "," << run << "," << t_my << "\n";
        }

        {
            MyArray<int, N> a(tmp.begin(), tmp.begin() + N);
            MyArray<int, N> b(tmp.begin(), tmp.begin() + N);
            auto t_my = time_us([&]() {
                a.swap(b);
            });
            csv << "MyArray" << N << ",swap," << N << "," << run << "," << t_my << "\n";
        }

        {
            MyArray<int, N> a(tmp.begin(), tmp.begin() + N);
            MyArray<int, N> b(a);
            auto t_my = time_us([&]() {
                volatile bool eq = (a == b);
            });
            csv << "MyArray" << N << ",compare_eq," << N << "," << run << "," << t_my << "\n";
        }

        {
            MyArray<int, N> a(tmp.begin(), tmp.begin() + N);
            MyArray<int, N> b(a);
            auto t_my = time_us([&]() {
                volatile auto cmp = (a <=> b);
            });
            csv << "MyArray" << N << ",compare_three_way," << N << "," << run << "," << t_my << "\n";
        }
    }
}

int main() {
    std::ofstream csv("results.csv");
    csv << "container,operation,size,run,time_us\n";

    std::vector<size_t> vec_sizes = {10'000, 100'000};
    std::vector<size_t> arr_sizes = {1'000, 5'000};
    int runs = 5;

    for (auto N : vec_sizes) {
        for (int run = 1; run <= runs; ++run) {
            {
                long long t_std = time_us([&](){
                    std::vector<int> v;
                    v.reserve(N);
                    for (int i = 0; i < (int)N; ++i) v.push_back(i);
                });
                csv << "std::vector,push_back,"<<N<<","<<run<<","<<t_std<<"\n";

                long long t_my = time_us([&](){
                    MyVector<int> v;
                    v.reserve(N);
                    for (int i = 0; i < (int)N; ++i) v.push_back(i);
                });
                csv << "MyVector,push_back,"<<N<<","<<run<<","<<t_my<<"\n";
            }

            std::vector<int> base_std(N);
            std::iota(base_std.begin(), base_std.end(), 0);
            MyVector<int> base_my(N,0);
            for(size_t i=0;i<N;++i) base_my[i]=int(i);

            {
                long long t_std = time_us([&](){
                    long long s = 0;
                    for (size_t i = 0; i < N; ++i) s += base_std[i];
                });
                csv << "std::vector,operator[],"<<N<<","<<run<<","<<t_std<<"\n";

                long long t_my = time_us([&](){
                    long long s = 0;
                    for (size_t i = 0; i < N; ++i) s += base_my[i];
                });
                csv << "MyVector,operator[],"<<N<<","<<run<<","<<t_my<<"\n";
            }

            {
                long long t_std = time_us([&](){
                    long long s = 0;
                    for (size_t i = 0; i < N; ++i) s += base_std.at(i);
                });
                csv << "std::vector,at,"<<N<<","<<run<<","<<t_std<<"\n";

                long long t_my = time_us([&](){
                    long long s = 0;
                    for (size_t i = 0; i < N; ++i) s += base_my.at(i);
                });
                csv << "MyVector,at,"<<N<<","<<run<<","<<t_my<<"\n";
            }

            {
                long long t_std = time_us([&](){
                    long long s = accumulate(base_std.begin(), base_std.end(), 0LL);
                });
                csv << "std::vector,iterate,"<<N<<","<<run<<","<<t_std<<"\n";

                long long t_my = time_us([&](){
                    long long s = std::accumulate(base_my.begin(), base_my.end(), 0LL);
                });
                csv << "MyVector,iterate,"<<N<<","<<run<<","<<t_my<<"\n";
            }

            {
                long long t_std = time_us([&](){
                    auto v2 = base_std;
                });
                csv << "std::vector,copy_ctor,"<<N<<","<<run<<","<<t_std<<"\n";

                long long t_my = time_us([&](){
                    MyVector<int> v2(base_my);
                });
                csv << "MyVector,copy_ctor,"<<N<<","<<run<<","<<t_my<<"\n";
            }

            {
                std::vector<int> tmp_std = base_std;
                MyVector<int> tmp_my = base_my;

                long long t_std = time_us([&](){
                    auto v2 = std::move(tmp_std);
                });
                csv << "std::vector,move_ctor,"<<N<<","<<run<<","<<t_std<<"\n";

                long long t_my = time_us([&](){
                    MyVector<int> v2(std::move(tmp_my));
                });
                csv << "MyVector,move_ctor,"<<N<<","<<run<<","<<t_my<<"\n";
            }

            {
                long long t_std = time_us([&](){
                    volatile int f = base_std.front();
                    volatile int b = base_std.back();
                });
                csv << "std::vector,front_back,"<<N<<","<<run<<","<<t_std<<"\n";

                long long t_my = time_us([&](){
                    volatile int f = base_my.front();
                    volatile int b = base_my.back();
                });
                csv << "MyVector,front_back,"<<N<<","<<run<<","<<t_my<<"\n";
            }

            {
                long long t_std = time_us([&](){
                    auto v = base_std;
                    v.resize(N/2);
                    v.resize(N);
                });
                csv << "std::vector,resize,"<<N<<","<<run<<","<<t_std<<"\n";

                long long t_my = time_us([&](){
                    MyVector<int> v = base_my;
                    v.resize(N/2);
                    v.resize(N);
                });
                csv << "MyVector,resize,"<<N<<","<<run<<","<<t_my<<"\n";
            }

            {
                long long t_std = time_us([&](){
                    auto v = base_std;
                    v.clear();
                });
                csv << "std::vector,clear,"<<N<<","<<run<<","<<t_std<<"\n";

                long long t_my = time_us([&](){
                    MyVector<int> v = base_my;
                    v.clear();
                });
                csv << "MyVector,clear,"<<N<<","<<run<<","<<t_my<<"\n";
            }

            {
                long long t_std = time_us([&](){
                    auto v = base_std;
                    v.insert(v.begin(), -1);
                });
                csv << "std::vector,insert_one,"<<N<<","<<run<<","<<t_std<<"\n";

                long long t_my = time_us([&](){
                    MyVector<int> v = base_my;
                    v.insert(v.begin(), -1);
                });
                csv << "MyVector,insert_one,"<<N<<","<<run<<","<<t_my<<"\n";
            }

            {
                long long t_std = time_us([&](){
                    auto v = base_std;
                    v.erase(v.begin());
                });
                csv << "std::vector,erase_one,"<<N<<","<<run<<","<<t_std<<"\n";

                long long t_my = time_us([&](){
                    MyVector<int> v = base_my;
                    v.erase(v.begin());
                });
                csv << "MyVector,erase_one,"<<N<<","<<run<<","<<t_my<<"\n";
            }

            {
                auto v2 = base_std;
                long long t_std = time_us([&](){
                    volatile bool eq = (base_std == v2);
                });
                csv << "std::vector,compare_eq,"<<N<<","<<run<<","<<t_std<<"\n";

                MyVector<int> m2(base_my);
                long long t_my = time_us([&](){
                    volatile bool eq = (base_my == m2);
                });
                csv << "MyVector,compare_eq,"<<N<<","<<run<<","<<t_my<<"\n";
            }

            {
                auto v2 = base_std;
                long long t_std = time_us([&](){
                    auto a = base_std;
                    a.swap(v2);
                });
                csv << "std::vector,swap,"<<N<<","<<run<<","<<t_std<<"\n";

                MyVector<int> m2(base_my);
                long long t_my = time_us([&](){
                    MyVector<int> a = base_my;
                    a.swap(m2);
                });
                csv << "MyVector,swap,"<<N<<","<<run<<","<<t_my<<"\n";
            }
        }
    }


    bench_myarray<1000>(csv, runs);
    bench_myarray<5000>(csv, runs);

    csv.close();
    std::cout << "Done. Results in results.csv\n";
    return 0;
}
