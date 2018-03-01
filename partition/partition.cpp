#include <algorithm>
#include <iostream>


unsigned int partition(int *array, int l, int r, int p) {
    while (l <= r) {
        while (array[l] < p) ++l;
        while (array[r] > p) --r;

        std::swap(array[l], array[r]);

        if (array[l] == array[r]) ++l;
    }

    return l-1;
}


int main() {
    int n, l, r, p;

    std::cout << "Array length:" << std::endl << "n = ";
    std::cin >> n;

    std::cout << "Array:" << std::endl;

    int array[n];
    for (int i = 0; i < n; ++i) {
        std::cin >> array[i];
    }

    std::cout << "Pivot bounds:" << std::endl << "l = ";
    std::cin >> l;

    std::cout << "r = ";
    std::cin >> r;

    std::cout << "Pivot:" << std::endl << "p = ";
    std::cin >> p;

    int pos = partition(array, l-1, r-1, p);

    std::cout << std::endl << "Array after pivoting:" << std::endl;

    for (int i = 0; i < n; ++i) {
        std::cout << array[i] << " ";
    }

    std::cout << std::endl;

    std::cout << "Pivot position: " << pos+1 << std::endl;

    return 0;
}

