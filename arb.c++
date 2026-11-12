#include <iostream>
#include <chrono>
#include <vector>
#include <random>
#include <algorithm>

using namespace std;

enum Color { RED, BLACK };

struct Node {
    int data;
    Color color;
    Node *left, *right, *parent;

    Node(int data) : data(data), color(RED), 
                     left(nullptr), right(nullptr), parent(nullptr) {}
};

class RedBlackTree {
private:
    Node* root;
    Node* NIL;

    void leftRotate(Node* x) {
        rotationsCount++;
        Node* y = x->right;
        x->right = y->left;

        if (y->left != NIL) y->left->parent = x;
        y->parent = x->parent;

        if (x->parent == nullptr)      root = y;
        else if (x == x->parent->left) x->parent->left = y;
        else                          x->parent->right = y;

        y->left = x;
        x->parent = y;
    }

    void rightRotate(Node* x) {
        rotationsCount++;
        Node* y = x->left;
        x->left = y->right;

        if (y->right != NIL) y->right->parent = x;
        y->parent = x->parent;

        if (x->parent == nullptr)       root = y;
        else if (x == x->parent->right) x->parent->right = y;
        else                           x->parent->left = y;

        y->right = x;
        x->parent = y;
    }

    void fixInsert(Node* k) {
        while (k != root && k->parent->color == RED) {
            if (k->parent == k->parent->parent->left) {
                Node* uncle = k->parent->parent->right;

                if (uncle->color == RED) {            
                    k->parent->color = BLACK;
                    uncle->color = BLACK;
                    k->parent->parent->color = RED;
                    recolorationsCount += 3;
                    k = k->parent->parent;
                } else {
                    if (k == k->parent->right) {       
                        k = k->parent;
                        leftRotate(k);
                    }
                    k->parent->color = BLACK;        
                    k->parent->parent->color = RED;
                    recolorationsCount += 2;
                    rightRotate(k->parent->parent);
                }
            } else {                                   
                Node* uncle = k->parent->parent->left;

                if (uncle->color == RED) {
                    k->parent->color = BLACK;
                    uncle->color = BLACK;
                    k->parent->parent->color = RED;
                    recolorationsCount += 3;
                    k = k->parent->parent;
                } else {
                    if (k == k->parent->left) {
                        k = k->parent;
                        rightRotate(k);
                    }
                    k->parent->color = BLACK;
                    k->parent->parent->color = RED;
                    recolorationsCount += 2;
                    leftRotate(k->parent->parent);
                }
            }
        }
        if (root->color != BLACK) {
            root->color = BLACK;
            recolorationsCount++;
        }
    }

    void rbTransplant(Node* u, Node* v) {
        if (u->parent == nullptr) {
            root = v;
        } else if (u == u->parent->left) {
            u->parent->left = v;
        } else {
            u->parent->right = v;
        }
        v->parent = u->parent;
    }

    Node* minimum(Node* node) {
        while (node->left != NIL) {
            node = node->left;
        }
        return node;
    }

    void fixDelete(Node* x) {
        while (x != root && x->color == BLACK) {
            if (x == x->parent->left) {
                Node* w = x->parent->right;

                if (w->color == RED) {
                    w->color = BLACK;
                    x->parent->color = RED;
                    recolorationsCount += 2;
                    leftRotate(x->parent);
                    w = x->parent->right;
                }

                if (w->left->color == BLACK && w->right->color == BLACK) {
                    w->color = RED;
                    recolorationsCount++;
                    x = x->parent;
                } else {
                    if (w->right->color == BLACK) {
                        w->left->color = BLACK;
                        w->color = RED;
                        recolorationsCount += 2;
                        rightRotate(w);
                        w = x->parent->right;
                    }
                    w->color = x->parent->color;
                    x->parent->color = BLACK;
                    w->right->color = BLACK;
                    recolorationsCount += 3;
                    leftRotate(x->parent);
                    x = root;
                }
            } else {
                Node* w = x->parent->left;

                if (w->color == RED) {
                    w->color = BLACK;
                    x->parent->color = RED;
                    recolorationsCount += 2;
                    rightRotate(x->parent);
                    w = x->parent->left;
                }

                if (w->right->color == BLACK && w->left->color == BLACK) {
                    w->color = RED;
                    recolorationsCount++;
                    x = x->parent;
                } else {
                    if (w->left->color == BLACK) {
                        w->right->color = BLACK;
                        w->color = RED;
                        recolorationsCount += 2;
                        leftRotate(w);
                        w = x->parent->left;
                    }
                    w->color = x->parent->color;
                    x->parent->color = BLACK;
                    w->left->color = BLACK;
                    recolorationsCount += 3;
                    rightRotate(x->parent);
                    x = root;
                }
            }
        }
        if (x->color != BLACK) {
            x->color = BLACK;
            recolorationsCount++;
        }
    }

    void destroy(Node* node) {
        if (node != NIL && node != nullptr) {
            destroy(node->left);
            destroy(node->right);
            delete node;
        }
    }

