//
// Created by 邓鑫林 on 2021/1/9.
//

#ifndef ALGORITHM_TREE_H
#define ALGORITHM_TREE_H

#include <memory>
#include <algorithm>
#include <vector>

using namespace std;

template<typename Key, typename Value>
class rb_tree {
    static const bool RED = true;
    static const bool BLACK = false;
    struct Node {
        Key key;
        Value value;
        Node *left;
        Node *right;
        int size;
        bool color;
    };
    Node *root = nullptr;
    Node *cache = nullptr;
    Key nullKey;
    Value nullValue;
public:
    rb_tree(Key key, Value value) : nullKey(key), nullValue(value) {}

    rb_tree(const rb_tree &rhs) {
        root = rhs.root;
        nullKey = rhs.nullKey;
        nullValue = rhs.nullValue;
    }

    rb_tree(rb_tree &&rhs) noexcept {
        root = rhs.root;
        cache = rhs.cache;
        nullKey = rhs.nullKey;
        nullValue = rhs.nullValue;
        rhs.root = nullptr;
        rhs.cache = nullptr;
    }

    rb_tree &operator=(const rb_tree &rhs) {
        if (*this != rhs) {
            rb_tree tmp(rhs);
            destructor(root);
            root = tmp.root;
            cache = nullptr;
        }
        return *this;
    }

    rb_tree &operator=(rb_tree &&rhs) noexcept {
        if (this != &rhs) {
            destructor(root);
            root = rhs.root;
            cache = rhs.cache;
            rhs.root = nullptr;
            rhs.cache = nullptr;
        }
        return *this;
    }

    ~rb_tree() {
        destructor(root);
    }

private:
    bool isRed(Node *x) {
        if (x == nullptr) return BLACK;
        return x->color == RED;
    }

    int size(Node *x) {
        return x == nullptr ? 0 : x->size;
    }

    Node *rotateLeft(Node *x) {
        Node *h = x->right;
        x->right = h->left;
        h->left = x;
        h->color = x->color;
        x->color = RED;
        h->size = x->size;
        x->size = size(x->right) + size(x->left) + 1;
        return h;
    }

    Node *rotateRight(Node *x) {
        Node *h = x->left;
        x->left = h->right;
        h->right = x;
        h->color = x->color;
        x->color = RED;
        h->size = x->size;
        x->size = size(x->right) + size(x->left) + 1;
        return h;
    }

    void flipColors(Node *x) {
        x->color = !x->color;
        x->left->color = !x->left->color;
        x->right->color = !x->right->color;
    }

    Value get(Node *x, const Key &key) {
        if (x == nullptr) return nullValue;
        if (x->key < key)
            return get(x->right, key);
        else if (x->key > key)
            return get(x->left, key);
        else {
            cache = x;
            return x->value;
        }
    }

    Node *put(Node *x, const Key &key, const Value &value) { // 2-3树
        if (x == nullptr)return createNode(key, value, 1, RED);
//        if (isRed(x->right) && isRed(x->left))
//            flipColors(x);
        if (x->key < key) x->right = put(x->right, key, value);
        else if (x->key > key) x->left = put(x->left, key, value);
        else x->value = value;
        if (isRED(x->right) && !isRED(x->left))
            x = rotateLeft(x);
        if (isRed(x->left) && isRed(x->left->left))
            x = rotateRight(x);
        if (isRed(x->right) && isRed(x->left))
            flipColors(x);
        x->size = 1 + size(x->left) + size(x->left);
        return x;
    }

    Node *deleteMin(Node *h) {
        if (h->left == nullptr) {
            deleteNode(h);
            return nullptr;
        }

        if (!isRed(h->left) && !isRed(h->left->left))
            h = moveRedLeft(h);

        h->left = deleteMin(h->left);
        return balance(h);
    }

    Node *moveRedLeft(Node *h) {
        flipColors(h);

        if (isRed(h->right->left)) {
            h->right = rotateRight(h->right);
            h = rotateLeft(h);
            flipColors(h);
        }

        return h;
    }

    Node *balance(Node *h) {
        if (isRed(h->right)) h = rotateLeft(h);
        if (isRed(h->left) && isRed(h->left->left)) h = rotateRight(h);
        if (isRed(h->left) && isRed(h->right)) flipColors(h);

        h->size = size(h->left) + size(h->right) + 1;
        return h;
    }

    Node *deleteMax(Node *h) {
        if (isRed(h->left))
            h = rotateRight(h);

        if (h->right == nullptr) {
            deleteNode(h);
            return nullptr;
        }
        if (!isRed(h->right) && !isRed(h->right->left))
            h = moveRedRight(h);

        h->right = deleteMax(h->right);
        return balance(h);
    }

    Node *moveRedRight(Node *h) {
        flipColors(h);

        if (isRed(h->left->left)) {
            h = rotateRight(h);
            flipColors(h);
        }
        return h;
    }

