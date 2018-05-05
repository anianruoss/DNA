#include <cassert>
#include <iostream>


enum Dir { dir_left, dir_right };

template <class T>
class AvlNode {
  private:
    AvlNode<T> *left = nullptr;
    AvlNode<T> *right = nullptr;
    AvlNode **this_ptr;


  public:
    int balance = 0;
    T value;

    AvlNode<T> (AvlNode<T>** this_p, T val) : this_ptr(this_p), value(val) {}

    void rotateRight() {
        assert (balance == -1);

        AvlNode** old_this_ptr = this->this_ptr;
        AvlNode* x = left;
        AvlNode* y = this;

        y->left = x->right;
        if (x->right)
            x->right->this_ptr = &y->left;
        y->balance = 0;

        x->right = y;
        y->this_ptr = &x->right;
        x->balance = 0;

        *old_this_ptr = x;
        x->this_ptr = old_this_ptr;
    }

    void rotateLeftRight() {
        assert (balance == -1);

        AvlNode** old_this_ptr = this->this_ptr;
        AvlNode* z = this;
        AvlNode* x = left;

        assert (x != nullptr);
        assert (x->balance == 1);

        AvlNode* y = left->right;

        assert (y != nullptr);

        AvlNode* t2 = y->left;
        AvlNode* t3 = y->right;

        z->left = t3;
        if (t3 != nullptr)
            t3->this_ptr = &z->left;
        if (y->balance == 1 || y->balance == 0)
            z->balance = 0;
        else
            z->balance = 1;

        x->right = t2;
        if (t2 != nullptr)
            t2->this_ptr = &x->right;
        if (y->balance == -1 || y->balance == 0)
            x->balance = 0;
        else
            x->balance = -1;

        y->left = x;
        x->this_ptr = &y->left;
        y->right = z;
        z->this_ptr = &y->right;
        y->balance = 0;

        *old_this_ptr = y;
        y->this_ptr = old_this_ptr;
    }

    void rotateLeft() {
        AvlNode** old_this_ptr = this->this_ptr;
        AvlNode* y = this;
        AvlNode* x = right;

        assert (y->balance > -1);
        assert (x != nullptr);
        assert (x->balance > -1);

        y->right = x->left;
        if (x->left != nullptr)
            x->left->this_ptr = &y->right;
        y->balance--;

        x->left = y;
        y->this_ptr = &x->left;
        x->balance--;

        *old_this_ptr = x;
        x->this_ptr = old_this_ptr;
    }

    void rotateRightLeft() {
        assert (balance == 1);

        AvlNode** old_this_ptr = this->this_ptr;
        AvlNode* z = this;
        AvlNode* x = right;

        assert (x != nullptr);
        assert (x->balance == -1);

        AvlNode* y = right->left;

        assert (y != nullptr);

        AvlNode* t2 = y->left;
        AvlNode* t3 = y->right;

        z->right = t2;
        if (t2 != nullptr)
            t2->this_ptr = &z->right;
        if (y->balance == 0 || y->balance == -1)
            z->balance = 0;
        else
            z->balance = -1;

        x->left = t3;
        if (t3 != nullptr)
            t3->this_ptr = &x->left;
        if (y->balance == 0 || y->balance == 1)
            x->balance = 0;
        else
            x->balance = 1;

        y->left = z;
        z->this_ptr = &y->left;
        y->right = x;
        x->this_ptr = &y->right;
        y->balance = 0;

        *old_this_ptr = y;
        y->this_ptr = old_this_ptr;
    }

    bool upin(Dir dir) {
        AvlNode * p = dir==dir_left ? this->left : this->right;

        assert (p->balance != 0);

        if (dir == dir_left) {
            switch (balance) {
            case 1:
                balance = 0;
                return false;
            case 0:
                balance = -1;
                return true;
            case -1:
                if (p->balance == -1) {
                    rotateRight();
                } else {
                    rotateLeftRight();
                }
                return false;
            default:
                assert(false);
            }
        } else {
            switch (balance) {
            case -1:
                balance = 0;
                return false;
            case 0:
                balance = 1;
                return true;
            case 1:
                if (p->balance == 1) {
                    rotateLeft();
                } else {
                    rotateRightLeft();
                }
                return false;
            default:
                assert(false);
            }
        }

        return false;
    }

    bool insert(T x) {
        if (value > x) {
            if (left == nullptr) {
                left = new AvlNode(&this->left, x);

                assert (balance != -1);

                if (balance == 0) {
                    balance = -1;
                    return true;
                } else if (balance == 1) {
                    balance = 0;
                    return false;
                }
            } else {
                if (left->insert(x))
                    return upin(dir_left);
            }
        } else {
            if (right == nullptr) {
                right = new AvlNode(&this->right, x);

                assert (balance != 1);

                if (balance == 0) {
                    balance = 1;
                    return true;
                } else if (balance == -1) {
                    balance = 0;
                    return false;
                }
            } else {
                if (right->insert(x))
                    return upin(dir_right);
            }
        }

        return false;
    }

    int height() {
        int h = 0;
        if (right != nullptr)
            h = std::max(h, right->height());
        if (left != nullptr)
            h = std::max(h, left->height());

        return h + 1;
    }

    void print() {
        std::cout << value << " ";
        if (left != nullptr)
            left->print();
        if (right != nullptr)
            right->print();
    }

    bool isAvl() {
        if (balance < -1 && 1 < balance)
            return false;

        bool lefts = true;
        if (left)
            lefts = left->this_ptr == &left;

        bool rights = true;
        if (right)
            rights = right->this_ptr == &right;

        int lheight = 0;
        bool lavl = true;
        if (left != nullptr) {
            lheight = left->height();
            lavl = left->isAvl();
        }

        int rheight = 0;
        bool ravl = true;
        if (right != nullptr) {
            rheight = right->height();
            ravl = right->isAvl();
        }

        if (!(balance == rheight - lheight && lavl && ravl && rights && lefts)) {
            std::cout << "Assertion fault at node value: " << this->value <<
                      " balance: " << this->balance << std::endl;
            print();
            std::cout << std::endl;
        }

        assert (balance == rheight - lheight);
        assert (lavl);
        assert (ravl);
        assert (rights);
        assert (lefts);

        return balance == rheight - lheight && lavl && ravl && rights && lefts;
    }
};


template<class T>
class AvlTree {
  public:
    AvlNode<T> * root = nullptr;
    AvlTree() {};

    void insert(T x) {
        if (root == nullptr) {
            root = new AvlNode<T> (&root, x);
        } else
            root->insert(x);

        assertAvl();
    }

    int height() {
        if (root != nullptr)
            return root->height();

        return 0;
    }

    void print() {
        if (root != nullptr)
            root->print();

        std::cout << std::endl;
    }

    void assertAvl() {
        if (root) {
            assert (root->isAvl());
        }
    }
};


int main() {
    AvlTree<int> tree;

    int num;
    std::cin >> num;
    while (num-- > 0) {
        int val;
        std::cin >> val;
        tree.insert(val);
    }

    tree.print();
    return 0;
}

