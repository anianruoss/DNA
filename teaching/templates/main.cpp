#include <iostream>
#include <vector>


template <typename T>
T square(T x) {
    return x*x;
}

template <typename Container, typename F>
void apply(Container &c, F f) {
    for (auto &x: c) {
        x = f(x);
    }
}

template <typename Container>
void output(Container &c) {
    for (auto &x: c) {
        std::cout << x << " ";
    }
    std::cout << std::endl;
}


int main() {
    std::vector<int> vec = {1, 2, 3};
    apply(vec, square<int>);
    output(vec);

    return 0;
}

