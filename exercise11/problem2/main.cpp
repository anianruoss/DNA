#include <cassert>
#include <chrono>
#include <iostream>
#include <random>
#include <thread>
#include <vector>


void merge(std::vector<int> &A, const int &l, const int &m, const int &r) {
    std::vector<int> B(r-l+1);

    int i = l;
    int j = m+1;
    int k = 0;

    while (i<=m and j<=r) {
        if (A[i]<=A[j]) {
            B[k] = A[i];
            ++i;
        } else {
            B[k] = A[j];
            ++j;
        }
        ++k;
    }

    while (i <= m) {
        B[k] = A[i];
        ++i;
        ++k;
    }

    while (j <= r) {
        B[k] = A[j];
        ++j;
        ++k;
    }

    for (int k = 0; k <= (r-l); ++k)
        A[k+l] = B[k];
}


void mergesort(std::vector<int> &A, const int &l, const int &r) {
    if (l < r) {
        int m = (l + r) / 2;
        mergesort(A, l, m);
        mergesort(A, m+1, r);
        merge(A, l, m, r);
    }
}


void mergesort_seq(std::vector<int> &v) {
    mergesort(v, 0, v.size()-1);
}


void mergesort_par(std::vector<int> &v) {
    int n = v.size();
    int nThreads = std::thread::hardware_concurrency();

    std::vector<std::thread> threads;
    int partSize =  n / nThreads;

    for (int i = 0; i < nThreads-1; ++i)
        threads.emplace_back(mergesort,
                             std::ref(v),
                             partSize*i, partSize*(i+1)-1);

    threads.emplace_back(mergesort,
                         std::ref(v),
                         partSize*(nThreads-1), n-1 );

    for (auto &t : threads)
        t.join();

    while (nThreads/2 > 1) {
        nThreads /= 2;
        std::vector<std::thread> threads;

        for (int i = 0; i < nThreads-1; ++i)
            threads.emplace_back(merge,
                                 std::ref(v),
                                 partSize*2*i,
                                 partSize*(2*i+1)-1,
                                 partSize*(2*i+2)-1);

        threads.emplace_back(merge,
                             std::ref(v),
                             partSize*(2*nThreads-2),
                             partSize*(2*nThreads-1)-1,
                             n-1);

        for (auto &t : threads )
            t.join();

        partSize *= 2;
    }

    merge(v, 0, partSize-1, n-1);
}


int main() {

    int n;
    std::cin >> n;

    std::default_random_engine e1( 42 );
    std::uniform_int_distribution<int> uniform_dist(-10, 10);

    std::vector<int> v_seq( n );
    for (int i = 0; i < n; ++i)
        v_seq[i] = uniform_dist(e1);
    std::vector<int> v_par = v_seq;

    {
        auto t1 = std::chrono::high_resolution_clock::now();
        mergesort_seq(v_seq);
        auto t2 = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> ms = t2 - t1;
        std::cout << std::fixed
                  << "sequential mergesort: " << ms.count() << " ms"
                  << std::endl;

        for (size_t i = 1; i < v_seq.size(); ++i)
            assert(v_seq[i-1] <= v_seq[i]);
    }
    {
        auto t1 = std::chrono::high_resolution_clock::now();
        mergesort_par(v_par);
        auto t2 = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> ms = t2 - t1;
        std::cout << std::fixed
                  << "parallel mergesort:   " << ms.count() << " ms"
                  << std::endl;

        for (size_t i = 1; i < v_par.size(); ++i)
            assert(v_par[i-1] <= v_par[i]);
    }

    return 0;
}

