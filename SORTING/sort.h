//
// Created by 邓鑫林 on 2021/1/4.
//
#ifndef ALGORITHM_STRINGSORT_H
#define ALGORITHM_SORT_H

#include <type_traits>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include "../FUNDAMENTALS/random.h"
#include "../utils/utils.h"
#include "PQ.h"
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
    if (len == 0)
        return;
    int h = 1;
    while (h < len / 3)
        h = 3 * h + 1;
    while (h >= 1) {
        for (int i = h; i < len; ++i) {
            auto value = *(begin + i);
            auto j = begin + i;
            for (; j >= begin + h && value < *(j - h); j -= h)
                *j = std::move(*(j - h));
            *j = std::move(value);
        }
        h /= 3;
    }
}

// 用函数对象实现
template<typename Itr>
class _MergeSort {
private:
    using value_type = typename iterator_traits<Itr>::value_type;
    vector<value_type> aux;

    void _merge(Itr begin, Itr mid, Itr end) {
        auto i = begin, j = mid;// [begin,mid),[mid,end)
        for (int k = 0; k < end - begin; k++) {
            if (i >= mid)
                aux[k] = std::move(*(j++));
            else if (j >= end)
                aux[k] = std::move(*(i++));
            else if (*i < *j)
                aux[k] = std::move(*(i++));
            else
                aux[k] = std::move(*(j++));
        }
        Itr itr = begin;
        for (int k = 0; k < (end - begin); k++)
            *(itr++) = aux[k];
    }

    void _sort(Itr begin, Itr end) {
        if (end - 1 <= begin)
            return;
        Itr mid = begin + ((end - begin) >> 1);
        _sort(begin, mid);
        _sort(mid, end);
        _merge(begin, mid, end);
    }

    void mergeSort(Itr begin, Itr end) {
        if (begin == end)
            return;
        _sort(begin, end);
    }

public:
    _MergeSort(Itr begin, Itr end) : aux(begin, end) {
        mergeSort(begin, end);
    }
};


template<typename Itr>
void mergeSort(Itr begin, Itr end) {
    _MergeSort<Itr>(begin, end);
}

/*
 * 改进
 * 2.2.2.1 对小规模数组用插入排序 设置CUTOFF
 * 2.2.2.2 测试数组是否已经有序
 * 2.2.2.3 不将元素复制到辅助数组
 */
template<typename Itr>
class _MergeSortX {
private:
    using value_type = typename iterator_traits<Itr>::value_type;
    vector<value_type> aux;
    const int CUTOFF = 15;

    void _merge(Itr srcBeg, Itr srcMid, Itr srcEnd, Itr destBeg, Itr destEnd) {
        // src 已经有序，转移到dest
        auto i = srcBeg, j = srcMid;// [begin,mid),[mid,end)
        auto dest = destBeg;
        for (int k = 0; k < srcEnd - srcBeg; k++) {
            //  ++优先级高于*
            if (i >= srcMid)
                *dest++ = std::move(*j++);
            else if (j >= srcEnd)
                *dest++ = std::move(*i++);
            else if (*i < *j)
                *dest++ = std::move(*i++);
            else
                *dest++ = std::move(*j++);
        }
    }

    // 给dest排序
    void _sort(Itr srcBeg, Itr srcEnd, Itr destBeg, Itr destEnd) {
        if ((srcEnd - srcBeg) < CUTOFF) {
            insertionSortX(destBeg, destEnd);
            return;
        }
        Itr srcMid = srcBeg + ((srcEnd - srcBeg) >> 1);
        Itr destMid = destBeg + ((destEnd - destBeg) >> 1);

        // 对src排序
        _sort(destBeg, destMid, srcBeg, srcMid);
        _sort(destMid, destEnd, srcMid, srcEnd);

        // sort 之后检查[srcBeg,srcEnd) 是否整体有序
        if (*(srcMid - 1) < *srcMid) {
            std::copy(srcBeg, srcEnd, destBeg);
            return;
        }
        // src是有序的，merge之后dest也有序，避免了一次复制
        _merge(srcBeg, srcMid, srcEnd, destBeg, destEnd);
    }

    void mergeSort(Itr begin, Itr end) {
        if (begin == end)
            return;
        _sort(aux.begin(), aux.end(), begin, end);
    }

public:
    _MergeSortX(Itr begin, Itr end) : aux(begin, end) {
        mergeSort(begin, end);
    }
};

template<typename Itr>
void mergeSortX(Itr begin, Itr end) {
    _MergeSortX<Itr>(begin, end);
}

// 自底向上归并
template<typename Itr>
class _MergeBU {
private:
    using value_type = typename iterator_traits<Itr>::value_type;
    vector<value_type> aux;

    void mergeSort(Itr beg, Itr end) {
        int N = end - beg;
        for (int sz = 1; sz < N; sz += sz) {
            for (int i = 0; i < N - sz; i += (sz + sz)) {
                merge(beg + i, beg + i + sz, std::min(beg + i + 2 * sz, end));
            }
        }
    }

