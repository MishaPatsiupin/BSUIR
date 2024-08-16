#ifndef LAB3_DEVELOPER_H
#define LAB3_DEVELOPER_H
#include <vector>
#include "name.h"


class developer : public name {
private:
    static int next_id;
public:
    friend std::ostream &operator<<(std::ostream &ostream, const developer &developer);
    developer create_developer();
    developer first_developer();
 //   ~developer() {};
 int ID_inzilizete() const override;

};


#endif //LAB3_DEVELOPER_H
