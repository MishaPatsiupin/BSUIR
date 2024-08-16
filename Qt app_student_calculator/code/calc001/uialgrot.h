#ifndef UIALGROT_H
#define UIALGROT_H
#include <QObject>
#include <QVariant>
#include "algrot.h"

class uialgrot : public QObject, public algrot
{
     Q_OBJECT
private:
     int chet_action;
public:
    explicit uialgrot(QObject *parent = nullptr);

    std::vector<std::string> L_my;
    std::vector<std::string> N_my;

public slots:
    void input_L_N(QVariantList L, QVariantList N);
    void action_rot();
    void prepare_f();

signals:
    void send_f(QVariantList data);
};

#endif // UIALGROT_H
