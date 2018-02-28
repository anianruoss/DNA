#include <iostream>
#include <vector>
// #include "tests.h"


class PrefixSum {
  private:
    std::vector<std::vector<int>> matrix;
    int rows = matrix.size();
    int columns = matrix[0].size();

  public:
    PrefixSum(const std::vector<std::vector<int>>& a) : matrix(a) {
        for (int n = 1; n < rows; ++n) {
            matrix[n][0] = matrix[n][0] + matrix[n-1][0];
        }

        for (int m = 1; m < columns; ++m) {
            matrix[0][m] = matrix[0][m] + matrix[0][m-1];
        }

        for (int n = 1; n < rows; ++n) {
            for (int m = 1; m < columns; ++m) {
                matrix[n][m] += matrix[n-1][m] + matrix[n][m-1] - matrix[n-1][m-1];
            }
        }
    }

    int sum (int x0, int x1, int y0, int y1) {
        if (x0 == 0 && y0 == 0) {
            return matrix[x1][y1];
        } else if (x0 == 0) {
            return matrix[x1][y1] - matrix[x1][y0-1];
        } else if (y0 == 0) {
            return matrix[x1][y1] - matrix[x0-1][y1];
        } else {
            return matrix[x1][y1] - matrix[x1][y0-1] - matrix[x0-1][y1] + matrix[x0-1][y0-1];
        }
    }
};

int main() {
    int rows, columns;
    std::cin >> rows >> columns;

    std::vector<std::vector<int>> matrix (rows, std::vector<int> (columns, 0));
    for (int n = 0; n < rows; ++n) {
        for (int m = 0; m < columns; ++m) {
            std::cin >> matrix[n][m];
        }
    }

    int x0, x1, y0, y1;
    std::cin >> x0 >> x1 >> y0 >> y1;

    PrefixSum psum(matrix);
    std::cout << psum.sum(x0,x1,y0,y1) << std::endl;

    return 0;
}

