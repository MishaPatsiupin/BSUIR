
#include "worker.h"
#include "tester.h"
#include "manager.h"
#include "developer.h"
#include "administrator.h"
#include "director.h"

#include "product.h"
#include "task_time.h"
#include "name.h"

#include "sting.h"

#include <iostream>
#include <vector>

/*
Класс сотрудника сделать абстрактным.

В нем прописать интерфейсы, необходимые для реализации функции в наследуемых классах.
Например, есть задачи на исполнение. Дать возможность Администратору давать задачи для остальных классов,
а остальным перегрузить функции выполнения задач (желательно использовать уникальный вывод в консоль для
определения класса, выполняющего задачу)
 */

int main() {



    std::vector<product> product_vec;
    product_vec.emplace_back();
    product_vec[0] = product_vec[0].first_product();

    std::vector<administrator> administrator_vec;
    administrator_vec.emplace_back();
    administrator_vec[0] = administrator_vec[0].first_administrator();

    std::vector<developer> developer_vec;
    developer_vec.emplace_back();
    developer_vec[0] = developer_vec[0].first_developer();

    std::vector<manager> manager_vec;
    manager_vec.emplace_back();
    manager_vec[0] = manager_vec[0].first_manager();

    std::vector<tester> tester_vec;
    tester_vec.emplace_back();
    tester_vec[0] = tester_vec[0].first_tester();

    std::vector<worker> worker_vec;
    worker_vec.emplace_back();
    worker_vec[0] = worker_vec[0].first_worker();

    director director_one;
    director_one = director_one.first();

    std::vector<std::shared_ptr<task_time>> task_vector;
//    std::vector<task_time> task_time_vec;

    int choize_menu = -1;
    while (true) {
        choize_menu = menu();
        switch (choize_menu) {
            case 0: {
                return 0;
                break;
            }
            case 1: {
                std::cout << director_one;
                switch (whot_you_wont(2)) {
                    case 0: {
                        break;
                    }
                    case 1: {
                        change_name(director_one);
                        break;
                    }
                    case 2: {
                        chacge_product(product_vec);
                        break;
                    }
                    case 3: {
                        view_administrator(administrator_vec);
                        break;
                    }
                    case 4: {
                        view_developer(developer_vec);
                        break;
                    }
                    case 5: {
                        view_manager(manager_vec);
                        break;
                    }
                    case 6: {
                        view_tester(tester_vec);
                        break;
                    }
                    case 7: {
                        view_worker(worker_vec);
                        break;
                    }
                }

                break;
            }
            case 2: {//Администратор, изменение статуса производства, ставить задачи на выполнение
                for (size_t i = 0; i < administrator_vec.size(); i++) {
                    std::cout << "\n(" << i + 1 << ") " << administrator_vec[i] << std::endl;
                }
                switch (whot_you_wont(5)) {
                    case 0: {
                        break;
                    }
                    case 1: {
                        view_administrator(administrator_vec);
                        break;
                    }
                    case 2: {
                        chacge_product(product_vec);
                        break;
                    }
                    case 3: {
                        std::shared_ptr<task_time> task1 = task_time::create_task();
                        task_vector.push_back(task1);
                        break;
                    }
                }
                break;
            }
            case 3: {
                for (size_t i = 0; i < manager_vec.size(); i++) {
                    std::cout << "\n(" << i + 1 << ") " << manager_vec[i];
                }

                switch (whot_you_wont(6)) {
                    case 0:{
                        break;
                    }
                    case 1: {
                        view_manager(manager_vec);
                        break;
                    }
                    case 2: {
                        chacge_product(product_vec);
                        break;
                    }
                    case 3: {
                        std::shared_ptr<task_time> task1 = task_time::create_task();
                        task_vector.push_back(task1);
                        break;
                    }
                }
                break;
            }
            case 4: {//-Разработчик, создание, участвует
                for (size_t i = 0; i < developer_vec.size(); i++) {
                    std::cout << "\n(" << i + 1 << ") " << developer_vec[i] << std::endl;
                }

                switch (whot_you_wont(7)) {
                    case 0:{
                        break;
                    }
                    case 1: {
                        view_developer(developer_vec);
                        break;
                    }
                    case 2: {
                        chacge_product(product_vec);
                        break;
                    }
                    case 3: {
                        std::shared_ptr<task_time> task1 = task_time::create_task();
                        task_vector.push_back(task1);
                        break;
                    }
                }
                break;
            }
            case 5: {//Тестировщик, примечания
                switch (whot_you_wont(8)) {
                    case 0:{
                        break;
                    }
                    case 1: {
                        for (size_t i = 0; i < tester_vec.size(); i++) {
                            std::cout << "\n(" << i + 1 << ") " << tester_vec[i] << std::endl;
                        }
                        break;
                    }
                    case 2: {
                        tester_product(product_vec);
                        break;
                    }
                    case 3: {
                        print_task(task_vector);
                        break;
                    }
                    case 4: {
                        break;
                    }
                }
                break;
            }
            case 6:{
                switch (whot_you_wont(11)) {
                    case 0: {
                        break;
                    }
                    case 1: {
                        for (size_t i = 0; i < worker_vec.size(); i++) {
                            std::cout << "\n(" << i + 1 << ") " << worker_vec[i] << std::endl;
                        }
                        break;
                    }
                    case 2: {
                        for (size_t i = 0; i < product_vec.size(); i++) {
                            std::cout << "\n(" << i + 1 << ") " << product_vec[i] << std::endl;
                        }
                        break;
                    }
                    case 3: {
                        print_task(task_vector);
                        break;
                    }
                    case 4: {
                        break;
                    }
                }
                break;
            }
        }
    }


    getchar();
    getchar();
    return 0;
}
