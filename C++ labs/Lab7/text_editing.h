#ifndef LAB7_TEXT_EDITING_H
#define LAB7_TEXT_EDITING_H
#include "queue.h"
#include <vector>


class text_editing : public queue {
public:
    int window_size;
    int id_for_q;
    queue output_string;
    std::string input_file_name;
    std::string output_file_name;
    std::vector<char> file_string;

    text_editing(int id) : id_for_q(id){}
    ~text_editing(){}

    void change_window_size();
    void change_file_name();

    void line_processing();
    void div_file();

    void save_to_file();

    int get_longest_word_size();

    queue_node* find_by_id(int id);

    void change_data(int id, std::vector<char> new_str);
    void change();
};


#endif //LAB7_TEXT_EDITING_H
