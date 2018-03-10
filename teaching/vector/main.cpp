#include <algorithm>
#include <cassert>
#include <iostream>


template<typename T>
class FixedVector {
private:
    size_t size_;
    size_t elements_;
    T *buffer_;

public:
    FixedVector(size_t l = 0) : size_(l),
                                elements_(0),
                                buffer_(new T[size_]) {}

    /* Rule of Three:
     *      - Destructor
     *      - Copy Constructor
     *      - Copy Assignment Operator
     */
    ~FixedVector() {
        delete[] buffer_;
    }

    FixedVector(FixedVector &other) : size_(other.size_),
                                      elements_(other.elements_),
                                      buffer_(new T[size_]) {
        std::cout << "-> Copy Constructor" << std::endl;

        if (elements_ > 0)
            std::copy(other.buffer_, other.buffer_ + elements_, buffer_);
    }

    FixedVector &operator=(const FixedVector &other) {
        std::cout << "-> Copy Assignment Operator" << std::endl;

        clear();
        size_ = other.size_;
        elements_ = other.elements_;
        buffer_ = new T[size_];
        std::copy(other.buffer_, other.buffer_ + size_, buffer_);

        return *this;
    }

    void push_back(const T &el) {
        assert(elements_ < size_);
        buffer_[elements_++] = el;
    }

    void clear() {
        delete[] buffer_;
        buffer_ = nullptr;
        size_ = 0;
        elements_ = 0;
    }

    void print() {
        for (size_t i = 0; i < elements_; ++i)
            std::cout << buffer_[i] << " ";
        std::cout << std::endl;
    }

    void pop_back() {
        --elements_;
    }
};


int main() {
    std::cout << "Create empty vector:" << std::endl;
    FixedVector<size_t> vec(8);
    vec.print();

    std::cout << "Add some elements:" << std::endl;
    for (size_t i = 0; i < 8; ++i)
        vec.push_back(i);
    vec.print();
    std::cout << std::endl;

    std::cout << "Create some copies:" << std::endl;
    FixedVector<size_t> copy1(vec);
    FixedVector<size_t> copy2;
    copy2 = copy1;
    std::cout << std::endl;

    std::cout << "Original: ";
    vec.print();
    std::cout << "Copy1: ";
    copy1.print();
    std::cout << "Copy2: ";
    copy2.print();
    std::cout << std::endl;

    std::cout << "Delete some elements:" << std::endl;
    for (size_t i = 0; i < 4; ++i)
        vec.pop_back();
    for (size_t i = 0; i < 2; ++i)
        copy1.pop_back();
    for (size_t i = 0; i < 1; ++i)
        copy2.pop_back();

    std::cout << "Original: ";
    vec.print();
    std::cout << "Copy1: ";
    copy1.print();
    std::cout << "Copy2: ";
    copy2.print();

    return 0;
}
