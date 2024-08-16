#ifndef UIVECTOR_H
#define UIVECTOR_H
#include <QObject>
#include <QVariant>
#include "vector.h"
#include "log.h"
#include <string>

class uivector : public QObject, public vector, public log
{
     Q_OBJECT
public:
    explicit uivector(QObject *parent = nullptr);

    std::vector<double> vec_max;

public slots:
    void input_vec1(QVariant data);
    void input_vec2(QVariant data);
    void input_vec_max(QVariant data);

    void action_kol();
    void action_scal();
    void action_vec();

    void prepareVec() {
        emit sendVec(QVariant::fromValue(res));
    }

signals:
void sendVec(QVariant data);

private:


};

#endif // UIVECTOR_H
