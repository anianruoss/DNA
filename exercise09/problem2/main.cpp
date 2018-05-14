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


class Edge {
  public:
    int u, v, c;
    Edge(int mu, int mv, int mc) : u(mu), v(mv), c(mc) {}

    bool operator < (const Edge& other) const {
        return c < other.c;
    }
};


std::ostream &operator<< (std::ostream &os, const Edge & e) {
    return os << "Edge(" << e.u << "," << e.v << "," << e.c << ")";
}


class Graph {
  private:
    int n;
    std::vector<Edge> edges;

  public:
    Graph(int mn) : n(mn) {}

    void add_edge(Edge e) {
        edges.push_back(e);
    }

    void kruskal(Graph & out) {
        UnionFind uf(n+1);

        std::sort(edges.begin(), edges.end());
        for (auto e : edges) {
            if (!uf.in_same_union(e.u, e.v)) {
                out.add_edge(e);
                uf.unite(e.u, e.v);
            }
        }
    }

    int cost() {
        int sum = 0;

        for (auto e : edges) {
            sum += e.c;
        }

        return sum;
    }
};


int main() {
    int n, m;
    std::cin >> n >> m;
    Graph g = Graph(n);

    for (int i = 0; i < m; ++i) {
        int u, v, c;
        std::cin >> u >> v >> c;
        Edge e(u,v,c);
        g.add_edge(e);
    }

    Graph mst(n);
    g.kruskal(mst);

    std::cout << mst.cost() << std::endl;

    return 0;
}

