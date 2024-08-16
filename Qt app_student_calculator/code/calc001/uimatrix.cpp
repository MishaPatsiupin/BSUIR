#include "uimatrix.h"
#include <QVariant>
#include <QGuiApplication>
#include <QProcess>
#include "mainwindow.h"
#include "vector"
#include <iostream>
#include <cmath>
#include <QUrl>
#include "log.h"

UImatrix::UImatrix(QObject *parent)
    : QObject{parent}
{

}

void UImatrix::input_matrix1(QVariant data, int cols, int rows)
{
    this->mat1.mat_data.clear();

    this->mat1.mat_cols = cols;
    this->mat1.mat_rows = rows;

    QList<QVariant> list = data.toList();
    this->mat1.mat_data.clear();
    for (int i = 0; i < list.size(); ++i) {
        this->mat1.mat_data.push_back(list[i].toInt());
    }

    qDebug() << "cols: " << this->mat1.mat_cols;
    qDebug() << "rows: " << this->mat1.mat_rows;
    qDebug() << "data: ";
    for (int i = 0; i < this->mat1.mat_data.size(); i++){
        qDebug() << this->mat1.mat_data[i] << " ";
    }
}


void UImatrix::input_matrix2(QVariant data, int cols, int rows)
{
        this->mat2.mat_data.clear();

    this->mat2.mat_cols = cols;
    this->mat2.mat_rows = rows;

    QList<QVariant> list = data.toList();
    this->mat2.mat_data.clear();
    for (int i = 0; i < list.size(); ++i) {
        this->mat2.mat_data.push_back(list[i].toInt());
    }

    qDebug() << "cols: " << this->mat2.mat_cols;
    qDebug() << "rows: " << this->mat2.mat_rows;
    qDebug() << "data: ";
    for (int i = 0; i < this->mat2.mat_data.size(); i++){
        qDebug() << this->mat2.mat_data[i] << " ";
    }
}

//void UImatrix::input_matrix2(QVariant data, int cols, int rows) {
//    try {
//        if (cols < 1 || rows < 1) {
//            throw std::runtime_error("Неверный размер матрицы");
//        }
//        QList<QVariant> list = data.toList();
//        this->mat2.mat_data.clear();
//        this->mat2.mat_cols = cols;
//        this->mat2.mat_rows = rows;

//        for (int i = 0; i < list.size(); ++i) {
//            this->mat2.mat_data.push_back(list[i].toInt());
//        }

//        qDebug() << "cols: " << this->mat2.mat_cols;
//        qDebug() << "rows: " << this->mat2.mat_rows;
//        qDebug() << "data: ";
//        for (int i = 0; i < this->mat2.mat_data.size(); i++) {
//            qDebug() << this->mat2.mat_data[i] << " ";
//        }
//    } catch (const std::exception& e) {
//        qDebug() << "Ошибка приема матрицы 2 классом - " << e.what();
//    }
//}

void UImatrix::action_add()
{
    log_start("Сложение матриц");

    this->result.mat_data.clear();
    this->result.mat_rows = this->mat1.mat_rows;
    this->result.mat_cols = this->mat1.mat_cols;

    for (int i = 0; i < this->mat1.mat_data.size(); i++){
        this->result.mat_data.push_back(this->mat1.mat_data[i] + this->mat2.mat_data[i]);
    }

    log_end("Сложение матриц");
    prepareMatrix();
}

void UImatrix::action_sub()
{
    log_start("Вычитание матриц");
    this->result.mat_data.clear();
    this->result.mat_rows = this->mat1.mat_rows;
    this->result.mat_cols = this->mat1.mat_cols;

    for (int i = 0; i < this->mat1.mat_data.size(); i++){
        this->result.mat_data.push_back(this->mat1.mat_data[i] - this->mat2.mat_data[i]);
    }

    log_end("Вычитание матриц");
    prepareMatrix();
}

