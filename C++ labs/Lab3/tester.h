#ifndef LAB3_TESTER_H
#define LAB3_TESTER_H

#include <vector>
#include "name.h"


class tester : public name{
private:
    static int next_id;
public:

    friend std::ostream &operator<<(std::ostream &ostream, const tester &tester);
    tester create_tester ();
    tester first_tester ();
    ~tester() {};
    int ID_inzilizete() const override;
};


#endif //LAB3_TESTER_H
