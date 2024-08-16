#ifndef UIMATRIX_H
#define UIMATRIX_H
#include <QObject>
#include <QVariant>
#include "matrix.h"
#include "log.h"


class UImatrix : public QObject, public matrix, public log
{
     Q_OBJECT
public:
    explicit UImatrix(QObject *parent = nullptr);

    matrix mat1;
    matrix mat2;
    matrix result;

signals:

void sendMatrix(QVariant data, int cols, int rows);

public slots:
    void input_matrix1(QVariant data, int cols, int rows);
void input_matrix2(QVariant data, int cols, int rows);

    void action_add(); //mat1+mat2
    void action_sub();//mat1-mat2
    void action_mul();//*
    void action_tra();
    void action_rnk();
    void action_sly();
    void action_det();

    void prepareMatrix() {//отправка
    emit sendMatrix(QVariant::fromValue(result.mat_data), result.mat_cols, result.mat_rows);
}
private:
    void log_start(const std::string& str);
    void log_end(const std::string& str);
    void log_start_single(const std::string& str);

};

#endif // UIMATRIX_H
