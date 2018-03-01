#include <iostream>
#include <vector>


void print(const std::vector<std::vector<int>> &a) {
    size_t n = a.size();
    size_t m = a[0].size();

    for (int i = m-1; i >= 0; --i) {
        for (int j = 0; j < n; ++j) {
            std::cout << a[j][i] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}


class PrefixSum {
  public:
    PrefixSum(const std::vector<std::vector<int> >& a) : pSum(a) {
        if (a.size() == 0) return;

        size_t n = a.size();
        size_t m = a[0].size();

        std::cout << "Original:" << std::endl;
        print(pSum);

        std::cout << "Horizontal Summation:" << std::endl;
        for(size_t i=1; i<n; i++)
            pSum[i][0] += pSum[i-1][0];
        print(pSum);

        std::cout << "Vertical Summation:" << std::endl;
        for(size_t j=1; j<m; j++)
            pSum[0][j] += pSum[0][j-1];
        print(pSum);

        std::cout << "Quadratic Summation:" << std::endl;
        for(size_t i=1; i<n; i++)
            for(size_t j=1; j<m; j++)
                pSum[i][j] += (pSum[i-1][j] + pSum[i][j-1] - pSum[i-1][j-1]);
        print(pSum);
    }

    // Pre: 0 <= x0 <= x1 < n and 0 <= y0 <= y1 < m
    int sum(size_t x0, size_t x1, size_t y0, size_t y1) {
        int num =                      pSum[x1  ][y1  ];
        int a   = (y0 > 0)           ? pSum[x1  ][y0-1] : 0;
        int b   = (x0 > 0)           ? pSum[x0-1][y1  ] : 0;
        int c   = (y0 > 0 && x0 > 0) ? pSum[x0-1][y0-1] : 0;

        return (num - a - b + c);
    }

  private:
    std::vector<std::vector<int>> pSum;
};


int main() {
    size_t n, m, x0, x1, y0, y1;

    std::cin >> n;
    std::cin >> m;

    std::vector<std::vector<int>> a(n, std::vector<int>(m));

    for(size_t i = 0; i < n; ++i)
        for(size_t j = 0; j < m; ++j)
            std::cin >> a[i][j];
    std::cout << std::endl;

    std::cin >> x0;
    std::cin >> x1;
    std::cin >> y0;
    std::cin >> y1;

    PrefixSum prefixSum(a);
    std::cout << "Sum = " << prefixSum.sum(x0, x1, y0, y1) << std::endl;

    return 0;
}

