#include "uialgrot.h"
#include <QVariant>
#include <QGuiApplication>
#include <QProcess>
#include "vector"
#include <iostream>
#include <vector>
#include <cmath>
#include <QUrl>




uialgrot::uialgrot(QObject *parent)
    : QObject{parent}
{

}

void uialgrot::input_L_N(QVariantList L, QVariantList N)
{
    qDebug() << "input_L_N called";
    this->L_my.clear();
    this->N_my.clear();

    for (const QVariant &var : L)
    {
        L_my.push_back(var.toString().toStdString());
    }

    for (const QVariant &var : N)
    {
        N_my.push_back(var.toString().toStdString());
    }
    qDebug() << "L: " << this->L_my;
    qDebug() << "N: " << this->N_my;

}

void uialgrot::action_rot()
{
    log.f_log_text("\nАлгоритм рота: начало\n");


    log.f_log_vec("Входные данные, набор N: ", N_my);
    log.f_log_vec("Входные данные, набор L: ", L_my);

    algrot alg(L_my, N_my);
    alg.AlgRot();
    L_my.clear();
    N_my.clear();

    prepare_f();
     log.f_log_text("Алгоритм рота: завершен\n\n");

}

void uialgrot::prepare_f()
{
    std::ifstream file("rot_temp.txt");
    std::vector<std::string> lines;
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            lines.push_back(line);
        }
        file.close();
    }
    QVariantList variantList;
    for(const auto &str : lines)
    {
        variantList << QString::fromStdString(str);
    }
        emit send_f(variantList);
}
