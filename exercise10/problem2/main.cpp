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

    std::vector<Point> points;
    for (size_t i = 0; i < n; ++i) {
        unsigned x, y;
        std::cin >> x >> y;
        points.push_back(Point(x, y));
    }

    if (n == 0) {
        std::cout << 0 << std::endl;
        return 0;
    }

    if (n == 1) {
        std::cout << 1 << std::endl << points[0].first << " " << points[0].second << std::endl;
        return 0;
    }

    size_t min_index = 0;
    for (size_t i = 1; i < n; ++ i) {
        if (points[i] < points[min_index])
            min_index = i;
    }

    std::swap(points[min_index], points[0]);
    std::sort(points.begin() + 1, points.end(), [&] (Point a, Point b) {
        Orientation o = getOrientation(points[0], b, a);
        if (o == Orientation::COLL)
            return a < b;
        return o == Orientation::CCW;
    });

    std::vector<Point> hull;
    hull.push_back(points[0]);
    hull.push_back(points[1]);

    for (size_t i = 2; i < n; ++i) {
        while (hull.size() >= 2 && getOrientation(hull[hull.size()-2], hull.back(), points[i]) != Orientation::CW)
            hull.pop_back();
        hull.push_back(points[i]);
    }

    if (hull.size() == 2 && hull[0] == hull[1])
        hull.pop_back();

    std::cout << hull.size() << std::endl;

    for (auto p : hull)
        std::cout << p.first << " " << p.second << " ";
    std::cout << std::endl;

    return 0;
}

