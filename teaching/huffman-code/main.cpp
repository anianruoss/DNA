#include <cassert>
#include <iostream>
#include <fstream>
#include <map>
#include <memory>
#include <queue>
#include <random>
#include <sstream>
#include <vector>


struct Node;

using SharedNode=std::shared_ptr<Node>;

struct Node {
    char value;
    int frequency;
    SharedNode left;
    SharedNode right;

    Node(char v, int f)
        : value{v}, frequency{f}, left{nullptr}, right{nullptr} {}

    Node(SharedNode l, SharedNode r)
        : value{0}, frequency{l->frequency+r->frequency}, left{l}, right{r} {}
};

struct comparator {
    bool operator()(const SharedNode a, const SharedNode b) const {
        return a->frequency > b->frequency;
    }
};

int length(SharedNode node, int depth) {
    if (node != nullptr) {
        if (node->value != 0) {
            return depth * node->frequency;
        }

        return length(node->left, depth+1) + length(node->right, depth+1);
    }

    return 0;
}

void encode(std::istream& in) {
    // read in characters and get frequencies
    std::map<char, int> m;
    char x;
    int n = 0;

    while (in.get(x)) {
        ++m[x];
        ++n;
    }

    std::cout << "n = " << n << " characters" << std::endl;

    // build head
    std::priority_queue<SharedNode, std::vector<SharedNode>, comparator> q;

    for (auto y: m)
        q.push(std::make_shared<Node>(y.first, y.second));

    // build code tree
    SharedNode left;

    while (!q.empty()) {
        left = q.top();
        q.pop();

        if (!q.empty()) {
            auto right = q.top();
            q.pop();
            q.push(std::make_shared<Node>(left, right));
        }
    }

    // output length of the code, i.e. the summed number of bits times the
    std::cout << "length = " << length(left,0) << " bits" << std::endl;
}

void encodeString(std::string str) {
    std::stringstream s;
    s << str;
    encode(s);
}

void encodeFile(std::string filename) {
    std::ifstream input(filename);
    encode(input);
}

// main function, use this unaltered function for your final submission
int main() {

    std::string command;
    std::cin >> command;

    if (command == "file") {
        std::string filename;
        std::cin >> filename;
        encodeFile(filename);
    } else {
        char och;
        std::cin >> och;
        assert(och == '"' || och == '\'');
        char ch;
        std::string str;

        while (std::cin.get(ch) && (ch != och)) {
            str += ch;
        }

        encodeString(str);
    }

    return 0;
}

