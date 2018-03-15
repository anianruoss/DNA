#include <cassert>
#include <vector>


template<typename T>
struct Node {
    Node *next;
    T val;

    Node(Node *n, T v) : next(n), val(v) {}
};

template<typename T>
class Queue {
private:
    Node<T> *head_ = nullptr;
    Node<T> *tail_ = nullptr;

public:
    void enqueue(T val) {
        Node<T> *node = new Node<T>{nullptr, val};

        if (tail_ != nullptr)
            tail_->next = node;

        tail_ = node;

        if (head_ == nullptr)
            head_ = tail_;
    }

    T dequeue() {
        assert(head_ != nullptr);

        Node<T> *tmp = head_;
        T val = tmp->val;
        head_ = head_->next;

        if (head_ == nullptr)
            tail_ = nullptr;

        delete tmp;

        return val;
    }

    Node<T> *head() {
        return head_;
    }

    bool isEmpty() {
        return head_ == nullptr;
    }

    void emptyQueue() {
        while (head_ != nullptr)
            dequeue();
    }
};


int main() {
    std::vector<int> vec{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    Queue<int> queue;

    // test enqueue and dequeue
    for (auto x : vec)
        queue.enqueue(x);
    for (auto x : vec)
        assert(x == queue.dequeue());

    // test isEmpty
    assert(queue.isEmpty());
    for (auto x : vec)
        queue.enqueue(x);
    assert(!queue.isEmpty());

    // test emptyQueue
    queue.emptyQueue();
    assert(queue.isEmpty());

    return 0;
}
