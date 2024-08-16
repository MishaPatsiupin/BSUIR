#include "sting.h"
#include "manager.h"
#include "administrator.h"

void print_task(const std::vector<std::shared_ptr<task_time>>& task_vector) {
    for (const auto& task : task_vector) {
        std::cout << "Task: " << task->task << ", Time: " << task->time;
        if(task->flag_work == 0){
            std::cout << ", Task complete " << std::endl;;
        }else std::cout << ", Task in process" << std::endl;;
    }
}


int id_chet = 0;

int get_choize(int min, int max) {
    int choize = -1;

    while (choize == -1) {
        std::cout << "Enter the number from " << min << " to " << max << ": ";
        std::cin >> choize;

        if (std::cin.fail() || choize < min || choize > max) {
            std::cin.clear();
            std::cout << "Error input. Please try again." << std::endl;
            choize = -1;
        } else {
            std::cout << std::endl;
            return choize;
        }
    }
    std::cout << std::endl;
}


int get_number(int min) {
    int number = -1;

    while (number == -1) {
        if (!(std::cin >> number) || number < min) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Error input. Please try again." << std::endl;
        } else {
            std::cout << std::endl;
            return number;
        }
    }
    std::cout << std::endl;
    return -99;
}

int whot_you_wont(int i) {
    int flag = -1;

    switch (i) {
        case 1: {
            std::cout << "\nEnter 0, back" << std::endl;
            std::cout << "Enter 1, to changes data obj" << std::endl;
            std::cout << "Enter 2, to delete odj" << std::endl;
            std::cout << "Enter 3, to add new odj" << std::endl;

            flag = get_choize(0, 3);
            break;
        }
        case 2: {
            std::cout <<"\n----------DIRECTOR----------";
            std::cout << "\nEnter 0, back" << std::endl;
            std::cout << "Enter 1, to changes director's information" << std::endl;
            std::cout << "Enter 2, to view product" << std::endl;
            std::cout << "Enter 3, to view administrator's" << std::endl;
            std::cout << "Enter 4, to view developer's" << std::endl;
            std::cout << "Enter 5, to view manager's" << std::endl;
            std::cout << "Enter 6, to view tester's" << std::endl;
            std::cout << "Enter 7, to view worker's" << std::endl;

            flag = get_choize(0, 7);
            break;
        }
        case 3: {
            std::cout << "\nEnter number the element -> " << std::endl;

            flag = get_number(1) - 1;
            break;
        }
        case 4: {
            std::cout << "\nEnter the -2, if product in development" << std::endl;
            std::cout << "Enter the -1, if product is in production" << std::endl;
            std::cout << "Enter quantity, if product  out of Stock" << std::endl;
      //      std::cout << "Enter the product quantity" << std::endl;

            flag = get_number(-2);
            break;
        }
        case 5: {
            std::cout <<"\n----------ADMINISTRATOR'S----------";
            std::cout << "\nEnter 0, back" << std::endl;
            std::cout << "Enter 1, to change administrator's" << std::endl;
            std::cout << "Enter 2, to change product's" << std::endl;
            std::cout << "Enter 3, to setting a task to be completed" << std::endl;

            flag = get_choize(0, 3);
            break;
        }
        case 6: {
            std::cout <<"\n----------MANAGER'S----------";
            std::cout << "\nEnter 0, back" << std::endl;
            std::cout << "Enter 1, to change manager's" << std::endl;
            std::cout << "Enter 2, to change product's" << std::endl;
            std::cout << "Enter 3, to setting a task to be completed" << std::endl;

            flag = get_choize(0, 3);
            break;
        }
        case 7: {
            std::cout <<"\n----------DEVELOPER'S----------";
            std::cout << "\nEnter 0, back" << std::endl;
            std::cout << "Enter 1, to change developer's" << std::endl;
            std::cout << "Enter 2, to change product's" << std::endl;
            std::cout << "Enter 3, to setting a task to be completed" << std::endl;

            flag = get_choize(0, 3);
            break;
        }
        case 8: {
            std::cout <<"\n----------TESTER'S----------";
            std::cout << "\nEnter 0, back" << std::endl;
            std::cout << "Enter 1, to view tester's" << std::endl;
            std::cout << "Enter 2, to view product's" << std::endl;
            std::cout << "Enter 3, to view all task" << std::endl;

            flag = get_choize(0, 3);
            break;
        }
        case 9: {
            std::cout << "\nEnter 0, back" << std::endl;
            std::cout << "Enter 1, to changes primitives" << std::endl;

            flag = get_choize(0, 1);
            break;
        }
        case 10: {//-
            std::cout << "\nEnter 0, back" << std::endl;
            std::cout << "Enter 1, to view tester's" << std::endl;

            flag = get_choize(0, 1);
            break;
        }
        case 11: {
            std::cout <<"\n----------WORKER'S----------";
            std::cout << "\nEnter 0, back" << std::endl;
            std::cout << "Enter 1, to view worker's" << std::endl;
            std::cout << "Enter 2, to view product's" << std::endl;
            std::cout << "Enter 3, to view all task" << std::endl;

            flag = get_choize(0, 3);
            break;
        }

    }
    return flag;
}