    Node *deleteKey(Node *h, Key key) {
        if (key < h->key) {
            if (!isRed(h->left) && !isRed(h->left->left))
                h = moveRedLeft(h);
            h->left = deleteKey(h->left, key);
        } else {
            if (isRed(h->left))
                h = rotateRight(h);
            if (key == h->key && h->right == nullptr) {
                deleteNode(h);
                return nullptr;
            }

            if (!isRed(h->right) && !isRed(h->right->left))
                h = moveRedRight(h);

            if (key == h->key) {
                Node *x = min(h->right);
                h->key = x->key;
                h->value = x->value;
                h->right = deleteMin(h->right);
            } else h->right = deleteKey(h->right, key);
        }

        return balance(h);
    }

public:
    bool isEmpty(){return root == nullptr;}
    int size(){return root->size;}
    bool contains(const Key&key){return get(key)!=nullValue; }
    Value get(const Key&key){
        if (cache->key == key)
            return cache->value;
        return get(root,key);
    }
    void put(const Key&key,const Value&value){
      root =  put(root,key,value);
      root->color = BLACK;
    }

    void deleteMin()
    {
        if (!isRed(root->left) && !isRed(root->right)) root->color = RED;//保证当前结点不是2-结点

        root = deleteMin(root);
        if (!isEmpty()) root->color = BLACK;
    }

    void deleteMax()
    {
        if (!isRed(root->left) && !isRed(root->right)) root->color = RED;
        root = deleteMax(root);
        if (!isEmpty()) root->color = BLACK;
    }

    void deleteKey(Key key)
    {
        if (!isRed(root->left) && !isRed(root->right)) root->color = RED;
        root = deleteKey(root,key);
        if (!isEmpty()) root->color = BLACK;
    }
    Key min() { return min(root)->key; }

    Key max() { return max(root)->key; }

    Key floor(Key key)
    {
        Node* x = floor(root, key);
        if (x == nullptr) return nullKey;
        return x->key;
    }

    Key ceiling(Key key)
    {
        Node *x = ceiling(root, key);
        if (x == nullptr) return nullKey;
        return x->key;
    }
    Key select(int k)
    {
        if (cache && size(cache->left) == k) return cache->key;
        return select(root, k)->key;
    }

    int rank(Key key)
    {
        if (cache && cache->key == key) return size(cache->left);
        return rank(root, key);
    }
    struct Item
    {
        Key key;
        Value value;
    };

    vector<Item> toArray() { return toArray(min(), max()); }

    vector<Item> toArray(const Key lo, const Key hi)
    {
        vector<Item> ret;
        toArray(root, ret, lo, hi);
        return ret;
    }


private:

    Node* select(Node *x, int k)
    {
        if (x == nullptr) return nullptr;
        int t = size(x->left);
        if (t > k) return select(x->left, k);
        else if (t < k) return select(x->right, k - t - 1);
        else
        {
            cache = x;
            return x;
        }
    }

    int rank(Node *x, Key key)
    {
        if (x == nullptr) return 0;
        if (key < x->key) return rank(x->left, key);
        else if (x->key < key) return 1 + size(x->left) + rank(x->right, key);
        else return size(x->left);
    }
    Node* min(Node* x)
    {
        if (x->left == nullptr) return x;
        else return min(x->left);
    }

    Node* max(Node* x)
    {
        if (x->right == nullptr) return x;
        else return max(x->right);
    }

    Node* floor(Node* x, Key key)
    {
        if (x == nullptr) return nullptr;
        if (key < x->key) return floor(x->left);
        else if (x->key == key) return x;

        Node *t = floor(x->right, key);
        if (t != nullptr) return t;
        else return x;
    }

    Node* ceiling(Node *x, Key key)
    {
        if (x == nullptr) return nullptr;
        if (x->key == key) return x;
        else if (x->key < key) return ceiling(x->right, key);

        Node *t = ceiling(x->left, key);
        if (t == nullptr) return x;
        else return t;
    }

    void toArray(Node *x, vector<Item> &q, const Key lo, const Key hi)
    {
        if (x == nullptr)return;
        if (x->key > lo) toArray(x->left, q, lo, hi);
        if (x->key >= lo && x->key <= hi) q.push_back({ x->key,x->value });
        if (x->key < hi) toArray(x->right, q, lo, hi);

    }

private:
    void destructor(Node *x) {
        if (x == nullptr)
            return;
        destructor(x->left);
        destructor(x->rigt);
        deleteNode(x);
    }

    Node *constructor(Node *x) {
        if (x == nullptr) return nullptr;
        auto left = constructor(x->left);
        auto right = constructor(x->right);
        auto ret = createNode(x->key, x->value, x->size, x->color);
        ret->right = right;
        ret->left = left;
        return ret;
    }

    static std::allocator<Node> alloc;

    Node *createNode(const Key &key, const Value &value, const int n = 1, bool col = RED) {
        auto p = alloc.allocate(1);
        alloc.construct(&(p->key), key);
        alloc.construct(&(p->value), value);
        alloc.construct(&(p->left), nullptr);
        alloc.construct(&(p->right), nullptr);
        alloc.construct(&(p->color), col);
        alloc.construct(&(p->size), n);
        return p;
    }

    Node *createNode(Key &&key, Value &&val, const int n, bool col) {
        auto p = alloc.allocate(1);
        alloc.construct(&(p->key), std::move(key));
        alloc.construct(&(p->value), std::move(val));
        alloc.construct(&(p->left), nullptr);
        alloc.construct(&(p->right), nullptr);
        alloc.construct(&(p->color), col);
        alloc.construct(&(p->size), n);
        return p;
    }

    void deleteNode(Node *node) {
        alloc.destroy(&(node->key));
        alloc.destroy(&(node->value));
        alloc.deallocate(node, 1);
    }

};


#endif //ALGORITHM_TREE_H