    Node* findNode(int data) const {
        Node* current = root;
        while (current != NIL && current != nullptr) {
            if (data == current->data) return current;
            if (data < current->data) current = current->left;
            else current = current->right;
        }
        return nullptr;
    }

public:
    long long rotationsCount = 0;
    long long recolorationsCount = 0;

    void resetCounters() {
        rotationsCount = 0;
        recolorationsCount = 0;
    }

    RedBlackTree() {
        NIL = new Node(0);
        NIL->color = BLACK;
        NIL->left = NIL->right = NIL;
        root = NIL;
    }

    ~RedBlackTree() {
        vider();
        delete NIL;
    }

    RedBlackTree(const RedBlackTree&) = delete;
    RedBlackTree& operator=(const RedBlackTree&) = delete;

    void vider() {
        destroy(root);
        root = NIL;
    }

    void insert(int data) {
        Node* node = new Node(data);
        node->left = node->right = NIL;

        Node* parent = nullptr;
        Node* current = root;

        while (current != NIL) {
            parent = current;
            if (node->data < current->data) current = current->left;
            else current = current->right;
        }

        node->parent = parent;

        if (parent == nullptr) {
            root = node;
            node->color = BLACK;
            recolorationsCount++;
            return;
        } else if (node->data < parent->data) {
            parent->left = node;
        } else {
            parent->right = node;
        }

        if (node->parent->parent == nullptr) return;

        fixInsert(node);
    }

    bool supprimer(int data) {
        Node* z = findNode(data);
        if (z == nullptr) return false;

        Node* y = z;
        Color y_original_color = y->color;
        Node* x;

        if (z->left == NIL) {
            x = z->right;
            rbTransplant(z, z->right);
        } else if (z->right == NIL) {
            x = z->left;
            rbTransplant(z, z->left);
        } else {
            y = minimum(z->right);
            y_original_color = y->color;
            x = y->right;

            if (y->parent == z) {
                x->parent = y;
            } else {
                rbTransplant(y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }

            rbTransplant(z, y);
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;
            recolorationsCount++;
        }

        delete z;

        if (y_original_color == BLACK) {
            fixDelete(x);
        }

        return true;
    }

    bool cherche(int data) const {
        return findNode(data) != nullptr;
    }

    Node* getRoot() const { return root; }
};

void runBenchmark(int N, const string& label, const vector<int>& dataset) {
    RedBlackTree rbt;

    auto t_start_insert = chrono::high_resolution_clock::now();
    for (int x : dataset) rbt.insert(x);
    auto t_end_insert = chrono::high_resolution_clock::now();

    int testSamples = min(N, 1000);
    auto t_start_search = chrono::high_resolution_clock::now();
    for (int i = 0; i < testSamples; ++i) rbt.cherche(dataset[i]);
    auto t_end_search = chrono::high_resolution_clock::now();

    auto t_start_delete = chrono::high_resolution_clock::now();
    for (int i = 0; i < testSamples; ++i) rbt.supprimer(dataset[i]);
    auto t_end_delete = chrono::high_resolution_clock::now();

    auto t_start_vider = chrono::high_resolution_clock::now();
    rbt.vider();
    auto t_end_vider = chrono::high_resolution_clock::now();

    double insert_ms = chrono::duration_cast<chrono::nanoseconds>(t_end_insert - t_start_insert).count() / 1e6;
    double search_ns = (double)chrono::duration_cast<chrono::nanoseconds>(t_end_search - t_start_search).count() / testSamples;
    double delete_ms = chrono::duration_cast<chrono::nanoseconds>(t_end_delete - t_start_delete).count() / 1e6;
    double vider_ms = chrono::duration_cast<chrono::nanoseconds>(t_end_vider - t_start_vider).count() / 1e6;

    cout << "N = " << N << " | " << label << endl;
    cout << "  Insert time: " << insert_ms << " ms" << endl;
    cout << "  Rotations: " << rbt.rotationsCount << endl;
    cout << "  Recolorations: " << rbt.recolorationsCount << endl;
    cout << "  Search latency: " << search_ns << " ns/item" << endl;
    cout << "  Delete time (" << testSamples << " items): " << delete_ms << " ms" << endl;
    cout << "  Vider time: " << vider_ms << " ms" << endl << endl;
}

int main() {
    vector<int> sizes = {1000, 100000, 1000000};
    mt19937 rng(42);

    for (int N : sizes) {
        vector<int> sortedData(N);
        for (int i = 0; i < N; ++i) sortedData[i] = i;

        vector<int> almostSortedData = sortedData;
        int swaps = max(1, (int)(N * 0.05));
        for (int i = 0; i < swaps; ++i) {
            int a = rng() % N;
            int b = rng() % N;
            swap(almostSortedData[a], almostSortedData[b]);
        }

        vector<int> randomData = sortedData;
        shuffle(randomData.begin(), randomData.end(), rng);

        runBenchmark(N, "Sorted", sortedData);
        runBenchmark(N, "Almost Sorted", almostSortedData);
        runBenchmark(N, "Random", randomData);
        cout << "--------------------------------------------------------" << endl;
    }

    return 0;
}
