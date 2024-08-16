#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QMessageBox"
#include "QPixmap"
#include <QObject>
#include "log.h"





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
