//
// Created by 邓鑫林 on 2021/1/3.
//

#ifndef ALGORITHM_BINARYSEARCH_H
#define ALGORITHM_BINARYSEARCH_H

#include <vector>
#include "../utils/utils.h"
#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;

class BinarySearch {
private:
    BinarySearch() = default;

public:
    static int rank(int key, vector<int> a) {
        int lo = 0;
        int hi = a.size() - 1;
        while (lo <= hi) {
            int mid = (lo + hi) >> 1;
            if (key > a[mid])
                lo = mid + 1;
            else if (key < a[mid])
                hi = mid - 1;
            else
                return mid;
        }
        return -1;
    }

    static void test() {
        ofstream  fout;
        fout.open("../FILE/binarySearch.txt", ios::out | ios::trunc);
        writeRandomNum(fout, 0, 10000, 100);
        fout.close();
        ifstream fin;
        fin.open("../FILE/binarySearch.txt", ios::in);
        vector<int> whiteList = readVector<int>(fin);
        sort(whiteList.begin(),whiteList.end());
        fin.close();
        int tmp;
        cout << "input:  " << endl;
        while (cin >> tmp) {
            if (rank(tmp, whiteList) < 0)
                cout << tmp << " does not exit" <<endl;
            else
                cout<<tmp <<" already exits"<<endl;
        }
    }
};


#endif //ALGORITHM_BINARYSEARCH_H