void UImatrix::action_mul()
{
    log_start("Умножение матриц");
          this->result.mat_data.clear();
          this->result.mat_data = std::vector<double>(this->mat1.mat_rows * this->mat2.mat_cols, 0);

        for (int i = 0; i < this->mat1.mat_rows; ++i) {
            for (int j = 0; j < this->mat2.mat_cols; ++j) {
                for (int k = 0; k < this->mat1.mat_cols; ++k) {
                    this->result.mat_data[i * this->mat2.mat_cols + j] += this->mat1.mat_data[i * this->mat1.mat_cols + k] * this->mat2.mat_data[k * this->mat2.mat_cols + j];
                }
            }
        }

        result.mat_cols = mat2.mat_cols;
        result.mat_rows = mat1.mat_rows;

    log_end("Умножение матриц");
    prepareMatrix();
}

void UImatrix::action_tra()
{
    log_start_single("Таранспонирование матрицы");

        this->result.mat_data.clear();
        this->result.mat_data = std::vector<double>(this->mat1.mat_rows * this->mat1.mat_cols, 0);

        result.mat_cols = mat1.mat_rows;
        result.mat_rows = mat1.mat_cols;

        for (int i = 0; i < mat1.mat_rows; i++){
            for(int j = 0; j < mat1.mat_cols; j++){
                result.mat_data[j * mat1.mat_rows + i] = mat1.mat_data[i *  mat1.mat_cols + j];
            }
        }

            log_end("Транспонирование матрицы");

        prepareMatrix();
}

void UImatrix::action_rnk()
{
        log_start_single("Нахождение ранга матрицы");

        this->result.mat_data.clear();
        this->result.mat_data = std::vector<double>(1, 0);

        result.mat_cols = 1;
        result.mat_rows = 1;

        int rank = 0;
        const double EPSILON = 1E-10;
        int n = mat1.mat_rows;
        int m = mat1.mat_cols;

        std::vector<bool> row_selected(n, false);
std::vector<float> vec_float(mat1.mat_data.begin(), mat1.mat_data.end());

        for (int i = 0; i < m; ++i) {
            int j;
            for (j = 0; j < n; ++j) {
                if (!row_selected[j] && std::abs(vec_float[j * m + i]) > EPSILON)
                    break;
            }

            if (j != n) {
                ++rank;
                row_selected[j] = true;
                for (int p = i + 1; p < m; ++p)
                    vec_float[j * m + p] /= vec_float[j * m + i];
                for (int k = 0; k < n; ++k) {
                    if (k != j && std::abs(vec_float[k * m + i]) > EPSILON) {
                        for (int p = i + 1; p < m; ++p)
                            vec_float[k * m + p] -= vec_float[j * m + p] * vec_float[k * m + i];
                    }
                }
            }
        }
        result.mat_data[0] = rank;

        f_log_text("Ранг = ");
        f_log_int(result.mat_data[0]);
        f_log_text("\nНахожддение rnk: завершено");

        prepareMatrix();
}

std::vector<double> roundVectorToTenths(const std::vector<double>& inputVector) {
        std::vector<double> roundedVector;
        roundedVector.reserve(inputVector.size());

        for (const double& value : inputVector) {
            double roundedValue = std::round(value * 10) / 10; // Округляем до десятых
            roundedVector.push_back(roundedValue);
        }

        return roundedVector;
}

std::vector<double> solve(std::vector<std::vector<double>> a, std::vector<double> b) {
        int n = a.size();
        for (int i = 0; i < n; i++) {
            a[i].push_back(b[i]);
        }

        for (int i = 0; i < n; i++) {
            int pivot = i;
            for (int j = i + 1; j < n; j++) {
                if (abs(a[j][i]) > abs(a[pivot][i])) {
                    pivot = j;
                }
            }
            std::swap(a[i], a[pivot]);

            for (int j = i + 1; j <= n; j++) {
                a[i][j] /= a[i][i];
            }

            for (int j = 0; j < n; j++) {
                if (j != i) {
                    for (int k = n; k >= i; k--) {
                        a[j][k] -= a[i][k] * a[j][i];
                    }
                }
            }
        }

        std::vector<double> x(n);
        for (int i = 0; i < n; i++) {
            x[i] = a[i][n];
        }
        return roundVectorToTenths(x);
}