void tester_product(std::vector<product> &products) {
    for (size_t i = 0; i < products.size(); i++) {
        std::cout << "\n(" << i + 1 << ") " << products[i] << std::endl;
    }

    int temp_choize = whot_you_wont(9);
    if (temp_choize == 0) {
        return;
    }

    int temp_element;
    temp_element = whot_you_wont(3);


    if (temp_choize == 1) {
        add_product_primitives(products[temp_element]);
    }
    if (temp_choize == 2) {
        if (temp_element < products.size()) {
            products.erase(products.begin() + temp_element);
        }

    }
}

void change_name(name &obj) {
    std::cin.ignore();
    char input;
    obj.name_name.clear();
    std::cout << "Enter the new name: ";
    while (std::cin.get(input) && input != '\n') {
        obj.name_name.push_back(input);
    }
    // std::cout << "Ok. New information:\n" << obj;
}

void view_administrator(std::vector<administrator> &administrators) {
    while (true) {
        for (size_t i = 0; i < administrators.size(); i++) {
            std::cout << "\n(" << i + 1 << ") administrators:" << administrators[i] << std::endl;
        }

        int temp_choize = whot_you_wont(1);
        if (temp_choize == 0) {
            return;
        }

        int temp_element;
        if (temp_choize != 3) {
            temp_element = whot_you_wont(3);
        }

        if (temp_choize == 1) {
            change_name(administrators[temp_element]);
        }
        if (temp_choize == 2) {
            if (temp_element < administrators.size()) {
                administrators.erase(administrators.begin() + temp_element);
            }

        }
        if (temp_choize == 3) {
            administrator new_admin;
            new_admin = new_admin.create_administrator();
            administrators.push_back(new_admin);
        }
    }
}


void chacge_product(std::vector<product> &products) {
    while (true) {
        for (size_t i = 0; i < products.size(); i++) {
            std::cout << "\n(" << i + 1 << ") products:" << products[i] << std::endl;
        }

        int temp_choize = whot_you_wont(1);
        if (temp_choize == 0) {
            return;
        }

        int temp_element;
        if (temp_choize != 3) {
            temp_element = whot_you_wont(3);
        }
        if (temp_choize == 1) {
            std::cout << "Which field do you want to change? or enter the 5 for change primitives\n";
            switch (get_choize(1, 5)) {
                case 1: {
                    change_name(products[temp_element]);
                    break;
                }
                case 2: {
                    products[temp_element].quantity = whot_you_wont(4);
                    break;
                }
                case 3: {
                    change_product_developer(products[temp_element]);
                    break;
                }
                case 4: {
                    change_product_manager(products[temp_element]);
                    break;
                }
                case 5: {
                    add_product_primitives(products[temp_element]);
                    break;
                }
            }
        }
        if (temp_choize == 2) {
            remove_product_element(products, temp_element);
        }
        if (temp_choize == 3) {
            product new_product;
            new_product = new_product.create_product();
            products.push_back(new_product);
        }
    }
}

void remove_product_element(std::vector<product> &obj, size_t index) {
    if (index < obj.size()) {
        obj.erase(obj.begin() + index);
    }
}

void change_product_developer(product &obj) {
    std::cin.ignore();
    char input;
    obj.developer.clear();
    std::cout << "Enter the new developer: ";
    while (std::cin.get(input) && input != '\n') {
        obj.developer.push_back(input);
    }
    //std::cout << "Ok. New information:\n" << obj;
}

void change_product_manager(product &obj) {
    std::cin.ignore();
    char input;
    obj.manager.clear();
    std::cout << "Enter the new manager: ";
    while (std::cin.get(input) && input != '\n') {
        obj.manager.push_back(input);
    }
   // std::cout << "Ok. New information:\n" << obj;
}

