#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QMessageBox"
#include "QPixmap"
#include <QObject>
#include "log.h"
#include <symengine/symengine_config.h>
#include <symengine/symbol.h>
#include <symengine/add.h>
#include <symengine/mul.h>
#include <symengine/pow.h>
#include <symengine/functions.h>
#include <symengine/visitor.h>
#include <symengine/eval.h>




int main(int argc, char *argv[])
{

    class log l;
    l.f_del_data();
    l.f_log_text("Программа запущена\n");

    QApplication a(argc, argv);
    MainWindow w;

    w.setFixedSize(360, 740);
    w.show();

    return a.exec();
        l.f_log_text("Программа завершена\n");
}
