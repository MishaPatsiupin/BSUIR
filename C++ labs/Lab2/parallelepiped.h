#ifndef LAB2_PARALLELEPIPED_H
#define LAB2_PARALLELEPIPED_H

#include <algorithm>
#include <iostream>

struct Point {
    double x, y, z;
};

class parallelepiped {
public:
    Point points[4];
    Point min;
    Point max;


    parallelepiped(const Point &p1, const Point &p2, const Point &p3, const Point &p4) :
    points{p1, p2, p3, p4} {

        min = {std::min(p1.x, std::min(p2.x, std::min(p3.x, p4.x))),
               std::min(p1.y, std::min(p2.y, std::min(p3.y, p4.y))),
               std::min(p1.z, std::min(p2.z, std::min(p3.z, p4.z)))};
        max = {std::max(p1.x, std::max(p2.x, std::max(p3.x, p4.x))),
               std::max(p1.y, std::max(p2.y, std::max(p3.y, p4.y))),
               std::max(p1.z, std::max(p2.z, std::max(p3.z, p4.z)))};
    }

    bool operator&(const parallelepiped &other) const {
        // Проверяем пересечение по каждой из осей
        if (max.x < other.min.x || min.x > other.max.x) {
            return false;
        }
        if (max.y < other.min.y || min.y > other.max.y) {
            return false;
        }
        if (max.z < other.min.z || min.z > other.max.z) {
            return false;
        }

        return true;
    }


};


#endif //LAB2_PARALLELEPIPED_H
