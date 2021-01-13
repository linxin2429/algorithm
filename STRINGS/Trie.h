//
// Created by 邓鑫林 on 2021/1/12.
//

#ifndef ALGORITHM_TRIE_H
#define ALGORITHM_TRIE_H

#include <vector>
#include <iostream>
#include <string>
#include <memory>

using namespace std;

template<typename Value>
class Trie { //发音 ：try
    static const int R = 256;
    Value nullValue;

    struct Node {
        Value value_;
        int size_;
        vector<Node *> next_;

        Node(const Value &value, int n) : value_(Value()), size_(n), next_(R, nullptr) {}
    };

    static std::allocator<Node> alloc;

    Node *createNode(const Value &val, const int n) {
        auto p = alloc.allocate(1);
        alloc.construct(p, val, n);
        return p;
    }

    Node *createNode(Value &&val, const int n) {
        auto p = alloc.allocate(1);
        alloc.construct(p, std::forward<Value>(val), n);
        return p;
    }

    void deleteNode(Node *node) {
        alloc.destroy(node);
        alloc.deallocate(node, 1);
    }

    void destructor(Node *x) {
        if (x == nullptr) return;
        for (int c = 0; c < R; ++c)
            destructor(x->next[c]);
        deleteNode(x);
    }

    Node *constructor(Node *x) {
        if (x == nullptr) return nullptr;

        auto ret = createNode(x->value, x->size);
        for (int c = 0; c < R; ++c)
            ret->next[c] = constructor(x->next[c]);
        return ret;
    }

    Node *root_;
public:
    explicit Trie(const Value &nullvalue = Value()) : root_(nullptr), nullValue(nullvalue) {}

    Trie(const Trie &rhs) {
        root_ = constructor(rhs.root_);
        nullValue = rhs.nullValue;
    }

    Trie(const Trie &&rhs) noexcept {
        root_ = rhs.root_;
        nullValue = rhs.nullValue;
        rhs.root_ = nullptr;
    }

    Trie &operator=(const Trie &rhs) {
        if (this != &rhs) {
            Trie tmp(rhs);
            destructor(root_);
            root_ = tmp.root_;
        }
        return *this;
    }

    Trie &operator=(Trie &&rhs) noexcept {
        if (this != &rhs) {
            destructor(root_);
            root_ = rhs.root_;
            rhs.root_ = nullptr;
        }
        return *this;
    }

    ~Trie() {
        destructor(root_);
    }

public:
    /*
     * 向表中插入键值对
     */
    void put(const string &key, const Value &value) {
        put(root_, key, value, 0);
    }

    /*
     * 键key所对应的值
     */
    Value get(const string &key) const {
        Node *x = get(root_, key, 0);
        if (x == nullptr) return nullValue;
        return x->value_;
    }

    /*
     * 删除键
     */
    void deleteKey(const string &key) {
        deleteKey(root_, key, 0);
    }

    /*
     * 是否存在key
     */
    bool contains(const string &key) {
        return get(key) != nullValue;
    }

    /*
     * 表是否为空
     */
    bool isEmpty() {
        return size() == 0;
    }

    /*
     * s的前缀中最长的键
     */
    string longestPrefixOf(const string &s) {
        return s.substr(0, search(root_, s, 0, 0));
    }

    /*
     * 所有以s为前缀的键
     */
    vector<string> keysWithPrefix(const string &s) {
        vector<string> v;
        collect(root_, s, v);
        return v;
    }

    /*
     * 所有和s匹配的键
     */
    vector<string> keysThatMatch(const string &s) {
        return collect(root_, "", s, vector<string>());
    }

    /*
     * 键值对数量
     */
    int size() {
        return size(root_);
    }

    /*
     * 返回符号表中所有的键
     */
    vector<string> keys() const {
        return keysWithPrefix("");
    }

private:
    Node *get(Node *x, const string &key, int d) {
        if (x == nullptr) return nullptr;
        if (d == key.size()) return x;
        char c = key[d];
        return get(x->next_[c], key, d + 1);
    }

    Node *put(Node *x, const string &key, const Value &value, int d) {
        if (x == nullptr) x = createNode(nullValue, 0);
        if (key.size() == d) {
            x->value_ = value;
            return x;
        }
        char c = key[d];
        x->next_[c] = put(x->next_[c], key, value, d + 1);

        if (x->value_ == nullValue)
            x->size_ = 0;
        else
            x->size_ = 1;
        for (int c = 0; c < R; ++c)
            x->size_ += size(x->next_[c]);
        return x;
    }

    int size(Node *x) {
        return x == nullptr ? 0 : x->size_;
    }

