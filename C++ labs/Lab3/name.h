#ifndef LAB3_NAME_H
#define LAB3_NAME_H
#include <iostream>
#include <vector>

class name {
private:
    static int next_id;
public:
    std::vector<char> name_name;
    int ID;

    virtual void print_name_id() const;
virtual int ID_inzilizete() const = 0;
    friend std::ostream &operator<<(std::ostream &ostream, const name &name);

};


#endif //LAB3_NAME_H
