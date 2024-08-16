#ifndef LAB3_DIRECTOR_H
#define LAB3_DIRECTOR_H

#include <iostream>
#include <vector>
#include "name.h"


class director : public name {
private:
    static int next_id;
public:
    friend std::ostream &operator<<(std::ostream &ostream, const director &director);
    director create_director();
    director first();
    ~director() {};
    int ID_inzilizete() const override;
};


#endif //LAB3_DIRECTOR_H
