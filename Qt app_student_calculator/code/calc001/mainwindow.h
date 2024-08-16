#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "uimatrix.h"
#include "uialgrot.h"
#include "uivector.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:


    void on_menu_button_clicked();


    void on_button_matrix_add_clicked();

    void on_button_matrix_sub_clicked();

    void on_button_matrix_mul_clicked();

    void on_button_matrix_transpore_clicked();

    void on_button_matrix_rnk_clicked();

    void on_button_matrix_clay_clicked();

    void on_button_rot_clicked();

    void on_button_about_clicked();

    void on_new_window_clicked();

    void on_open_log_clicked();


    void on_button_matrix_det_clicked();

    void on_button_dec_clicked();

    void on_button_scal_clicked();

    void on_button_vec_clicked();

private:
    Ui::MainWindow *ui;
    UImatrix duo_matrix;
    uivector duo_vec;
    uialgrot rot_main;

};
#endif // MAINWINDOW_H
