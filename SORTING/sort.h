//
// Created by 邓鑫林 on 2021/1/4.
//
#ifndef ALGORITHM_SORT_H
#define ALGORITHM_SORT_H

#include <type_traits>
#include <string>
#include <vector>
#include "../FUNDAMENTALS/random.h"
#include "../utils/utils.h"

using namespace std;

template<typename Itr>
void selectionSort(Itr begin, Itr end) {
    //using value_type = typename iterator_traits<Itr>::value_type;

    if (begin == end)
        return;
    for (auto i = begin; i != end; i++) {
        Itr min = i;
        for (auto j = i + 1; j != end; j++)
            if (*j < *min)
                min = j;
        std::swap(*min, *i);
    }
}

template<typename Itr>
void insertionSort(Itr begin, Itr end) {
    //using value_type = typename iterator_traits<Itr>::value_type;
    if (begin == end)
        return;
    for (auto i = begin + 1; i != end; i++)
        for (auto j = i; j > begin && *j < *(j - 1); --j)
            std::swap(*j, *(j - 1));
}

/*
 * 对插入算法改进
 * 习题2.1.24 增加哨兵 -> 找出最小元素换到最左侧，减少j>begin的判断
 * 习题2.1.25 取消交换，改成移动较大元素
 */
template<typename Itr>
void insertionSortX(Itr begin, Itr end) {
    // using value_type = typename iterator_traits<Itr>::value_type;
    if (begin == end)
        return;
    /*
     Itr min = begin;
     for (Itr i = begin + 1; i != end; i++) {
         if (*i < *min)
             min = i;
     }
    std::swap(*begin, *min);
    */
    
    for (Itr i = end - 1; i != begin; --i) {
        if (*i < *(i - 1)) {
            std::swap(*i, *(i - 1));
        }
    }

    for (auto i = begin + 2; i != end; i++) {
        auto value = *i;
        auto j = i;
        for (; value < *(j - 1); --j)
            *j = std::move(*(j - 1));
        *j = std::move(value);
    }
}

template<typename Type>
bool isSorted(vector<Type> &v) {
    for (int i = 0; i < v.size() - 1; ++i) {
        if (v[i] > v[i + 1])
            return false;
    }
    return true;
}

double testSort(const string &s, vector<double> v) {
    auto beg = v.begin();
    auto end = v.end();
    Stopwatch stopwatch;
    if ("SelectionSort" == s)
        selectionSort(beg, end);
    else if ("InsertionSort" == s)
        insertionSort(beg, end);
    else if ("InsertionSortX" == s)
        insertionSortX(beg, end);

    double time = stopwatch.elapsedTime();
    cout << "##############\n" << s << endl;
    cout << boolalpha << "the result is :" << isSorted(v) << endl;
    cout << s << " spends " << time << "ms" << endl;
    return time;
}

void testSorting(int N) {
    Random random;
    random.setSeed(time(nullptr));
    vector<double> vec;
    vec.reserve(N);
    for (int i = 0; i < N; ++i)
        vec.push_back(random.uniform());

    double t1 = testSort("SelectionSort", vec);
    double t2 = testSort("InsertionSort", vec);
    double t3 = testSort("InsertionSortX", vec);
    cout << "##########\n" << "the speed ratio :  \n";
    cout << "InsertionSort / SelectionSort = " << t1 / t2 << "\n";
    cout << "InsertionSortX / SelectionSort = " <<t1/ t3 << "\n" << endl;
}

#endif //ALGORITHM_SORT_H
