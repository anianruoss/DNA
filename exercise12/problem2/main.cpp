#include <algorithm>
#include <chrono>
#include <iostream>
#include <mutex>
#include <random>
#include <thread>
#include <vector>


template<class T>
class LinkedList {
  private:
    struct Node {
        Node* next = nullptr;
        T val;
        std::mutex m;
        Node(T v) : val(v) {};
    };

    Node* head ;

  public:
    LinkedList(): head(new Node(0)) {};

    ~LinkedList() {
        while (head != nullptr) {
            Node* prev = head;
            head = head->next;
            delete prev;
        }
    }

    LinkedList &operator=(const LinkedList<T> &rhs) = delete;
    LinkedList (const LinkedList<T> &rhs) = delete;

    void insert(T el) {
        Node* prev = head;
        prev->m.lock();

        while (prev->next != nullptr && prev->next->val < el) {
            Node* next = prev->next;
            next->m.lock();
            prev->m.unlock();
            prev = next;
        }

        Node* next = prev->next;
        Node* new_node = new Node(el);
        new_node->next = next;
        prev->next = new_node;
        prev->m.unlock();
    }

    void print() {
        std::cout << "(";
        Node* prev = head;
        prev->m.lock();

        while (prev->next != nullptr) {
            Node* next = prev->next;
            next->m.lock();
            std::cout << next->val << " ";
            prev->m.unlock();
            prev = next;
        }

        prev->m.unlock();
        std::cout << ")" << std::endl;
    }

    void remove(const T val) {
        Node* prev = head;
        prev->m.lock();

        while (prev->next != nullptr) {
            Node* next = prev->next;
            next->m.lock();

            if (next->val == val) {
                prev->next = next->next;
                next->m.unlock();
                prev->m.unlock();
                return;
            }

            prev->m.unlock();
            prev = next;
        }

        prev->m.unlock();
    }
};


void test(LinkedList<int>& list, int seed) {
    {
        std::default_random_engine e1(seed);
        std::uniform_int_distribution<int> dist(1,100);
        for (int n = 0; n < 1000; ++n)
            list.insert(dist(e1));
    }
    {
        std::default_random_engine e1(seed);
        std::uniform_int_distribution<int> dist(1,100);
        for (int n = 0; n < 999; ++n)
            list.remove(dist(e1));
    }
}

void run_scenario(int n_threads, bool sequential) {
    LinkedList<int> list;
    std::vector<std::thread> threads(n_threads);

    int seed = 42;

    for (auto & thread: threads) {
        thread = std::thread(test,std::ref(list),seed++);
        if (sequential) thread.join();
    }

    if (!sequential)
        for (auto & thread: threads)
            thread.join();

    list.print();
}


int main() {
    int n_threads;
    std::cin >> n_threads;

    run_scenario(n_threads, true);
    run_scenario(n_threads, false);

    return 0;
}

