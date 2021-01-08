//
// Created by 邓鑫林 on 2021/1/6.
//

#ifndef ALGORITHM_ST_H
#define ALGORITHM_ST_H

#include <utility>

/*
 * Symbol Table
 * 包含符号表（无）、二叉查找树、平衡查找树、散列表
 */

/*
 * 二叉搜索树结点
 */
template<typename Key, typename Value>
struct TreeNode {
    pair<Key, Value> item;
    TreeNode *left;
    TreeNode *right;
    TreeNode *parent;
    /*
     * 以该节点为根的子树的节点总数
     */
    int size;
public:
    TreeNode() : left{nullptr}, right{nullptr}, parent{nullptr}, size(1) {}

    TreeNode(const Key &key, const Value &value, int n = 1)
            : item{key, value}, left{nullptr}, right{nullptr}, parent{nullptr}, size(n) {}

    TreeNode(Key &&key, Value &&value, int n = 1)
            : item{std::move(key), std::move(value)},
              left{nullptr}, right{nullptr}, parent{nullptr}, size(n) {}

    TreeNode *clone() {
        auto ret = new TreeNode(item.first, item.second, size);
        ret->right = this->right;
        ret->left = this->left;
        return ret;
    }

};

/*
 * 二叉搜索树结点
 */
template<typename Key, typename Value>
struct RBTreeNode {
    pair<Key, Value> item;
    RBTreeNode *left;
    RBTreeNode *right;
    RBTreeNode *parent;
    /*
     * 以该节点为根的子树的节点总数
     */
    int size;
    static const bool RED = true;
    static const bool BLACK = false;
    bool color;

public:
    RBTreeNode() : left{nullptr}, right{nullptr}, parent{nullptr}, size{1} {}

    RBTreeNode(const Key &key, const Value &value, const int n = 1, bool col = RED)
            : item{key, value}, left{nullptr}, right{nullptr}, parent{nullptr}, size{n}, color{col} {}

    RBTreeNode(Key &&key, Value &&value, const int n = 1, bool col = RED)
            : item{std::move(key), std::move(value)},
              left{nullptr}, right{nullptr}, parent{nullptr}, size{n}, color{col} {}

    RBTreeNode *clone() {
        auto ret = new RBTreeNode(item.first, item.second, size, color);
        ret->right = this->right;
        ret->left = this->left;
        return ret;
    }
};

template<typename TreeNode>
struct TreeIterator {
    TreeNode *node;
    using value_type = typename std::remove_reference<decltype(node->item)>::type;

    TreeIterator(TreeNode *s) : node(s) {}

    bool operator==(const TreeIterator &rhs) { return node == rhs.node; }

    bool operator!=(const TreeIterator &rhs) { return !(*this == rhs); }

    value_type &operator*() { return node->item; }

    value_type *operator->() { return &(operator*()); }

    TreeIterator &operator++() {
        increase();
        return *this;
    }

    TreeIterator &operator++(int) {
        auto ret = *this;
        increase();
        return ret;
    }

    TreeIterator &operator--() {
        decrease();
        return *this;
    }

    TreeIterator &operator--(int) {
        auto ret = *this;
        decrease();
        return ret;
    }

private:
    void increase() {
        if (node->right != nullptr) {
            node = node->right;
            while (node->left != nullptr)
                node = node->left;
        } else {
            auto parent = node->parent;
            while (parent->right == node) {
                node = parent;
                parent = node->parent;
            }
            if (node->right != parent)
                node = parent;
        }
    }

    void decrease() {
        if (node->left != nullptr) {
            node = node->left;
            while (node->right != nullptr)
                node = node->right;
        } else {
            auto parent = node->parent;
            while (parent->left == node) {
                node = parent;
                parent = node->parent;
            }
            if (node->left != parent)
                node = parent;
        }
    }
};

template<typename Key, typename Value, typename Node = TreeNode<Key, Value>>
class BST {
    using Iterator = TreeIterator<Node>;
protected:
    Node *dummy;
public:
    BST();

    BST(const BST &rhs);

    BST(BST &&rhs) noexcept;

    ~BST();

    BST &operator=(const BST &rhs);

    BST &operator=(BST &&rhs) noexcept;

protected:
    void init() {
        dummy = new Node();
        dummy->parent = nullptr;//root
        dummy->right = dummy;//right most
        dummy->left = dummy;//left most
    }

    void destructor(Node *x) {
        if (x == nullptr)
            return;
        destructor(x->left);
        destructor(x->right);
        delete x;
        x = nullptr;
    }

    Node *constructor(Node *x) {
        if (x == nullptr)
            return nullptr;
        auto left = constructor(x->left);
        auto right = constructor(x->right);
        auto ret = x->clone();
        left->parent = ret;
        right->parent = ret;
        ret->right = right;
        ret->left = left;
        return ret;
    }