    void merge(Itr beg, Itr mid, Itr end) {
        Itr i = beg, j = mid;
        for (int k = 0; k < (end - beg); ++k) {
            if (i >= mid)
                aux[k] = std::move(*j++);
            else if (j >= end)
                aux[k] = std::move(*i++);
            else if (*i < *j)
                aux[k] = std::move(*i++);
            else
                aux[k] = std::move(*j++);
        }
        Itr itr = beg;
        for (int k = 0; k < (end - beg); ++k)
            *itr++ = aux[k];
    }

public:
    _MergeBU(Itr beg, Itr end) : aux(beg, end) {
        mergeSort(beg, end);
    }

};

template<typename Itr>
void mergeBUSort(Itr beg, Itr end) {
    _MergeBU<Itr>(beg, end);
}

template<typename Itr>
class _QuickSort {
private:
    Random random;
    const int CUTOFF = 15;

    Itr partition(Itr beg, Itr end) {
        Itr i = beg, j = end;
        // 随机选取切分元素，避免数组逆序造成效率低下
        auto p = beg + random.uniformInt(end - beg - 1);
        std::swap(*p, *beg);
        auto pivot = *beg;
        while (true) {
            while (*(++i) < pivot)
                if (i == end - 1)
                    break;
            while (*(--j) > pivot)
                if (j == beg)
                    break;
            if (i >= j)
                break;
            std::swap(*i, *j);
        }
        std::swap(*beg, *j);
        return j;
    }

    void _sort(Itr beg, Itr end) {
        /*
         if (end - 1 <= beg)
             return;
        */
        // 改进，小数组使用插入排序
        if (end - beg <= CUTOFF) {
            insertionSortX(beg, end);
            return;
        }
        auto pivot = partition(beg, end);
        if (pivot != beg)
            sort(beg, pivot);
        if (pivot != end - 1)
            sort(pivot + 1, end);
    }

    // 重复多
    void _sort3way(Itr beg, Itr end) {
        if (end - beg <= 1)
            return;
        auto lt = beg, i = beg + 1, gt = end - 1;
        auto pivot = *beg;
        // [beg,lt-1] [lt,gt][gt+1,end)
        while (i <= gt) {
            if (*i < pivot)
                std::swap(*lt++, *i++);
            else if (*i > pivot)
                std::swap(*i, *gt--);
            else
                ++i;
        }
        _sort3way(beg, lt);
        _sort3way(gt + 1, end);
    }

    void sort3way(Itr beg, Itr end) {
        // random.shuffle(beg,end);
        _sort3way(beg, end);
    }

public:
    _QuickSort(Itr beg, Itr end) {
        random.setSeed(time(nullptr));
        _sort(beg, end);
    }
};

template<typename Itr>
void quickSort(Itr beg, Itr end) {
    _QuickSort<Itr>(beg, end);
}

template <typename Itr>
void heapSort(Itr beg,Itr end){
    using value_type = typename iterator_traits<Itr>::value_type;
    PQ<value_type,std::greater<value_type>> pq(beg,end);
    auto i = beg;
    while (!pq.isEmpty()){
        *i++ = pq.pop();
    }
}
/*
 * #############
 *  以下为辅助函数
 * #############
 */

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
    else if ("MergeSort" == s)
        mergeSort(beg, end);
    else if ("MergeSortX" == s)
        mergeSortX(beg, end);
    else if ("MergeBUSort" == s)
        mergeBUSort(beg, end);
    else if ("QuickSort" == s)
        quickSort(beg, end);
    else if ("HeapSort" == s)
        heapSort(beg,end);
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
    double t5 = testRandomInput(testSort, "MergeSort", N, T);
    double t6 = testRandomInput(testSort, "MergeSortX", N, T);
    double t7 = testRandomInput(testSort, "MergeBUSort", N, T);
    double t8 = testRandomInput(testSort, "QuickSort", N, T);
    double t9 = testRandomInput(testSort, "HeapSort", N, T);

    cout << "##########\n" << "the speed ratio :  \n";
    cout << "InsertionSort / SelectionSort = " << t1 / t2 << "\n";
    cout << "InsertionSortX / SelectionSort = " << t1 / t3 << "\n";
    cout << "ShellSort / SelectionSort = " << t1 / t4 << "\n";
    cout << "MergeSort / SelectionSort = " << t1 / t5 << "\n";
    cout << "MergeSortX / SelectionSort = " << t1 / t6 << "\n";
    cout << "MergeBUSort / SelectionSort = " << t1 / t7 << "\n";
    cout << "QuickSort / SelectionSort = " << t1 / t8 << "\n";
    cout << "HeapSort / SelectionSort = " << t1 / t9 << "\n" << endl;
}

#endif //ALGORITHM_STRINGSORT_H
