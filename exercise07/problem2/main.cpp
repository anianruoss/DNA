#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <vector>

#include "array.hpp"
#include "bitmap.hpp"


template <typename Function>
Array Sum(unsigned int width, unsigned int height, Function f) {
    Array sum (width+1, height+1);

    for (unsigned int y = 0; y <= height; ++y) {
        float sx = 0;
        for (unsigned int x = 0; x <= width; ++x) {
            if (x == 0 || y == 0) {
                sum(x, y) = 0;
            } else {
                sx += f(x-1, y-1);
                sum(x, y) = sx + sum(x, y-1);
            }
        }
    }

    return sum;
}

double GetSum(const Array& sum, unsigned int x, unsigned int y, unsigned int w,
              unsigned int h) {
    return sum(x+w, y+h) - sum(x+w, y) - sum(x, y+h) + sum(x,y);
}

void Fill(Array& result, double value, unsigned int x, unsigned int y,
          unsigned int w, unsigned int h) {
    for (unsigned int ox = 0; ox < w; ++ox) {
        for (unsigned int oy = 0; oy < h; ++oy) {
            result(x+ox, y+oy) = value;
        }
    }
}

double Minimize(double gamma, Array& result, const Array& sum,
                const Array& ssq, unsigned int x, unsigned int y,
                unsigned int w, unsigned int h) {
    if (w == 1 || h == 1) {
        result(x,y) = GetSum(sum, x, y, w, h);
        return gamma;
    }

    double s = GetSum(sum, x, y, w, h);
    double e = GetSum(ssq, x, y, w, h) - (s*s / (w*h)) + gamma;

    double se = Minimize(gamma, result, sum, ssq, x, y, w/2, h/2);
    se += Minimize(gamma, result, sum, ssq, x+w/2, y, w/2, h/2);
    se += Minimize(gamma, result, sum, ssq, x, y+h/2, w/2, h/2);
    se += Minimize(gamma, result, sum, ssq, x+w/2, y+h/2, w/2, h/2);

    if (e < se) {
        Fill(result, s/(w*h), x, y, w, h);
        return e;
    } else {
        return se;
    }
}

void ShowResult(const Bitmap& original, const Array& result,
                const std::string filename) {
    std::string make_dir = "mkdir -p bin/results/" + filename;

    const int dir_err = system(make_dir.c_str());
    if (dir_err == -1) {
        printf("Error creating directory!");
        exit(1);
    }

    Bitmap resbmp = ArrayToBitmap(result);
    std::string originalF = "bin/results/" + filename + "/original.bmp";
    original.Save(originalF);
    std::string resultF = "bin/results/" + filename + "/result.bmp";
    resbmp.Save(resultF);

    std::cout << "Original: " << originalF << std::endl;
    std::cout << "Result: " << resultF << std::endl;
}


int main() {
    std::cout << std::setprecision(3);
    std::cout << "image name (e.g. bird, peppers or sailboat)? ";
    std::string filename;
    std::cin >> filename;
    std::string file = filename + ".pgm";
    Bitmap bitmap = readpgm(file);

    std::cout << "input gamma (e.g. 0.01)? ";
    double gamma=0.1;
    std::cin >> gamma;
    Array img = BitmapToArray(bitmap);
    unsigned int width = img.Width();
    unsigned int height = img.Height();

    Array sum = Sum(width, height, img);
    Array ssq = Sum(width, height, [&] (unsigned int x, unsigned int y) {
        return img(x,y)*img(x,y);
    });
    Array result(width,height);

    std::cout << std::endl;
    std::cout << "minimum = "
              << Minimize(gamma, result, sum, ssq, 0, 0, width, height)
              << std::endl;

    ShowResult(bitmap, result, filename);

    return  0;
}

