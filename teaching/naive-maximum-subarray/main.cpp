#include <iostream>


void naiveMaximumSubarray(double *array, int n, double &M, int &I, int &J) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            double m = 0.;

            for (int k = i; k < j; ++k)
                m += array[k];

            if (m > M) {
                M = m;
                I = i;
                J = j;
            }
        }
    }
}


int main() {
    int n;

    std::cout << "Array length:" << std::endl << "n = ";
    std::cin >> n;

    std::cout << "Array:" << std::endl;
    double array[n];

    for (int i = 0; i < n; ++i) {
        std::cin >> array[i];
    }

    double M = 0.;
    int I = 0;
    int J = 0;

    naiveMaximumSubarray(array, n, M, I, J);

    std::cout << std::endl;
    std::cout << "Maximum subarray: [" << I << "," << J << ")" << std::endl;
    std::cout << "Sum of subarray: " << M << std::endl;

    return 0;
}

