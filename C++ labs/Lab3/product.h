#ifndef LAB3_PRODUCT_H
#define LAB3_PRODUCT_H

#include <vector>

#include "task_time.h"
#include "name.h"



class product : public  name, public task_time{
private:
    static int next_id;
public:

    friend std::ostream &operator<<(std::ostream &ostream, const product &product);
    product create_product ();
    product first_product ();
    ~product() {};


    int quantity = 0; //0 - нет в наличии, -1 производится, -2 в разработке, >0 в наличии
    std::vector<char> developer;
    std::vector<char> manager;
    std::vector<char> primitives;

    int ID_inzilizete() const override;
};



#endif //LAB3_PRODUCT_H