void add_product_primitives(product &obj) {
    std::cin.ignore();
    char input;
    obj.primitives.clear();
    std::cout << "Enter the primitives: ";
    while (std::cin.get(input) && input != '\n') {
        obj.primitives.push_back(input);
    }
    //std::cout << "Ok. New information:\n" << obj;
}

void view_developer(std::vector<developer> &developers) {
    while (true) {
        for (size_t i = 0; i < developers.size(); i++) {
            std::cout << "\n(" << i + 1 << ") " << developers[i] << std::endl;
        }

        int temp_choize = whot_you_wont(1);
        if (temp_choize == 0) {
            return;
        }
        int temp_element;
        if (temp_choize != 3) {
            temp_element = whot_you_wont(3);
        }

        if (temp_choize == 1) {
            change_name(developers[temp_element]);
        }
        if (temp_choize == 2) {
            if (temp_element < developers.size()) {
                developers.erase(developers.begin() + temp_element);
            }

        }
        if (temp_choize == 3) {
            developer new_developer;
            new_developer = new_developer.create_developer();
            developers.push_back(new_developer);
        }
    }
}

void view_manager(std::vector<manager> &managers) {
    while (true) {
        for (size_t i = 0; i < managers.size(); i++) {
            std::cout << "\n(" << i + 1 << ") " << managers[i];
        }

        int temp_choize = whot_you_wont(1);
        if (temp_choize == 0) {
            return;
        }

        int temp_element;
        if (temp_choize != 3) {
            temp_element = whot_you_wont(3);
        }

        if (temp_choize == 1) {
            change_name(managers[temp_element]);
        }
        if (temp_choize == 2) {
            if (temp_element < managers.size()) {
                managers.erase(managers.begin() + temp_element);
            }

        }
        if (temp_choize == 3) {
            manager new_manager;
            new_manager = new_manager.create_manager();
            managers.push_back(new_manager);
        }

    }
}

void view_tester(std::vector<tester> &testers) {
    while (true) {
        for (size_t i = 0; i < testers.size(); i++) {
            std::cout << "\n(" << i + 1 << ") " << testers[i] << std::endl;
        }

        int temp_choize = whot_you_wont(1);
        if (temp_choize == 0) {
            return;
        }

        int temp_element;
        if (temp_choize != 3) {
            temp_element = whot_you_wont(3);
        }

        if (temp_choize == 1) {
            change_name(testers[temp_element]);
        }
        if (temp_choize == 2) {
            if (temp_element < testers.size()) {
                testers.erase(testers.begin() + temp_element);
            }

        }
        if (temp_choize == 3) {
            tester new_tester;
            new_tester = new_tester.create_tester();
            testers.push_back(new_tester);
        }

    }
}

void view_worker(std::vector<worker> &workers) {
    while (true) {
        for (size_t i = 0; i < workers.size(); i++) {
            std::cout << "\n(" << i + 1 << ") " << workers[i] << std::endl;
        }

        int temp_choize = whot_you_wont(1);
        if (temp_choize == 0) {
            return;
        }

        int temp_element;
        if (temp_choize != 3) {
            temp_element = whot_you_wont(3);
        }

        if (temp_choize == 1) {
            change_name(workers[temp_element]);
        }
        if (temp_choize == 2) {
            if (temp_element < workers.size()) {
                workers.erase(workers.begin() + temp_element);
            }

        }
        if (temp_choize == 3) {
            worker new_worker;
            new_worker = new_worker.create_worker();
            workers.push_back(new_worker);
        }
    }
}
/*
int ID_inzilizete() {
    id_chet++;
    return id_chet;
}
*/
int menu() {
    int choize = -1;
    std::cout << "\n==============MENU==============" << std::endl;
    std::cout << "Enter 0, exit" << std::endl;
    std::cout << "Enter 1, to go to director" << std::endl;
    std::cout << "Enter 2, to go to administrator's" << std::endl;
    std::cout << "Enter 3, to go to manager's" << std::endl; //+строка, толбец
    std::cout << "Enter 4, to go to developer's" << std::endl;
    std::cout << "Enter 5, to go to tester's" << std::endl;
    std::cout << "Enter 6, to go to worker's" << std::endl;
    choize = get_choize(0, 6);
    std::cout << std::endl;
    return choize;
}
