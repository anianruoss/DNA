#include <iostream>
#include <string>


template<typename It1, typename It2>
It1 findOccurrence(const It1 from, const It1 to, const It2 begin, const It2 end) {
    const unsigned M = 32768;
    const unsigned C = 1021;

    unsigned c_to_k = 1;
    unsigned hash_a = 0;
    unsigned hash_b = 0;

    It1 window_end = from;
    for (It2 current = begin; current != end; ++current, ++window_end) {
        if (window_end == to)
            return to;

        hash_a = ((C * hash_a) % M + *window_end) % M;
        hash_b = ((C * hash_b) % M + *current) % M;
        c_to_k = (c_to_k * C) % M;
    }

    for (It1 window_begin = from;; ++window_begin, ++window_end) {
        if (hash_a == hash_b && std::equal(window_begin, window_end, begin, end))
            return window_begin;
        if (window_end == to)
            return to;

        hash_a = (C * hash_a + *window_end + (M - c_to_k) * *window_begin) % M;
    }
}

int main() {
    std::string input;
    std::getline(std::cin, input);

    std::string substring;
    std::getline(std::cin, substring);

    auto pos = findOccurrence(input.begin(), input.end(),
                              substring.begin(), substring.end());

    if (pos == input.end()) {
        std::cout << "NOT FOUND" << std::endl;
    } else {
        std::cout << std::distance(input.begin(), pos) << std::endl;
    }

    return 0;
}
