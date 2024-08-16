// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#include "matrix.h"
#include "func.h"

void matrix::create_matrix() {

    if (this->matrix_cols != 0) {
        this->matrix_cols = 0;
        this->matrix_data = {'\0'};
    }

    if (whot_create_matrix(1) == 0) {
        int temp_numbers_matrix_rows = 0;
        this->matrix_cols = whot_create_matrix(2);
        temp_numbers_matrix_rows = whot_create_matrix(3);

        for (int i = this->matrix_cols * temp_numbers_matrix_rows; i > 0; i--) {
            this->matrix_data.push_back(random(-20, 20));
        }
    } else {
        std::cin >> *this;
    }


    std::cout << *this;
}

/*void matrix::print_matrix() {
    std::cout << "\nYour matrix:\n";

    if (this->matrix_cols == 0) {
        std::cout << "Matrix emply\n";

    } else {
        for (int j = 0; j < this->matrix_data.size() / this->matrix_cols; j++) {
            for (int i = 0; i < matrix_cols; i++) {
                printf("%5d", matrix_data[j * this->matrix_cols + i]);
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
}*/

void matrix::transposing_matrix() {
    if (this->matrix_cols == 0) {
        std::cout << "Matrix emply\n";

    } else {
        this->matrix_data = transposing(this->matrix_data, this->matrix_cols);
        this->matrix_cols = this->matrix_data.size() / this->matrix_cols;
        std::cout << *this;
    }
}

void matrix::change_one_element() {
    std::cout << *this;
    int i = 0, j = 0;

    i = whot_create_matrix(6);
    j = whot_create_matrix(7);

    std::cout << "Enter a new value --> ";
    std::cin >> this->matrix_data[(i - 1) * this->matrix_cols + (j - 1)];
    std::cout << "Okk.";
    std::cout << *this;
}

void matrix::change_data() {
    int func_choize;

    func_choize = menu_change_data();

    switch (func_choize) {
        case 0: {
            break;
        }
        case 1: {
            this->change_one_element();
            break;
        }
        case 2: {
            this->add_row();
            break;
        }
        case 3: {
            this->delete_row();
            break;
        }
        case 4: {
            add_col();
            break;
        }
        case 5: {
            delete_col();
            break;
        }
    }
}

void matrix::add_row() {
    std::cout << *this;
    int after_this_rows = 0;
    std::cout << "After which line will the new line be placed?";
    after_this_rows = get_number(0);

    for (int i = 1; i <= this->matrix_cols; i++) {
        int time_znah = 0;
        std::cout << "Enter the " << i << "st element, -->";
        std::cin >> time_znah;

        auto position = this->matrix_data.begin() + after_this_rows * this->matrix_cols + i - 1;
        this->matrix_data.insert(position, time_znah);
    }
    std::cout << *this;
}

void matrix::delete_row() {
    std::cout << *this;
    int del_rows_after_this = 0;
    std::cout << "After which row do you want to delete?";
    del_rows_after_this = get_number(0);

    for (int i = 0; i < this->matrix_cols; i++) {
        this->matrix_data.erase(this->matrix_data.begin() + del_rows_after_this * this->matrix_cols);
    }
    std::cout << *this;
}

void matrix::add_col() {
    std::cout << *this;
    int after_this_cols = 0;
    std::cout << "After which cols will the new col be placed?";
    after_this_cols = get_number(0);

    int number_rows = this->matrix_data.size()/ this->matrix_cols;
int correction = 0;
    for (int i = 0; i < number_rows; i++) {
        int time_znah = 0;
        std::cout << "Enter the " << i+1 << "st element, -->";
        std::cin >> time_znah;

        auto position = this->matrix_data.begin() + i * this->matrix_cols + after_this_cols + correction;
        this->matrix_data.insert(position, time_znah);
correction++;
    }
    this->matrix_cols++;

    std::cout << *this;
}

