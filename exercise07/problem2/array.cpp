#include <algorithm>
#include <cassert>

#include "array.hpp"

void Array::swap(Array &a) {
    std::swap(width, a.width);
    std::swap(height, a.height);
    std::swap(data, a.data);
}

Array::Array(unsigned int w, unsigned int h) : width{w}, height{h} {
    data = new double[width * height];
}

Array::Array(const Array &a) : width{a.width}, height{a.height} {
    data = new double[width * height];
    std::copy(a.data, a.data + width * height, data);
}

Array::~Array() {
    assert(data != nullptr);
    delete[] data;
}

Array &Array::operator=(const Array &a) {
    Array cpy(a);
    swap(cpy);
    return *this;
}

double &Array::operator()(unsigned int x, unsigned int y) {
    assert(x < width);
    assert(y < height);
    return data[y * width + x];
}

double Array::operator()(unsigned int x, unsigned int y) const {
    assert(x < width);
    assert(y < height);
    return data[y * width + x];
}

unsigned int Array::Width() const {
    return width;
}

unsigned int Array::Height() const {
    return height;
}

Array BitmapToArray(const Bitmap &bitmap) {
    unsigned int width = bitmap.Width();
    unsigned int height = bitmap.Height();
    Array result(width, height);
    for (unsigned int y = 0; y < height; ++y) {
        for (unsigned int x = 0; x < width; ++x) {
            result(x, y) = bitmap(x, y).GetR();
        }
    }
    return result;
}

Bitmap ArrayToBitmap(const Array &array) {
    unsigned int width = array.Width();
    unsigned int height = array.Height();
    Bitmap result(width, height);
    for (unsigned int y = 0; y < height; ++y) {
        for (unsigned int x = 0; x < width; ++x) {
            float value = array(x, y);
            result(x, y) = Color(value, value, value);
        }
    }
    return result;
}
