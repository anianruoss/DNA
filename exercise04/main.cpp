#include <algorithm>
#include <iostream>
#include <vector>
#include <sstream>
#include <string>


template<typename T>
class SkipList {
  private:
    struct Node {
        T value;
        std::vector<Node*> forward;

        Node (const T& v, int level) : value(v), forward(level, nullptr) {}

    };

    const float probability;
    const int maxLevel;
    Node* head;

    int randomLevel() const {
        int v = 1;
        while (((double)std::rand() / RAND_MAX) < probability && v < maxLevel)
            ++v;

        return v;
    }

    static int nodeLevel(const Node* v) {
        return v->forward.size();
    }

    static Node* makeNode(const T& val, int level) {
        return new Node(val, level);
    }

    Node* lowerBound(const T& searchValue) const {
        Node* x = head;

        for (int i = nodeLevel(head)-1; i >= 0; --i) {
            while (x->forward[i] != nullptr && x->forward[i]->value < searchValue)
                x = x->forward[i];
        }

        return x->forward[0];
    }

    std::vector<Node*> predecessors(const T& searchValue) const {
        std::vector<Node*> result(nodeLevel(head), nullptr);
        Node* x = head;

        for (int i = nodeLevel(head)-1; i >= 0; --i) {
            while (x->forward[i] != nullptr && x->forward[i]->value < searchValue)
                x = x->forward[i];
            result[i] = x;
        }

        return result;
    }


  public:
    SkipList() : probability(0.5), maxLevel(16) {
        int headKey = std::numeric_limits<T>::min();
        head = new Node(headKey, maxLevel);
        std::fill(head->forward.begin(), head->forward.end(), nullptr);
    }

    ~SkipList() {
        Node* node = head;
        while (node->forward[0] != nullptr) {
            Node* temp = node;
            node = node->forward[0];
            delete temp;
        }

        delete node;
    }

    void print() const {
        Node* temp = head->forward[0];

        while (temp != nullptr) {
            std::cout << temp->value << " ";
            temp = temp->forward[0];
        }

        std::cout << std::endl;
    }

    T* find(const T& searchValue) const {
        T* temp = nullptr;

        if (Node* x = lowerBound(searchValue)) {
            if (x != nullptr && x->value == searchValue)
                temp = &(x->value);
        }

        return temp;
    }

    void insert(const T& newValue) {
        std::vector<Node*> preds = predecessors(newValue);

        const int newNodeLevel = randomLevel();
        Node* newNodePtr = makeNode(newValue, newNodeLevel);

        for (int i = 0; i < newNodeLevel; ++i) {
            newNodePtr->forward[i] = preds[i]->forward[i];
            preds[i]->forward[i] = newNodePtr;
        }
    }

    void erase (const T& eraseValue) {
        std::vector<Node*> preds = predecessors(eraseValue);
        Node* node = preds[0]->forward[0];

        while (node != nullptr && node->value == eraseValue) {
            for (int i = 0; i < nodeLevel(node); ++i)
                preds[i]->forward[i] = node->forward[i];

            delete node;
            node = preds[0]->forward[0];
        }
    }
};


int main() {
    SkipList<int> s;

    int input;
    std::cin >> input;
    while (input != 0) {
        s.insert(input);
        std::cin >> input;
    }

    int erase;
    std::cin >> erase;
    while (erase != 0) {
        s.erase (erase);
        std::cin >> erase;
    }

    s.print();

    return 0;
}

