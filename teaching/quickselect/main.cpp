#include <algorithm>
#include <cassert>
#include <iomanip>
#include <iostream>
#include <random>
#include <vector>


size_t sep = 20;

class QuickSelect {
  public:
    using Iterator = std::vector<int>::iterator;

    QuickSelect(const std::vector<int>& a) : v(a) {}

    int select(size_t k, bool naive) {
        step = 0;
        Iterator begin = v.begin();
        Iterator end = v.end();

        size_t n = end - begin;
        assert(k < n);

        std::mt19937 gen(123);

        while (n > 1) {
            // choose bad pivot on purpose
            Iterator pivot = begin;
            Iterator pos;

            if (naive) {
                pos = naivePartition(begin, end, *pivot);
            } else {
                pos = cleverPartition(begin, end, *pivot);
            }

            size_t offset = pos - begin;
            print(offset);

            if (offset < k) {
                begin = pos + 1;
                k -= offset + 1;
            } else if (offset > k) {
                end = pos;
            } else {
                return *pos;
            }

            n = end - begin;
        }

        return *begin;
    }

  private:
    std::vector<int> v;
    size_t step = 0;

    Iterator naivePartition(Iterator begin, Iterator end, int pivot) {
        auto left = begin;
        auto right = end - 1;

        while (left <= right) {
            while (*left < pivot) ++left;
            while (pivot < *right) --right;

            std::swap(*left, *right);

            if (*left == *right) ++left;
        }

        return left - 1;
    }

    Iterator cleverPartition(Iterator begin, Iterator end, int pivot) {
        auto left = begin;
        auto right = end - 1;
        bool do_left = true;

        while (left <= right) {
            while (*left < pivot) ++left;
            while (pivot < *right) --right;

            if (*left == *right) {
                if (left == right || do_left)
                    ++left;
                else
                    --right;

                do_left = !do_left;

            } else {
                std::swap(*left, *right);
            }
        }

        return left - 1;
    }


    void print(size_t offset) {
        std::cout << std::setw(sep) << "STEP " << ++step
                  << std::setw(sep) << v.size() - offset - 1
                  << std::setw(sep) << offset
                  << std::endl;
    }
};


int main() {
    const size_t k = 0;
    std::vector<int> vec {6, 6, 6, 6, 6, 6};

    std::cout << "Finding the " << k << "-th smallest element from: ";
    for (auto x : vec)
        std::cout << x << " ";
    std::cout << std::endl << std::endl;

    QuickSelect qs(vec);

    std::cout << std::left << std::setw(sep) << "NAIVE Partition"
              << std::setw(2*sep) << "Number of Elements:"
              << std::endl << std::right << std::endl
              << std::setw(sep) << ""
              << std::setw(sep) << "Left Partition"
              << std::setw(sep) << "Right Partition"
              << std::endl;

    qs.select(k, true);

    std::cout << std::endl << std::endl
              << std::left << std::setw(sep) << "CLEVER Partition"
              << std::setw(2*sep) << "Number of Elements:"
              << std::endl << std::internal << std::endl
              << std::setw(sep) << ""
              << std::setw(sep) << "Left Partition"
              << std::setw(sep) << "Right Partition"
              << std::endl;

    qs.select(k, false);

    return 0;
}

