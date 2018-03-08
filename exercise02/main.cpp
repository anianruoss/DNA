#include <cstdlib>
#include <iostream>
#include <vector>


using Iterator = std::vector<int>::iterator;

class Selector {
  private:
    std::vector<int> vec;
    const size_t CUTOFF = 10;

    Iterator partition(Iterator begin, Iterator end, int pivot) {
        auto left = begin;
        auto right = end - 1;
        bool do_left = true;

        while (left <= right) {
            while (*left < pivot) ++left;
            while (pivot < *right) --right;

            if (*left == *right) {
                if (left == right || do_left) {
                    ++left;
                } else {
                    --right;
                }
                do_left = !do_left;
            } else {
                std::swap(*left, *right);
            }
        }

        return left - 1;
    }

    const Iterator selectBruteForce(const Iterator begin, const Iterator end,
                                    size_t k) {
        for (auto current = begin; current != end; ++current) {
            size_t less = 0;
            size_t equal = 0;

            for (auto other = begin; other != end; ++other) {
                if (*other < *current)
                    ++less;
                else if (*current == *other)
                    ++equal;
            }

            if (less <= k && less + equal > k)
                return current;
        }

        return end;
    }

    const Iterator selectRandomized(Iterator begin, Iterator end, size_t k) {
        srand(123);
        size_t n = end - begin;

        while (n > CUTOFF) {
            auto pivot = begin + (rand() % n);
            auto pos = partition(begin, end, *pivot);
            size_t offset = pos - begin;

            if (offset < k) {
                begin = pos;
                k -= offset;
            } else if (offset > k)
                end = pos;
            else
                return pos;

            n = end - begin;
        }

        return selectBruteForce(begin, end, k);
    }


    Iterator selectBlum(Iterator begin, Iterator end, size_t k) {
        size_t n = end - begin;

        while (n > CUTOFF && n > 5) {
            auto medians = begin;

            for (auto current = begin; current < end - 5; current += 5) {
                auto m = selectBruteForce(current, current + 5, 2);
                std::swap(*medians, *m);
                ++medians;
            }

            auto m = selectBlum(begin, medians, (medians - begin) / 2);
            auto pos = partition(begin, end, *m);
            size_t offset = pos - begin;

            if (offset < k) {
                begin = pos;
                k -= offset;
            } else if (offset > k)
                end = pos;
            else
                return pos;
            n = end - begin;
        }

        return selectBruteForce(begin, end, k);
    }


  public:
    Selector(const std::vector<int>& a): vec(a) {}

    int selectBruteForce(size_t k) {
        return *selectBruteForce(vec.begin(), vec.end(), k);
    }

    int selectRandomized(size_t k) {
        return *selectRandomized(vec.begin(), vec.end(), k);
    }

    int selectBlum(size_t k) {
        return *selectBlum(vec.begin(), vec.end(), k);
    }
};


int main() {
    size_t size;
    std::cin >> size;

    std::vector<int> vec(size);

    for (size_t i = 0; i < size; ++i) {
        std::cin >> vec[i];
    }

    size_t k;
    std::cin >> k;

    Selector selector(vec);

    std::cout << selector.selectBruteForce(k) << std::endl;
    std::cout << selector.selectRandomized(k) << std::endl;
    std::cout << selector.selectBlum(k) << std::endl;

    return 0;
}

