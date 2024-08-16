#include "text_editing.h"


void text_editing::change_window_size() {
    std::cout << "\nEnter window size -> ";
    window_size = get_number(0);

    while (window_size < get_longest_word_size()) {
        std::cout << "\nWindow size is too small, min window size is " << get_longest_word_size();
        std::cout << "\nEnter window size -> ";
        window_size = get_number(0);
    }
}

void text_editing::change_file_name() {
input_file_name = "input.txt";
output_file_name = "output.txt";
}

void text_editing::div_file() {
    std::ifstream file(input_file_name);

    char c;
    while (file.get(c)) {
        if (c != '\n') {
            file_string.push_back(c);
        } else {
            file_string.push_back('\0');
            line_processing();
            file_string.clear();
        }
    }

    if (!file_string.empty()) {
        file_string.push_back('\0');
        line_processing();
        file_string.clear();
    }

    file.close();
}

void text_editing::line_processing() {
    std::vector<char> current_line;
    std::vector<char> current_word;
    int current_line_size = 0;

    for (char c : file_string) {
        if (c == ' ' || c == '\n' || c == '\t') {
            if (current_line_size + current_word.size() + 1 > window_size) {
                current_line.push_back('\0');
                output_string.enqueue(id_for_q++);
                output_string.get_last()->str = current_line;
                current_line.clear();
                current_line_size = 0;
            }

            if (current_word.size() + 1 <= window_size) {
                current_line.insert(current_line.end(), current_word.begin(), current_word.end());
                current_line.push_back(' ');
                current_line_size += current_word.size() + 1;
            }
            current_word.clear();

            if (c == '\n' && !current_line.empty()) {
                current_line.push_back('\0');
                output_string.enqueue(id_for_q++);
                output_string.get_last()->str = current_line;
                current_line.clear();
                current_line_size = 0;
            }

            if (c == '\t') {
                current_line.push_back('\t');
                current_line_size += 8;
            }
        } else {
            current_word.push_back(c);
        }
    }

    if (!current_word.empty() && current_word.size() + 1 + current_line_size <= window_size) {
        current_line.insert(current_line.end(), current_word.begin(), current_word.end());
        current_line_size += current_word.size();
    }

    if (!current_word.empty() && current_word.size() + 1 + current_line_size > window_size) {
        current_line.push_back('\0');
        output_string.enqueue(id_for_q++);
        output_string.get_last()->str = current_line;
        current_line.clear();
        current_line_size = 0;
        if (current_word.size() + 1 <= window_size) {
            current_line.insert(current_line.end(), current_word.begin(), current_word.end());
            current_line_size += current_word.size();
        }
    }

    if (!current_line.empty()) {
        current_line.push_back('\0');
        output_string.enqueue(id_for_q++);
        output_string.get_last()->str = current_line;
    }
}

void text_editing::save_to_file() {
    std::ofstream file(output_file_name, std::ios::trunc);

    queue_node* node = output_string.front;
    while (node != nullptr) {
        file << node->id << "-< " << node->str.data() << '\n';
        node = node->next;
    }

    file.close();
}

int text_editing::get_longest_word_size() {
    std::ifstream file(input_file_name);
    if (!file.is_open()) {
        std::cerr << "\nError to open input file";
        return -1;
    }

    int max_word_size = 0;
    int current_word_size = 0;
    char c;

    while (file.get(c)) {
        if (c == ' ' || c == '\n' || c == '\t') {

            if (current_word_size > max_word_size) {
                max_word_size = current_word_size;
            }
            current_word_size = 0;
        } else {

            current_word_size++;
        }
    }

    if (current_word_size > max_word_size) {
        max_word_size = current_word_size;
    }

    file.close();
    return max_word_size;
}




void text_editing::change_data(int id, std::vector<char> new_str) {
    queue_node *temp = find_by_id(id);
    temp->str.clear();
    temp->str.assign(new_str.begin(), new_str.end());
    for (int i = temp->str.size(); i < window_size - 1; i++) {
        temp->str.push_back(' ');
    }
    save_to_file();
}

void text_editing::change() {
    int choice;
    std::cout << "Enter 1 to change data by id -> ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    choice = get_number(0);
    while (choice == 1) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Enter the new string -> ";
        std::string input;
        std::getline(std::cin, input);
        std::vector<char> vec(input.begin(), input.end());
        std::cin.clear();
        int id;
        std::cout << "\nEnter id elementa-> ";
        id = get_number(0);
        while (find_by_id(id) == nullptr) {
            std::cout << "\nError id, enter again -> ";
            id = get_number(0);
        }
        change_data(id, vec);

        std::string command = "C:\\Windows\\System32\\notepad.exe D:/OAiP_3/Lab7/cmake-build-debug/output.txt";
        system(command.c_str());
    }
}

queue_node *text_editing::find_by_id(int id) {
    queue_node *temp = output_string.front;
    while (temp != nullptr) {
        if (temp->id == id) {
            return temp;
        }
        temp = temp->next;
    }
    return nullptr;
}
