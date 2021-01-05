//
// Created by 邓鑫林 on 2021/1/3.
//

#ifndef ALGORITHM_UTILS_H
#define ALGORITHM_UTILS_H

#include <vector>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <Windows.h>

using namespace std;

template<typename Type>
vector<Type> readVector(istream &in) {
    vector<Type> array;
    Type num;
    while (in >> num)
        array.push_back(num);
    return array;
}

void writeRandomNum(ostream &out, int begin, int end, int count) {
    srand((unsigned int) time(0));
    stringstream s;
    for (int i = 0; i < count; ++i) {
        s << (rand() % (end - begin + 1) + begin) << endl;
    }
    out << s.str();
}

class Stopwatch {
private:
    LARGE_INTEGER StartingTime, EndingTime, ElapsedMicroseconds;
    LARGE_INTEGER Frequency;
public:
    Stopwatch() {
        StartingTime.QuadPart = EndingTime.QuadPart = ElapsedMicroseconds.QuadPart  = Frequency.QuadPart = 0;
        QueryPerformanceCounter(&StartingTime);
    }

    double elapsedTime() {
        QueryPerformanceFrequency(&Frequency);
        QueryPerformanceCounter(&EndingTime);
        ElapsedMicroseconds.QuadPart = EndingTime.QuadPart - StartingTime.QuadPart;
        double timeSpend = (ElapsedMicroseconds.QuadPart * 1000.0) / Frequency.QuadPart;
        return timeSpend;
    }

    void restart() {
        QueryPerformanceCounter(&StartingTime);
    }
};

#endif //ALGORITHM_UTILS_H
