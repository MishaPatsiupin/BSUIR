#ifndef LAB3_ADMINISTRATOR_H
#define LAB3_ADMINISTRATOR_H
#include <iostream>
#include <vector>
#include "name.h"
#include <limits>


class administrator : public name{
private:
    static int next_id;
public:
    friend std::ostream &operator<<(std::ostream &ostream, const administrator &administrator);
    administrator create_administrator();
    administrator first_administrator();
    //   ~administrator() = default;
   int ID_inzilizete() const override;
};


#endif //LAB3_ADMINISTRATOR_H
