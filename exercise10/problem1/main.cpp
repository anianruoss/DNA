#include <algorithm>
#include <iostream>
#include <vector>


enum Orientation {
    CCW = 1,
    CW = -1,
    COLL = 0
};

typedef long long Coordinate;
typedef std::pair<Coordinate, Coordinate> Point;

Orientation getOrientation(Point a, Point b, Point c) {
    Coordinate det = (b.first - a.first)*(c.second - a.second)
                     - (b.second - a.second)*(c.first - a.first);

    if (det > 0)
        return Orientation::CCW;
    if (det < 0)
        return Orientation::CW;

    return Orientation::COLL;
}


int main() {
    size_t n = 0;
    std::cin >> n;

    Point triple[3];
    while (n) {
        for (unsigned i = 0; i < 3; ++i)
            std::cin >> triple[i].first >> triple[i].second;

        std::cout << getOrientation(triple[0], triple[1], triple[2]);
        --n;

        if (n)
            std::cout << " ";
    }

    std::cout << std::endl;

    return 0;
}

