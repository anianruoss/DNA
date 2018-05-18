#include <cassert>
#include <iostream>
#include <mutex>
#include <random>
#include <thread>
#include <vector>


using Iterator = std::vector<int>::const_iterator;


std::mutex m;

void serialScalarProduct(Iterator beg1, Iterator end1,
                         Iterator beg2, Iterator end2,
                         int &result) {

    assert((end1 - beg1) == (end2 - beg2));

    int tmp = 0;

    while (beg1 != end1) {
        tmp += *beg1 * *beg2;
        ++beg1;
        ++beg2;
    }

    std::lock_guard<std::mutex> guard(m);
    result += tmp;
}

void parallelScalarProduct(Iterator beg1, Iterator end1,
                           Iterator beg2, Iterator end2,
                           int &result) {

    assert((end1 - beg1) == (end2 - beg2));

    const int &nThreads = std::thread::hardware_concurrency();
    std::vector<std::thread> threads;
    const int partSize = (end1 - beg1)/nThreads;

    for (int i = 0; i < nThreads-1; ++i) {
        threads.emplace_back(std::thread(serialScalarProduct,
                                         beg1, beg1 + partSize,
                                         beg2, beg2 + partSize,
                                         std::ref(result)));
        beg1 += partSize;
        beg2 += partSize;
    }

    threads.emplace_back(std::thread(serialScalarProduct,
                                     beg1, end1,
                                     beg2, end2,
                                     std::ref(result)));

    for (auto &t : threads)
        t.join();
}


int main() {

    std::default_random_engine e1(42);
    std::uniform_int_distribution<int> uniform_dist(1, 6);

    const int n = 8192;
    std::vector<int> v1(n);
    std::vector<int> v2(n);

    for (int i = 0; i < n; ++i) {
        v1[i] = uniform_dist(e1);
        v2[i] = uniform_dist(e1);
    }

    int serial = 0;
    serialScalarProduct(v1.begin(), v1.end(), v2.begin(), v2.end(), serial);

    int parallel = 0;
    parallelScalarProduct(v1.begin(), v1.end(), v2.begin(), v2.end(), parallel);

    std::cout << "serial:   " << serial << std::endl;
    std::cout << "parallel: " << parallel << std::endl;

    assert(parallel == serial);

    return 0;
}

