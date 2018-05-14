#include <algorithm>
#include <cassert>
#include <iostream>
#include <vector>


class UnionFind {
  private:
    std::vector<int> parents;

  public:
    UnionFind(int n) : parents(n, -1) {}

    int find(int n) {
        while (parents[n] != -1)
            n = parents[n];

        return n;
    }

    void unite(int a, int b) {
        parents[ find(a) ] = b;
    }

    bool in_same_union(int a, int b) {
        return find(a) == find(b);
    }
};


int main() {
    int n, i, j;
    std::cin >> n >> i >> j;

    UnionFind uf(n);

    for (int x=0; x<i; ++x) {
        int a, b;
        std::cin >> a >> b;
        uf.unite(a,b);
    }

    for (int x = 0; x < j; ++x) {
        int a, b;
        std::cin >> a >> b;

        if (uf.in_same_union(a,b)) {
            std::cout << "true ";
        } else {
            std::cout << "false ";
        }
    }

    std::cout << std::endl;

    return 0;
}

