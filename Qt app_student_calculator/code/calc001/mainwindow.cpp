#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGuiApplication>
#include <QProcess>
#include "vector"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "uimatrix.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    //chet_rot = 0;
    ui->setupUi(this);
    ui->basic_window->setSource(QUrl("qrc:/general.qml"));
    ui->basic_window->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_menu_button_clicked()
{
    if(ui->menu->width() <= 50){
        ui->menu->setFixedWidth(200);
        ui->horizontalLayout->setContentsMargins(0, 0, 160, 0);
    }else{
        ui->menu->setFixedWidth(40);
        ui->horizontalLayout->setContentsMargins(0, 0, 0, 0);
    }


}





void MainWindow::on_button_matrix_add_clicked()
{
    ui->menu->setFixedWidth(40);
    ui->horizontalLayout->setContentsMargins(0, 0, 0, 0);


    ui->basic_window->rootContext()->setContextProperty("duo_matrix", &duo_matrix);
    ui->basic_window->setSource(QUrl("qrc:/add_matrix.qml"));
    ui->basic_window->show();


}


void MainWindow::on_button_matrix_sub_clicked()
{
    ui->menu->setFixedWidth(40);
    ui->horizontalLayout->setContentsMargins(0, 0, 0, 0);

    ui->basic_window->rootContext()->setContextProperty("duo_matrix", &duo_matrix);
    ui->basic_window->setSource(QUrl("qrc:/sub_matrix.qml"));
    ui->basic_window->show();
}


void MainWindow::on_button_matrix_mul_clicked()
{
    ui->menu->setFixedWidth(40);
    ui->horizontalLayout->setContentsMargins(0, 0, 0, 0);

    ui->basic_window->rootContext()->setContextProperty("duo_matrix", &duo_matrix);
    ui->basic_window->setSource(QUrl("qrc:/mul_matrix.qml"));
    ui->basic_window->show();

}


void MainWindow::on_button_matrix_transpore_clicked()
{
    ui->menu->setFixedWidth(40);
    ui->horizontalLayout->setContentsMargins(0, 0, 0, 0);

    ui->basic_window->rootContext()->setContextProperty("duo_matrix", &duo_matrix);
    ui->basic_window->setSource(QUrl("qrc:/tra_matrix.qml"));
    ui->basic_window->show();
}


void MainWindow::on_button_matrix_rnk_clicked()
{
    ui->menu->setFixedWidth(40);
    ui->horizontalLayout->setContentsMargins(0, 0, 0, 0);

    ui->basic_window->rootContext()->setContextProperty("duo_matrix", &duo_matrix);
    ui->basic_window->setSource(QUrl("qrc:/rnk_matrix.qml"));
    ui->basic_window->show();
}


void MainWindow::on_button_matrix_clay_clicked()
{
    ui->menu->setFixedWidth(40);
    ui->horizontalLayout->setContentsMargins(0, 0, 0, 0);

    ui->basic_window->rootContext()->setContextProperty("duo_matrix", &duo_matrix);
    ui->basic_window->setSource(QUrl("qrc:/sly_matrix.qml"));
    ui->basic_window->show();

}




void MainWindow::on_button_rot_clicked()
{
    ui->menu->setFixedWidth(40);
    ui->horizontalLayout->setContentsMargins(0, 0, 0, 0);

    ui->basic_window->rootContext()->setContextProperty("rot_main", &rot_main);

    ui->basic_window->setSource(QUrl("qrc:/rot.qml"));
    ui->basic_window->show();


}



void MainWindow::on_button_about_clicked()
{
    ui->menu->setFixedWidth(40);
    ui->horizontalLayout->setContentsMargins(0, 0, 0, 0);

    ui->basic_window->setSource(QUrl("qrc:/about.qml"));
    ui->basic_window->show();

}


void MainWindow::on_new_window_clicked()
{
    QStringList arguments = QCoreApplication::arguments();
    QString applicationPath = arguments[0];
    QProcess::startDetached(applicationPath);
}


void MainWindow::on_open_log_clicked()
{
    system("C:\\Windows\\notepad.exe D:\\OAiP_3\\course's\\calculate\\build-calc001-Desktop_7f5b75-Debug\\log_file");
}



void MainWindow::on_button_matrix_det_clicked()
{
    ui->menu->setFixedWidth(40);
    ui->horizontalLayout->setContentsMargins(0, 0, 0, 0);

    ui->basic_window->rootContext()->setContextProperty("duo_matrix", &duo_matrix);
    ui->basic_window->setSource(QUrl("qrc:/det_matrix.qml"));
    ui->basic_window->show();
}


void MainWindow::on_button_dec_clicked()
{
    ui->menu->setFixedWidth(40);
    ui->horizontalLayout->setContentsMargins(0, 0, 0, 0);

    ui->basic_window->rootContext()->setContextProperty("duo_vec", &duo_vec);
    ui->basic_window->setSource(QUrl("qrc:/dec_vec.qml"));
    ui->basic_window->show();
}


void MainWindow::on_button_scal_clicked()
{
    ui->menu->setFixedWidth(40);
    ui->horizontalLayout->setContentsMargins(0, 0, 0, 0);

    ui->basic_window->rootContext()->setContextProperty("duo_vec", &duo_vec);
    ui->basic_window->setSource(QUrl("qrc:/scal_vec.qml"));
    ui->basic_window->show();
}


void MainWindow::on_button_vec_clicked()
{
    ui->menu->setFixedWidth(40);
    ui->horizontalLayout->setContentsMargins(0, 0, 0, 0);

    ui->basic_window->rootContext()->setContextProperty("duo_vec", &duo_vec);
    ui->basic_window->setSource(QUrl("qrc:/vec_vec.qml"));
    ui->basic_window->show();
}

