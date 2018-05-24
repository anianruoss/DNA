#include <chrono>
#include <iostream>
#include <random>
#include <thread>
#include <vector>


using Iterator = std::vector<int>::const_iterator;

void sum_ser (Iterator from, Iterator to, int& result) {
    int local = 0;
    for (; from != to; ++from)
        local += *from;
    result = local;
}

void sum_par(Iterator beg, Iterator end, int& result) {
    const int& nThreads = std::thread::hardware_concurrency();
    std::vector<std::thread> threads;
    std::vector<int> sums(nThreads, 0);
    const int partSize = (end-beg)/nThreads;

    for (int i = 0; i < nThreads-1; ++i) {
        threads.emplace_back(std::thread(sum_ser,
                                         beg, beg + partSize,
                                         std::ref(sums[i])));
        beg += partSize;
    }

    threads.emplace_back(std::thread(sum_ser,
                                     beg, end,
                                     std::ref(sums[nThreads-1])));

    for (auto& t : threads)
        t.join();

    sum_ser(sums.begin(), sums.end(), result);
}


int main() {
    int n;
    std::cin >> n;

    std::default_random_engine e1(42);
    std::uniform_int_distribution<int> uniform_dist(1, 6);

    std::vector<int> v(n);
    for (int i = 0; i < n; ++i)
        v[i] = uniform_dist(e1);

    int result;

    {
        auto t1 = std::chrono::high_resolution_clock::now();
        sum_ser(v.begin(), v.end(), result);
        auto t2 = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> ms = t2 - t1;
        std::cout << std::fixed << "serial:   " << result
                  << " (took " << ms.count() << " ms)" << std::endl;
    }
    {
        auto t1 = std::chrono::high_resolution_clock::now();
        sum_par(v.begin(), v.end(), result);
        auto t2 = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> ms = t2 - t1;
        std::cout << std::fixed << "parallel: " << result
                  << " (took " << ms.count() << " ms)" << std::endl;
    }

    return 0;
}

