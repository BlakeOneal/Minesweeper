using namespace std;
#pragma once
#include <random>
class Random {
private:
    static std::mt19937 random;
public:
    static int Int(int min, int max);
};



