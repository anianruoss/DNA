#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <vector>


template<typename Matrix>
void allPairsShortestPaths(unsigned int n, Matrix& m) {
    for (unsigned int k = 0; k < n; ++k) {
        for (unsigned int i = 0; i < n; ++i) {
            for (unsigned int j = i + 1; j < n; ++j) {
                if (k == i || k == j)
                    continue;
                if (m[i][k] == 0 || m[k][j] == 0)
                    continue;
                if (m[i][j] == 0 || m[i][k] + m[k][j] < m[i][j])
                    m[i][j] = m[j][i] =  m[i][k] + m[k][j];
            }
        }
    }
}


int main() {
    unsigned int n = 0;
    std::ifstream input("input.txt");
    char buffer[41];
    buffer[40] = '\0';

    for (unsigned int i = 0; i < 21; ++i)
        input.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    input >> n;

    std::cout << "vertices: " << n << std::endl;

    std::vector<std::vector<unsigned int>> adjacencies(n, std::vector<unsigned int>(n, 0));
    std::vector<std::string> names(n);

    for(unsigned int i = 0; i < n; ++i) {
        unsigned int index, degree, dummy;
        input >> index >> degree >> dummy;
        assert(i + 1 == index);

        while (input.peek() == ' ')
            input.get();

        input.read(buffer, 40);
        names[i] = buffer;

        for (unsigned int j = 0; j < degree; ++j) {
            unsigned int neighbor;
            input >> neighbor;
            adjacencies[i][neighbor - 1] = 1;
            adjacencies[neighbor - 1][i] = 1;
        }
    }

    allPairsShortestPaths(n, adjacencies);

    for (unsigned int i = 0; i < n; ++i) {
        std::cout << names[i] << ": ";
        unsigned int centrality = 0;

        for (unsigned int j = 0; j < n; ++j) {
            centrality += adjacencies[i][j];
        }

        std::cout << centrality << std::endl;
    }

    return 0;
}

