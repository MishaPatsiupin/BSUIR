#ifndef LAB3_WORKER_H
#define LAB3_WORKER_H

#include <vector>

#include "name.h"


class worker : public name{
private:
    static int next_id;
public:

    friend std::ostream &operator<<(std::ostream &ostream, const worker &worker);

    worker first_worker();
    worker create_worker();
    ~worker() {};
    int ID_inzilizete() const override;
};


#endif //LAB3_WORKER_H