void UImatrix::action_sly()
{            log_start("Решение слау");
                this->result.mat_data.clear();

        int trmp_check = 0;
        for(int i = 0; i < mat1.mat_data.size(); i++){
            if(mat1.mat_data[i] == 0){
                trmp_check++;
            }
        }

        std::vector<std::vector<double>> matrix(mat1.mat_rows, std::vector<double>(mat1.mat_cols));
        for (int i = 0; i < mat1.mat_rows; ++i) {
            for (int j = 0; j < mat1.mat_cols; ++j) {
                matrix[i][j] = mat1.mat_data[i * mat1.mat_cols + j];
            }
        }

        std::vector<double> x = solve(matrix, mat2.mat_data);
        for(int i = 0; i < x.size(); i++){
            result.mat_data.push_back(0);
            result.mat_data[i] = x[i];
        }

        if (trmp_check == mat1.mat_data.size()){
            result.mat_data.clear();
            for(int i = 0; i < mat2.mat_data.size(); i++){
                result.mat_data.push_back(0);
            }
        }
        log_end("Решение слау");
                prepareMatrix();
}

double calculateDeterminant(int rows, int cols, const std::vector<double>& matrix) {
        if (rows == 1) {
            return matrix[0];
        }

        double determinant = 0.0;
        int sign = 1;

        for (int i = 0; i < cols; ++i) {
            std::vector<double> submatrix;
            int submatrixRows = rows - 1;

            for (int j = 1; j < rows; ++j) {
                for (int k = 0; k < cols; ++k) {
                    if (k != i) {
                        submatrix.push_back(matrix[j * cols + k]);
                    }
                }
            }

            determinant += sign * matrix[i] * calculateDeterminant(submatrixRows, cols - 1, submatrix);
            sign = -sign;
        }
        qDebug() << "Det: " << determinant;
        return determinant;
}

void UImatrix::action_det()
{         log_start_single("Нахождение детерминанта матрицы");
                 this->result.mat_data.clear();
        result.mat_data.push_back(calculateDeterminant(mat1.mat_rows, mat1.mat_cols, mat1.mat_data));

                 f_log_text("Детерминант = ");
                 f_log_int(result.mat_data[0]);
                 f_log_text("\nНахожддение детерминанта: завершено");
                 prepareMatrix();
}

void UImatrix::log_start(const std::string& str)
{
    f_log_text("\n\n");
    f_log_text(str);
    f_log_text(": начало");
    f_log_text("\n");
    f_log_vec("Первая матрица: ", this->mat1.mat_data);
    f_log_text("Количество столбцов: "); f_log_int(this->mat1.mat_cols);
    f_log_text("\n");
    f_log_text("Количество строк: "); f_log_int(this->mat1.mat_rows);
    f_log_text("\n");
    f_log_vec("Вторая матрица: ", this->mat1.mat_data);
    f_log_text("Количество столбцов: "); f_log_int(this->mat1.mat_cols);
    f_log_text("\n");
    f_log_text("Количество строк: "); f_log_int(this->mat1.mat_rows);
    f_log_text("\n");
}

void UImatrix::log_end(const std::string &str)
{
    f_log_vec("Результат: ", this->result.mat_data);
    f_log_text("Количество столбцов: "); f_log_int(this->result.mat_cols);
    f_log_text("\nКоличество строк: "); f_log_int(this->result.mat_rows);
    f_log_text("\n");
    f_log_text(str);
    f_log_text(": конец\n");
}

void UImatrix::log_start_single(const std::string &str)
{
    f_log_text("\n\n");
    f_log_text(str);
    f_log_text(": начало");
    f_log_text("\n");
    f_log_vec("Матрица: ", this->mat1.mat_data);
    f_log_text("Количество столбцов: "); f_log_int(this->mat1.mat_cols);
    f_log_text("\n");
    f_log_text("Количество строк: "); f_log_int(this->mat1.mat_rows);
    f_log_text("\n");
}


