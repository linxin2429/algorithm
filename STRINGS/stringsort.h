//
// Created by 邓鑫林 on 2021/1/12.
//

#ifndef ALGORITHM_STRINGSORT_H
#define ALGORITHM_STRINGSORT_H

#include <string>
#include <vector>
#include <iostream>
#include <iostream>
#include "../FUNDAMENTALS/random.h"
#include <ctime>

using namespace std;

class LSD { // 低位优先  适合键长相同的字符串
public:
    LSD(vector<string> &a, int W) {
        int N = a.size();
        int R = 256;
        vector<string> aux(N);
        // 根据第d个字符用键索引计数法排序
        for (int d = W - 1; d >= 0; --d) {
            vector<int> count(R + 1, 0);
            for (int i = 0; i < N; ++i) // 计算出现频率
                count[a[i][d] + 1]++;
            for (int r = 0; r < R; ++r) // 将出现频率转化为索引
                count[r + 1] += count[r];
            for (int i = 0; i < N; ++i) {// 将元素分类
                aux[count[a[i][d]]++] = a[i];
            }
            for (int i = 0; i < N; ++i) {
                a[i] = aux[i];
            }
        }
    }
};

class MSD {
    int R = 256; // 基数
    const int M = 15; // 小数组切换阈值
    vector<string> aux;
public:
    explicit MSD(vector<string> &a) : aux(a.size()) {
        sort(a, 0, a.size() - 1, 0);
    }

private:
    void sort(vector<string> &a, int lo, int hi, int d) {
        if (hi <= lo + M) {
            insertSort(a, lo, hi, d);
            return;
        }
        vector<int> count(R + 2, 0); // -1:结尾 0空出 R个字符
        for (int i = lo; i <= hi; ++i) {
            count[charAt(a[i], d) + 2]++;
        }
        for (int r = 0; r < R + 1; ++r) { // 计算各个字符的频率
            count[r + 1] += count[r];
        }
        for (int i = lo; i <= hi; ++i) {
            aux[count[charAt(a[i], d) + 1]++] = std::move(a[i]);
        }

        for (int i = lo; i <= hi; ++i) {
            a[i] = aux[i - lo];
        }
        for (int r = 0; r < R; ++r) {
            sort(a, lo + count[r], lo + count[r + 1] - 1, d + 1);
        }
    }

    static int charAt(const string &str, int d) {
        return d < str.size() ? str[d] : -1;
    }

    static void insertSort(vector<string> &a, int lo, int hi, int d) {
        for (int i = lo; i <= hi; ++i) {
            int j = i;
            string tmp = std::move(a[j]);
            for (; j > lo && less(tmp, a[j - 1], d); --j) {
                a[j] = a[j - 1];
            }
            a[j] = std::move(tmp);
        }
    }

    static bool less(const string &str1, const string &str2, int d) {
        for (int i = d; i < std::min(str1.size(), str2.size()); ++i) {
            if (str1[i] < str2[i]) return true;
            if (str1[i] > str2[i]) return false;
        }
        return str1.size() < str2.size();
    }
};

class Quick3string {
    const int M = 15; // 小数组切换阈值

    static int charAt(const string &str, int d) {
        return d < str.size() ? str[d] : -1;
    }

    static void insertSort(vector<string> &a, int lo, int hi, int d) {
        for (int i = lo; i <= hi; ++i) {
            int j = i;
            string tmp = std::move(a[j]);
            for (; j > lo && less(tmp, a[j - 1], d); --j) {
                a[j] = std::move(a[j - 1]);
            }
            a[j] = std::move(tmp);
        }
    }

    static bool less(const string &str1, const string &str2, int d) {
        for (int i = d; i < std::min(str1.size(), str2.size()); ++i) {
            if (str1[i] < str2[i]) return true;
            if (str1[i] > str2[i]) return false;
        }
        return str1.size() < str2.size();
    }

    void sort(vector<string> &a, int lo, int hi, int d) {
        if (hi <= lo + M) {
            insertSort(a, lo, hi, d);
            return;
        }
        int lt = lo, gt = hi;
        int v = charAt(a[lo], d);
        int i = lo + 1;
        while (i <= gt) {
            int t = charAt(a[i], d);
            if (t < v) std::swap(a[i++], a[lt++]);
            else if (t > v) std::swap(a[i], a[gt--]);
            else i++;
        }
        sort(a, lo, lt - 1, d);
        if (v >= 0)
            sort(a, lt, gt, d + 1);
        sort(a, gt + 1, hi, d);
    }

public:
    explicit Quick3string(vector<string> &s) {
        sort(s, 0, s.size() - 1, 0);
    }
};

void testStringSort() {
    vector<string> str;
    int N = 1000000, M = 10;
    Random random;
    random.setSeed(time(nullptr));
    for (int i = 0; i < N; ++i) {
        string tmp(M, 0);
        int m = random.uniformInt(M - 5, M);
        for (int j = 0; j < m; ++j) {
            tmp[j] = random.uniformInt('A', 'z');
        }
        str.push_back(tmp);
    }

//    MSD msd(str);
    Quick3string quick3String(str);
//    ostringstream out;
    for (int i = 0; i < N - 1; ++i) {
        if (str[i] >= str[i + 1]) {
            cout << "erro" << endl;
            break;
        }
//        out << str[i] << "   " << i << "\n";
    }
    cout << "success "<< endl;
//    cout << out.str();
}

#endif //ALGORITHM_STRINGSORT_H