void matrix::delete_col() {
    std::cout << *this;
    int del_col_after_this = 0; int correction = 0;
    std::cout << "After which col do you want to delete?";
    del_col_after_this = get_number(0);


    int number_rows = this->matrix_data.size()/ this->matrix_cols;
    for (int i = 0; i < number_rows; i++) {
        this->matrix_data.erase(this->matrix_data.begin() + del_col_after_this + this->matrix_cols * i + correction);
        correction--;
    }
    this->matrix_cols--;
    std::cout << *this;
}

std::ostream& operator<<(std::ostream& ostream, const matrix& matrix) {
    ostream << "your matrix:\n";

    if (matrix.matrix_cols == 0) {
        ostream << "Matrix emply\n";

    } else {
        for (int j = 0; j < matrix.matrix_data.size() / matrix.matrix_cols; j++) {
            for (int i = 0; i < matrix.matrix_cols; i++) {
                printf("%5d", matrix.matrix_data[j * matrix.matrix_cols + i]);
            }
            ostream << std::endl;
        }
        ostream << std::endl;
    }
    return ostream;
}

std::istream& operator>>(std::istream& istream, matrix& matrix) {
    matrix.matrix_cols = whot_create_matrix(4);
    int expected_number_of_rows = 1;
    for (int i = 1; i <= expected_number_of_rows; ++i) {//по строкам
        for (int j = 1; j <= matrix.matrix_cols; ++j) { //по столбцам
            std::cout << "\nEnter the element " << i << " rows, and  " << j << " cols.  -->";
            int buffer_num = 0;
            std::cin >> buffer_num;
            matrix.matrix_data.push_back(buffer_num);
        }
        int choise = 0;

        choise = whot_create_matrix(5);
        if (choise == 1) {
            expected_number_of_rows++;
        }
    }
    return istream;
}

matrix& matrix::operator=(const matrix& other) {

        this->matrix_cols = other.matrix_cols;
        this->matrix_data = other.matrix_data;

    return *this;
}


matrix matrix::operator+(const matrix& other) const {
    if (matrix_cols != other.matrix_cols || matrix_data.size() != other.matrix_data.size()) {
        throw std::invalid_argument("Matrices have different sizes and cannot be added");
        return *this;
    }

    matrix result;
    result.matrix_cols = matrix_cols;

    for (size_t i = 0; i < matrix_data.size(); i++) {
        result.matrix_data.push_back(matrix_data[i] + other.matrix_data[i]);
    }
std::cout << result;
    return result;
}

matrix matrix::operator-(const matrix& other) const {
    if (matrix_cols != other.matrix_cols || matrix_data.size() != other.matrix_data.size()) {
        throw std::invalid_argument("Matrices have different sizes and cannot be added");
        return *this;
    }

    matrix result;
    result.matrix_cols = matrix_cols;

    for (size_t i = 0; i < matrix_data.size(); i++) {
        result.matrix_data.push_back(matrix_data[i] - other.matrix_data[i]);
    }
    std::cout << result;
    return result;
}

matrix& matrix::operator+=(int N) {
    for (size_t i = 0; i < matrix_data.size(); i++) {
        matrix_data[i] += N;
    }
    std::cout << *this;
    return *this;
}

matrix& matrix::operator-=(int N) {
    for (size_t i = 0; i < matrix_data.size(); i++) {
        matrix_data[i] -= N;
    }
    std::cout << *this;
    return *this;
}

matrix& matrix::operator*=(int N) {
    for (size_t i = 0; i < matrix_data.size(); i++) {
        matrix_data[i] *= N;
    }
    std::cout << *this;
    return *this;
}

int matrix::operator==(const matrix& other) const {
    if (matrix_data.empty() && other.matrix_data.empty()) {
        return 0;
    } else if (matrix_data.empty()) {
        return 1;
    } else if (other.matrix_data.empty()) {
        return -1;
    } else {
        if (matrix_data[0] < other.matrix_data[0]) {
            return -1;
        } else if (matrix_data[0] > other.matrix_data[0]) {
            return 1;
        } else {
            return 0;
        }
    }
}
