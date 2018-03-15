#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>


/*
 * MyVector keeps the capacity of the backing array (buffer_) between
 * size_ / 4 <= capacity_ <= 2 * size_
 *
 */
template<typename T>
class MyVector {
private:
    size_t size_;
    size_t capacity_;

    T *buffer_;

    void resize(size_t new_capacity) {
        capacity_ = new_capacity;
        T *new_buffer = new T[capacity_];

        if (buffer_ != nullptr) {
            std::copy(buffer_, buffer_ + size_, new_buffer);
            delete[] buffer_;
        }

        buffer_ = new_buffer;
    }

public:
    MyVector() : size_(0), capacity_(0), buffer_(nullptr) {}

    MyVector(MyVector &other) : size_(other.size_),
                                capacity_(other.capacity_),
                                buffer_(nullptr) {
        if (size_ > 0) {
            buffer_ = new T[capacity_];
            std::copy(other.buffer_, other.buffer_ + size_, buffer_);
        }
    }

    MyVector(MyVector &&other) : size_(other.size_),
                                 capacity_(other.capacity_),
                                 buffer_(other.buffer_) {
        other.size_ = 0;
        other.capacity_ = 0;
        other.buffer_ = nullptr;
    }

    MyVector &operator=(const MyVector &other) {
        clear();
        size_ = other.size_;
        capacity_ = other.capacity_;

        if (other.buffer_ != nullptr) {
            buffer_ = new T[capacity_];
            std::copy(other.buffer_, other.buffer_ + size_, buffer_);
        }

        return *this;
    }

    ~MyVector() {
        delete[] buffer_;
    }

    size_t size() const {
        return size_;
    }

    size_t capacity() const {
        return capacity_;
    }

    bool empty() const {
        return size_ == 0;
    }

    void push_back(const T &el) {
        if (size_ == capacity_) {
            size_t new_capacity = capacity_ ? capacity_ * 2 : 2;
            resize(new_capacity);
        }

        buffer_[size_++] = el;
    }

    T &back() {
        return buffer_[size_ - 1];
    }

    void pop_back() {
        --size_;
        if (capacity_ > 2 && size_ <= capacity_ / 4)
            resize(capacity_ / 2);
    }

    void clear() {
        delete[] buffer_;
        buffer_ = nullptr;
        size_ = 0;
        capacity_ = 0;
    }

    T &operator[](size_t pos) {
        return buffer_[pos];
    }

    T *begin() {
        return buffer_;
    }

    T *end() {
        if (buffer_ == nullptr)
            return nullptr;
        return buffer_ + size_;
    }
};

/*
 * SHELLSORT
 *
 */
void generate_pratt_seq(std::vector<int> &gaps, int max) {
    gaps.push_back(1);
    int row_parent = 0;

    for (int row_len = 2; true; ++row_len) {
        for (int row_idx = 0; row_idx < row_len - 1; ++row_idx) {
            gaps.push_back(gaps[row_parent + row_idx] * 2);

            if (gaps.back() >= max)
                return;
        }

        gaps.push_back(gaps[row_parent + row_len - 2] * 3);
        if (gaps.back() >= max) return;
        row_parent = row_parent + row_len - 1;
    }
}

template<typename Container>
void shellSort(Container &A) {
    std::vector<int> gaps;
    generate_pratt_seq(gaps, A.size());
    std::reverse(gaps.begin(), gaps.end());

    for (auto gap : gaps) {
        for (size_t i = gap; i < A.size(); ++i) {
            int j = i;
            auto v = A[j];

            while (j >= gap && v < A[j - gap]) {
                A[j] = A[j - gap];
                j -= gap;
            }

            A[j] = v;
        }
    }
}

/*
 * HEAPSORT
 *
 */
static inline int heap_left(int idx) {
    return 2 * idx + 1;
}

static inline int heap_right(int idx) {
    return 2 * idx + 2;
}

template<typename Container>
void heapify(Container &A, int pos, int size) {
    int i = pos;

    while (true) {
        int max = i;
        if (heap_left(i) < size && A[heap_left(i)] > A[max])
            max = heap_left(i);
        if (heap_right(i) < size && A[heap_right(i)] > A[max])
            max = heap_right(i);
        if (max == i)
            break;

        std::swap(A[i], A[max]);
        i = max;
    }
}

template<typename Container>
void heapSort(Container &A) {
    for (int i = A.size() / 2; i >= 0; --i) {
        heapify(A, i, A.size());
    }

    for (int i = A.size() - 1; i >= 1; --i) {
        std::swap(A[0], A[i]);
        heapify(A, 0, i);
    }
}


int main() {
    MyVector<int> myvec;

    int e;

    do {
        std::cin >> e;
        myvec.push_back(e);
    } while (e != 0);

    MyVector<int> shelltest = myvec;
    MyVector<int> heaptest = myvec;

    shellSort(shelltest);
    heapSort(heaptest);

    std::cout << "Shellsort: ";
    for (auto x : shelltest) {
        std::cout << x << " ";
    }
    std::cout << std::endl;

    std::cout << "Heapsort:  ";
    for (auto x : heaptest) {
        std::cout << x << " ";
    }
    std::cout << std::endl;

    return 0;
}
