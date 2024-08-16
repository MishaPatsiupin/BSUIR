#include "uivector.h"
#include <cmath>
#include "vector"
#include <iostream>

uivector::uivector(QObject *parent)
: QObject{parent}
{

}


void uivector::input_vec1(QVariant data)
{
    vec1.clear();

    try {
        if(data.isNull())
            throw std::runtime_error("Неверный 1го вектора");

        QList<QVariant> list = data.toList();
        for (int i = 0; i < list.size(); ++i) {
            this->vec1.push_back(list[i].toDouble());
        }
    } catch (const std::exception& e) {
        f_log_text("\nОшибка: ");
        f_log_text(e.what());
        f_log_text("\n");

    }
}

void uivector::input_vec2(QVariant data)
{
    vec2.clear();

    try {
        if(data.isNull())
            throw std::runtime_error("Неверный 2го вектора");

        QList<QVariant> list = data.toList();
        for (int i = 0; i < list.size(); ++i) {
            this->vec2.push_back(list[i].toDouble());
        }
    } catch (const std::exception& e) {
        f_log_text("\nОшибка: ");
        f_log_text(e.what());
        f_log_text("\n");

    }
}



//void uivector::input_vec1(QVariant data)
//{
//    vec1.clear();

//    QList<QVariant> list = data.toList();
//    for (int i = 0; i < list.size(); ++i) {
//        this->vec1.push_back(list[i].toDouble());
//    }
//}

//void uivector::input_vec2(QVariant data)
//{
//    vec2.clear();

//    QList<QVariant> list = data.toList();
//    for (int i = 0; i < list.size(); ++i) {
//        this->vec2.push_back(list[i].toInt());
//    }
//}


void uivector::input_vec_max(QVariant data)
{
vec_max.clear();
    QList<QVariant> list = data.toList();
    for (int i = 0; i < list.size(); ++i) {
        this->vec_max.push_back(list[i].toDouble());
    }
}

bool isOrt(const uivector::point& v1, const uivector::point& v2) {
    if(v1.x*v2.x + v1.y*v2.y + v1.z*v2.z == 0)
        return true;
    else
        return false;
}
bool isCollinear(const uivector::point& v1, const uivector::point& v2) {
    double kx = v1.x != 0 ? v2.x / v1.x : v2.x;
    double ky = v1.y != 0 ? v2.y / v1.y : v2.y;
    double kz = v1.z != 0 ? v2.z / v1.z : v2.z;
    return kx == ky && ky == kz;
}

void uivector::action_kol()
{
    f_log_text ("\nНахождение коллиниарности/ортагональности векторов: начато");
    f_log_vec("\nИсходный массив векторов: ", vec_max);

    res.clear();
    std::vector<uivector::point> vectors;
    for(int i = 0; i < vec_max.size() / 3; i++){
        uivector::point temp;
        temp.x = vec_max[i * 3];
        temp.y = vec_max[i * 3 + 1];
        temp.z = vec_max[i * 3 + 2];
        vectors.push_back(temp);
    }
    res.push_back(0);
    for(int i = 0; i < vectors.size(); i++){
        for (int j = i +1; j < vectors.size(); j++){
            if (isCollinear(vectors[i], vectors[j])) {
                f_log_text ("Вектора "); f_log_int(i+1); f_log_text (" и "); f_log_int( j+1);  f_log_text (" коллиниарны\n"); //лооооги
                res[0] = 1;
            }else {
                res[0] = 0;
            }
        }
    }
    res.push_back(0);
    for(int i = 0; i < vectors.size(); i++){
        for (int j = i +1; j < vectors.size(); j++){
            if (!isOrt(vectors[i], vectors[j])) {
                res[1] = 0;
            }else {
                f_log_text ("Вектора "); f_log_int(i+1); f_log_text (" и "); f_log_int( j+1);  f_log_text (" ортагональны\n"); //лооооги
                res[1] = 1;
            }
        }
    }

    qDebug() << res[0];
    prepareVec();
    f_log_text("Нахождение коллиниарности/ортагональности векторов: завершено\n");
}


double dot_mul(const uivector::point& v1, const uivector::point& v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

void uivector::action_scal()
{
    f_log_text ("\nСкалярное умножение векторов: начато");
        f_log_vec("\nИсходный массив векторов: ", vec_max);
    res.clear();
    res.push_back(0);

    double resultX = 1;
    for(int i = 0; i < vec_max.size(); i += 3){
        resultX *= vec_max[i];
    }

    double resultY = 1;
    for(int i = 1; i < vec_max.size(); i += 3){
        resultY *= vec_max[i];
    }

    double resultZ = 1;
    for(int i = 2; i < vec_max.size(); i += 3){
        resultZ *= vec_max[i];
    }

    res[0] = resultX + resultY + resultZ;
    prepareVec();
    f_log_text("Результат умножения: "); f_log_int(res[0]);
    f_log_text ("\nСкалярное умножение векторов: завершено\n");
}

void uivector::action_vec()
{
    f_log_text ("\nВекторное умножение векторов: начато");
    f_log_vec("Первый вектор: ", vec1);
    f_log_vec("Второй вектор: ", vec2);
    res.clear();

    res.push_back(vec1[1] * vec2[2] - vec1[2] * vec2[1]);
    res.push_back(vec1[2] * vec2[0] - vec1[0] * vec2[2]);
    res.push_back(vec1[0] * vec2[1] - vec1[1] * vec2[0]);

    prepareVec();
    f_log_vec("Разультат: ", res);
    f_log_text ("\nВекторное умножение векторов: завершено\n");
}