    void collect(Node *x, const string &pre, vector<string> &v) {
        if (x == nullptr) return;
        if (x->value_ != nullValue) v.push_back(pre);
        for (char c = 0; c < R; ++c)
            collect(x->next_[c], pre + c, v);
    }

    void collect(Node *x, const string &pre, const string &pat, vector<string> &v) {
        int d = pre.size();
        if (x == nullptr) return;
        if (d == pat.size() && x->value_ != nullValue) v.push_back(pre);
        if (d == pat.size()) return;
        char next = pat[d];
        for (char c = 0; c < R; ++c)
            if (next == '.' || next == c)
                collect(x->next_[c], pre + c, pat, v);
    }

    int search(Node *x, const string &s, int d, int length) {
        if (x == nullptr) return length;
        if (x->value_ != nullValue) length = d;
        if (d == s.size()) return length;
        char c = s[d];
        return search(x->next_[c], s, d + 1, length);
    }

    Node *deleteKey(Node *x, const string &key, int d) {
        if (x == nullptr) return nullptr;
        if (d == key.size()) {
            x->value_ = nullValue;
        } else {
            char c = key[d];
            x->next_[c] = deleteKey(x->next_[c], key, d + 1);
        }
        if (x->value_ == nullValue)
            x->size_ = 0;
        else
            x->size_ = 1;
        for (int c = 0; c < R; ++c) {
            x->size_ += size(x->next_[c]);
        }
        if (x->size_ == 0) {
            destructor(x);
            return nullptr;
        } else return x;
    }

};

template<typename Value>
class TST { // 三向单词查找树
    struct Node {
        char c;
        Node *left, *mid, *right;
        Value value;
        int size;

        Node(char c_, const Value &value_, int n) :
                c(c_), left(nullptr), right(nullptr), mid(nullptr), value(value_), size(n) {}
    };

    Node *root;
    Value nullValue;
public:
    explicit TST(Value nullval = Value()) : nullValue(nullval) {}

    TST(const TST &rhs) {
        root = constructor(rhs.root);
        nullValue = rhs.nullValue;
    }

    TST &operator=(const TST &rhs) {
        TST tmp(rhs);
        destructor(root);
        root = tmp.root;

        return *this;
    }

    TST(TST &&rhs) noexcept {
        root = rhs.root;
        nullValue = rhs.nullValue;
        rhs.root = nullptr;
    }

    TST &operator=(TST &&rhs) noexcept {
        if (this != &rhs) {
            destructor(root);
            root = rhs.root;
            rhs.root = nullptr;
        }
        return *this;
    }

    ~TST() {
        destructor(root);
    }

private:
    void destructor(Node *x) {
        if (x == nullptr) return;
        destructor(x->left);
        destructor(x->right);
        deleteNode(x);
    }

    Node *constructor(Node *x) {
        if (x == nullptr) return nullptr;

        auto left = constructor(x->left);
        auto right = constructor(x->right);
        auto ret = createNode(x->c, x->value, x->size);
        ret->left = left;
        ret->right = right;
        return ret;
    }

    static std::allocator<Node> alloc;

    Node *createNode(char c, const Value &val, const int n) {
        auto p = alloc.allocate(1);
        alloc.construct(p, c, val, n);
        return p;
    }

    Node *createNode(char c, Value &&val, const int n) {
        auto p = alloc.allocate(1);
        alloc.construct(p, std::forward<Value>(val), n);
        return p;
    }

    void deleteNode(Node *node) {
        alloc.destroy(node);
        alloc.deallocate(node, 1);
    }

public:
    Value get(const string &key) {
        Node * x =get(root,key,0) ;
        if (x == nullptr)
            return nullValue;
        return x->value;
    }

    void put(const string &key, const Value &value) {
        root = put(root,key,value,0);
    }

private:
    Node* get(Node *x, const string &key, int d) {
        if (x == nullptr) return nullptr;
        char c =key[d];
        if (c < x->c) return get(x->left,key,d);
        else if (c > x->c) return get(x->right,key,d);
        else if (d < key.size() - 1) return get(x->mid, key,d+1);
        else return x;
    }

    Node *put(Node *x,const string &key,const Value &value,int d){
        char c = key[d];
        if (x == nullptr) {x = createNode(c,nullValue,0);}
        if (c < x->c) x->left = put(x->left,key,value,d);
        else if (c > x->c) x->right = put(x->right,key,value,d);
        else if (d < key.size() - 1) x->mid = put(x->mid,key,value,d+1);
        else x->value = value;
        return x;
    }

};


#endif //ALGORITHM_TRIE_H
