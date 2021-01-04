//
// Created by 邓鑫林 on 2021/1/3.
//

#ifndef ALGORITHM_BAG_H
#define ALGORITHM_BAG_H

#include<forward_list>
#include<string>
#include <algorithm>

/**
 * Bag类不支持删除元素，支持插入以及遍历。
 * 实现这种功能最佳数据结构是C++的vector
 * 但是因为不需要随机访问，所以List也可以使用
 * 因为实现一个类需要考虑的细节已经在singleList这个类中实现了，所以我们只需要包装一下就好
 *
 * API
 * Bag()				创建一个空背包
 * void add(Item item)	添加一个元素
 *
 * bool isEmpty()		判断背包是否为空
 * int size()			背包中的数量
 *
 * Iter begin();		迭代
 * Iter end();
 *
 * string toString()	返回bag的字符串表示
 */
template<typename Type>
class Bag {
public:
    typedef std::forward_list<Type> List;
    typedef typename List::iterator Iterator;
    typedef typename List::const_iterator CIterator;
private:
    List _list;
    int _size;
public:
    Bag() : _list{List()}, _size(0) {}

    bool isEmpty() const {
        return _size == 0;
    }

    int size() const {
        return _size;
    }

    Iterator begin() {
        return _list.begin();
    }

    CIterator begin() const {
        return _list.begin();
    }

    Iterator end() {
        return _list.end();
    }

    CIterator end() const {
        return _list.end();
    }

    void add(const Type &x) {
        _list.push_front(x);
        ++_size;
    }

    void add(const Type &&x) {
        _list.push_front(std::move(x));
        ++_size;
    }

};

#endif //ALGORITHM_BAG_H
