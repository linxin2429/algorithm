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
    // 从后往前交换逆序对，并使得第一个元素最小
    for (auto i = end - 1; i != begin; --i) {
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

template<typename Itr>
void shellSort(Itr begin, Itr end) {
    int len = end - begin;
    int h = 1;
    while (h < len / 3)
        h = 3 * h + 1;
    while (h >= 1) {
        for (int i = h; i < len; ++i) {
            auto value = *(begin + i);
            auto j = begin + i;
            for (; j >= begin + h && value < *(j-h); j -= h)
                *j = std::move(*(j - h));
            *j = std::move(value);
        }
        h /= 3;
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

double testSort(const string &s, vector<double> &v) {
    auto beg = v.begin();
    auto end = v.end();
    Stopwatch stopwatch;
    if ("SelectionSort" == s)
        selectionSort(beg, end);
    else if ("InsertionSort" == s)
        insertionSort(beg, end);
    else if ("InsertionSortX" == s)
        insertionSortX(beg, end);
    else if ("ShellSort" == s)
        shellSort(beg, end);

    return stopwatch.elapsedTime();
}


template<typename Func>
double testRandomInput(Func testSort, const std::string &alg, int N, int T) {
    //使用算法将长度为N的数组排序T次
    double total = 0;
    std::vector<double> a(N);
    static Random r;
    bool sorted = true;
    for (int t = 0; t < T; ++t) {
        for (int i = 0; i < N; ++i)
            a[i] = r.uniform(0, 1);

        total += testSort(alg, a);
        if (!isSorted(a)) {
            sorted = false;
            break;
        }
    }

    cout << "##############\n" << alg << endl;
    cout << boolalpha << "the result is :" << sorted << endl;
    cout << alg << " spends " << total << "ms" << endl;
    return total;
}

void testSorting(int N, int T) {
    Random random;
    random.setSeed(time(nullptr));
    vector<double> vec;
    vec.reserve(N);
    for (int i = 0; i < N; ++i)
        vec.push_back(random.uniform());

    double t1 = testRandomInput(testSort, "SelectionSort", N, T);
    double t2 = testRandomInput(testSort, "InsertionSort", N, T);
    double t3 = testRandomInput(testSort, "InsertionSortX", N, T);
    double t4 = testRandomInput(testSort, "ShellSort", N, T);

    cout << "##########\n" << "the speed ratio :  \n";
    cout << "InsertionSort / SelectionSort = " << t1 / t2 << "\n";
    cout << "InsertionSortX / SelectionSort = " << t1 / t3 << "\n";
    cout << "ShellSortX / SelectionSort = " << t1 / t4 << "\n" << endl;
}

#endif //ALGORITHM_SORT_H