    Node *&root() { return dummy->parent; }

    int size(Node *x) {
        return x == nullptr ? 0 : x->size;
    }

    Node *select(Node *x, int k) {
        if (x == nullptr)return nullptr;
        int t = size(x->left);
        if (t < k) {
            return select(x->right, k - t - 1);
        } else if (t > k) {
            return select(x->left, k);
        } else
            return x;
    }

    int rank(Node *x, const Key &key) {
        if (x == nullptr)
            return 0;
        if (key < x->item.first)
            return rank(x->left, key);
        else if (key > x->item.first)
            return size(x->left) + 1 + rank(x->right, key);
        else
            return size(x->left);
    }

    Node *floor(Node *x, const Key &key) {
        if (x == nullptr)
            return nullptr;
        if (key < x->item.first)
            return floor(x->left, key);
        else if (key == x->item.first)
            return x;
        Node *t = floor(x->right, key);
        if (t != nullptr)
            return t;
        else return x;

    }

    Node *ceiling(Node *x, const Key &key) {
        if (x == nullptr)
            return nullptr;
        if (key == x->item.first)
            return x;
        else if (x->item.first < key)
            return ceiling(x->right, key);
        Node *t = ceiling(x->left, key);
        if (t != nullptr)
            return t;
        else return x;
    }

    Node *get(Node *x, const Key &key) {
        if (x == nullptr)
            return dummy;
        if (key < x->item.first)
            return get(x->left, key);
        else if (key > x->item.first)
            return get(x->right, key);
        else
            return x;
    }

    virtual Node *put(Node *&x, const Key &key, const Value &value) {
        if (x == nullptr)
            return new Node(key, value, 1);
        if (key < x->item.first) {
            x->left = put(x->left, key, value);
            x->left->parent = x;
        } else if (key > x->item.first) {
            x->right = put(x->right, key, value);
            x->right->parent = x;
        } else
            x->item.second = value;

        x->size = size(x->left) + size(x->right) + 1;
        return x;
    }

    Node *min(Node *x) {
        if (x == nullptr)
            return x;
        if (x->left == nullptr)
            return x;
        else
            return min(x->left);
    }

    Node *max(Node *x) {
        if (x == nullptr)
            return x;
        if (x->right == nullptr)
            return x;
        else
            return max(x->right);
    }

    virtual Node *deleteMin(Node *x) {
        if (x == nullptr)
            return nullptr;
        if (x->left == nullptr) {
            auto ret = x->right;
            delete x;
            return ret;
        }
        x->left = deleteMin(x->left);
        if (x->left != nullptr)
            x->left->parent = x;
        x->size = size(x->left) + size(x->right) + 1;
        return x;
    }

    virtual Node *deleteMax(Node *x) {
        if (x == nullptr)
            return nullptr;
        if (x->right == nullptr) {
            auto ret = x->left;
            delete x;
            return ret;
        }
        x->right = deleteMin(x->right);
        if (x->right != nullptr)
            x->right->parent = x;
        x->size = size(x->left) + size(x->right) + 1;
        return x;
    }

    virtual Node *deleteKey(Node *x, const Key &key) {
        if (x == nullptr)
            return x;
        if (key < x->item.first) {
            x->left = deleteKey(x->left, key);
            x->left->parent = x;
        } else if (key > x->item.first) {
            x->right = deleteKey(x->right, key);
            x->right->parent = x;
        } else {
            if (x->right == nullptr) {
                Node *ret = x->left;
                delete x;
                return ret;
            }
            if (x->left == nullptr) {
                Node *ret = x->right;
                delete x;
                x = nullptr;
                return ret;
            }
            Node *t = x;
            x = min(t->right)->clone();
            x->right = deleteMin(t->right);
            x->right->parent = x;
            x->left = t->left;
            x->left->parent = x;
            return x;
        }
        x->size = size(x->left) + size(x->right);
        return x;
    }

public:
    bool isEmpty() {
        return root() == nullptr;
    }

    bool contains(const Key &key) {
        return get(key) == end();
    }

    Iterator get(const Key &key) {
        return get(root(), key);
    }

    void put(const Key &key, const Value &value) {
        auto r = put(root(), key, value);
        r->parent = dummy;
        dummy->parent = r;
        dummy->right = max(r);
        dummy->left = min(r);
    }

    Iterator begin() {
        return min();
    }

    Iterator end() {
        return dummy;
    }

    virtual void deleteMin() {
        auto r = deleteMin(root());
        r->parent = dummy;
        dummy->parent = r;
        dummy->right = max(r);
        dummy->left = min(r);
    }

    virtual void deleteMax() {
        auto r = deleteMax(root());
        r->parent = dummy;
        dummy->parent = r;
        dummy->right = max(r);
        dummy->left = min(r);
    }

