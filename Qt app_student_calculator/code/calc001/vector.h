#ifndef VECTOR_H
#define VECTOR_H
#include <iostream>
#include <vector>

class vector
{
public:
    vector();
    std::vector<double> vec1;
    std::vector<double> vec2;
    std::vector<double> res;

    struct point {
        double x, y, z;
    };
};

#endif // VECTOR_H
