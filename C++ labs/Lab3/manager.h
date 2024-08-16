#ifndef LAB3_MANAGER_H
#define LAB3_MANAGER_H
#include <iostream>
#include <vector>

#include "name.h"



class manager : public name{
private:
    static int next_id;
public:
    friend std::ostream &operator<<(std::ostream &ostream, const manager &manager);
    ~manager() {};

    manager create_manager();
    manager first_manager();
    int ID_inzilizete() const override;
};














#endif //LAB3_MANAGER_H