    virtual void deleteKey(const Key &key) {
        auto r = deleteKey(root(), key);
        r->parent = dummy;
        dummy->parent = r;
        dummy->right = max(r);
        dummy->left = min(r);
    }

    Iterator max() {
        return max(root());
    }

    Iterator min() {
        return min(root());
    }

    Iterator floor(const Key &key) {
        Node *x = floor(root(), key);
        if (x == nullptr)
            return end();
        return x;
    }

    Iterator ceiling(const Key &key) {
        Node *x = ceiling(root(), key);
        if (x == nullptr)
            return end();
        return x;
    }

    int size() { return size(root()); }

    Iterator select(int k) {
        Node *x = select(root(), k);
        if (x == nullptr)
            return end();
        return x;
    }

    int rank(const Key &key) {
        return rank(root(), key);
    }


};

template<typename Key, typename Value, typename Node>
BST<Key, Value, Node>::BST() {
    init();
}

template<typename Key, typename Value, typename Node>
BST<Key, Value, Node>::BST(const BST &rhs) {
    init();
    if (rhs.size() > 0) {
        auto x = constructor(rhs.root());
        x->parent = dummy;
        dummy->left = min(x);
        dummy->right = max(x);
        dummy->parent = x;
    }
}

template<typename Key, typename Value, typename Node>
BST<Key, Value, Node>::BST(BST &&rhs) noexcept {
    dummy = rhs.dummy;
    rhs.init();
}

template<typename Key, typename Value, typename Node>
BST<Key, Value, Node>::~BST() {
    destructor(root());
}

template<typename Key, typename Value, typename Node>
BST<Key, Value, Node> &BST<Key, Value, Node>::operator=(const BST &rhs) {
    if (*this != rhs) {
        BST tmp(rhs);
        *this = std::move(tmp);
    }
    return *this;
}

template<typename Key, typename Value, typename Node>
BST<Key, Value, Node> &BST<Key, Value, Node>::operator=(BST &&rhs) noexcept {
    if (*this != rhs) {
        destructor(root());
        dummy = rhs.dummy;
        rhs.init();
    }
    return *this;
}

template<typename Key, typename Value>
class RB {
private:
    RBTreeNode<Key, Value> root;

};

template<typename Key, typename Value, typename Node = RBTreeNode<Key, Value>>
class RBTree : public BST<Key, Value, Node> {
private:
    static const bool RED = true;
    static const bool BLACK = false;

    bool isRed(Node *x) {
        return x->color == RED;
    }

    Node *rotateLeft(Node *x) {
        Node *h = x->right;
        x->right = h->left;
        if (x->right != nullptr)
            x->right->parent = x;
        x->parent = h;
        h->left = x;
        h->color = x->color;
        x->color = RED;
        h->size = x->size;
        x->size = 1 + this->size(x->left) + this->size(x->right);
        return h;
    }

    Node *rotateRight(Node *x) {
        Node *h = x->left;
        x->left = h->right;
        if (x->left != nullptr)
            x->left->parent = x;
        x->parent = h;
        h->right = x;
        h->color = x->color;
        x->color = RED;
        h->size = x->size;
        x->size = 1 + this->size(x->left) + this->size(x->right);
        return h;

    }


    void flipColor(Node *x) {
        x->color = !x->color;
        x->left->color = !x->left->color;
        x->right->color = !x->right->color;

    }

    virtual Node *put(Node *x, const Key &key, const Value &value) {
        if (x == nullptr)
            return new Node(key, value);
        if (key < x->item.first) {
            x->left = put(x->left, key, value);
            x->left->parent = x;
        } else if (key > x->item.first) {
            x->right = put(x->right, key, value);
            x->right->parent = x;
        } else
            x->item.second = value;
        if (isRed(x->right) && !isRed(x->left))x = rotateLeft(x);
        if (isRed(x->left) && isRed(x->left->left)) x = rotateRight(x);
        if (isRed(x->right) && isRed(x->left)) flipColor(x);
        x->size = 1 + this->size(x->left) + this->size(x->right);
        return x;
    }

public:
    void put(const Key &key, const Value &value) {
        put(this->root(), key, value);
        BST<Key, Value, Node>::root()->color = BLACK;
    }

};

void testST() {
    RBTree<int, string> st;
    st.put(1, "wangyan");
    st.put(2, "zhangpengcheng");
    st.put(3, "wangshanshi");
    st.put(4, "april");

    std::cout << st.get(4)->second << std::endl;
    std::cout << st.floor(3)->second << std::endl;
    std::cout << st.ceiling(2)->second << std::endl;

    std::cout << st.rank(3) << std::endl;

    for (auto item:st)
        std::cout << item.first << ":" << item.second << std::endl;
    st.deleteKey(3);


    for (auto item:st)
        std::cout << item.first << ":" << item.second << std::endl;

}


#endif //ALGORITHM_ST_H
