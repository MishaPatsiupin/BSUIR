/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtQuickWidgets/QQuickWidget>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *widget;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QPushButton *new_window;
    QPushButton *open_log;
    QLabel *hello1;
    QLabel *hello2;
    QQuickWidget *basic_window;
    QStatusBar *statusbar;
    QDockWidget *menu;
    QWidget *dockWidgetContents_3;
    QPushButton *menu_button;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *alg_ra;
    QLabel *label_3;
    QPushButton *button_matrix_add;
    QPushButton *button_matrix_sub;
    QPushButton *button_matrix_mul;
    QPushButton *button_matrix_det;
    QPushButton *button_matrix_transpore;
    QPushButton *button_matrix_rnk;
    QPushButton *button_matrix_clay;
    QWidget *layoutWidget;
    QVBoxLayout *vec_al;
    QLabel *label;
    QPushButton *button_dec;
    QPushButton *button_scal;
    QPushButton *button_vec;
    QWidget *verticalLayoutWidget_2;
    QVBoxLayout *oth_er;
    QLabel *label_2;
    QPushButton *button_rot;
    QPushButton *button_about;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(360, 733);
        MainWindow->setBaseSize(QSize(360, 740));
        MainWindow->setTabletTracking(false);
        MainWindow->setWindowTitle(QString::fromUtf8("MACL"));
        MainWindow->setLayoutDirection(Qt::LeftToRight);
        MainWindow->setAutoFillBackground(false);
        MainWindow->setStyleSheet(QString::fromUtf8(""));
        MainWindow->setIconSize(QSize(0, 0));
        MainWindow->setAnimated(true);
        widget = new QWidget(MainWindow);
        widget->setObjectName("widget");
        horizontalLayoutWidget = new QWidget(widget);
        horizontalLayoutWidget->setObjectName("horizontalLayoutWidget");
        horizontalLayoutWidget->setGeometry(QRect(0, 0, 311, 26));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        new_window = new QPushButton(horizontalLayoutWidget);
        new_window->setObjectName("new_window");
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(new_window->sizePolicy().hasHeightForWidth());
        new_window->setSizePolicy(sizePolicy);
        new_window->setMaximumSize(QSize(16777215, 25));

        horizontalLayout->addWidget(new_window);

        open_log = new QPushButton(horizontalLayoutWidget);
        open_log->setObjectName("open_log");
        open_log->setMaximumSize(QSize(16777215, 25));

        horizontalLayout->addWidget(open_log);

        hello1 = new QLabel(widget);
        hello1->setObjectName("hello1");
        hello1->setGeometry(QRect(140, 150, 49, 16));
        QFont font;
        font.setFamilies({QString::fromUtf8("Ink Free")});
        font.setPointSize(11);
        hello1->setFont(font);
        hello2 = new QLabel(widget);
        hello2->setObjectName("hello2");
        hello2->setGeometry(QRect(40, 170, 191, 20));
        hello2->setFont(font);
        basic_window = new QQuickWidget(widget);
        basic_window->setObjectName("basic_window");
        basic_window->setGeometry(QRect(0, 29, 311, 691));
        basic_window->setResizeMode(QQuickWidget::SizeRootObjectToView);
        MainWindow->setCentralWidget(widget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);
        menu = new QDockWidget(MainWindow);
        menu->setObjectName("menu");
        menu->setMinimumSize(QSize(40, 35));
        menu->setMaximumSize(QSize(200, 524287));
        menu->setStyleSheet(QString::fromUtf8("/*background-color: rgb(236, 236, 236);*/"));
        menu->setFloating(false);
        menu->setFeatures(QDockWidget::NoDockWidgetFeatures);
        menu->setAllowedAreas(Qt::BottomDockWidgetArea|Qt::LeftDockWidgetArea);
        dockWidgetContents_3 = new QWidget();
        dockWidgetContents_3->setObjectName("dockWidgetContents_3");
        dockWidgetContents_3->setEnabled(true);
        dockWidgetContents_3->setInputMethodHints(Qt::ImhNone);
        menu_button = new QPushButton(dockWidgetContents_3);
        menu_button->setObjectName("menu_button");
        menu_button->setEnabled(true);
        menu_button->setGeometry(QRect(0, 0, 251, 24));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(menu_button->sizePolicy().hasHeightForWidth());
        menu_button->setSizePolicy(sizePolicy1);
        menu_button->setMinimumSize(QSize(0, 0));
        QFont font1;
        font1.setKerning(false);
        menu_button->setFont(font1);
        menu_button->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    text-align: left;\n"
"    padding-left: 10px;\n"
"}"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/img/ing_photo/menu.png"), QSize(), QIcon::Normal, QIcon::Off);
        menu_button->setIcon(icon);
        menu_button->setCheckable(false);
        menu_button->setAutoRepeat(false);
        menu_button->setAutoExclusive(false);
        menu_button->setAutoRepeatInterval(99);
        menu_button->setAutoDefault(false);
        menu_button->setFlat(true);
        verticalLayoutWidget = new QWidget(dockWidgetContents_3);
        verticalLayoutWidget->setObjectName("verticalLayoutWidget");
        verticalLayoutWidget->setGeometry(QRect(0, 220, 201, 281));
        alg_ra = new QVBoxLayout(verticalLayoutWidget);
        alg_ra->setObjectName("alg_ra");
        alg_ra->setContentsMargins(0, 0, 0, 0);
        label_3 = new QLabel(verticalLayoutWidget);
        label_3->setObjectName("label_3");
        QFont font2;
        font2.setItalic(true);
        font2.setUnderline(true);
        label_3->setFont(font2);
        label_3->setAlignment(Qt::AlignCenter);

        alg_ra->addWidget(label_3);

        button_matrix_add = new QPushButton(verticalLayoutWidget);
        button_matrix_add->setObjectName("button_matrix_add");
        button_matrix_add->setEnabled(true);
        sizePolicy1.setHeightForWidth(button_matrix_add->sizePolicy().hasHeightForWidth());
        button_matrix_add->setSizePolicy(sizePolicy1);
        button_matrix_add->setMinimumSize(QSize(0, 0));
        button_matrix_add->setFont(font1);
        button_matrix_add->setAutoFillBackground(false);
        button_matrix_add->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    text-align: left;\n"
"    padding-left: 10px;\n"
"}"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/img/ing_photo/addition.png"), QSize(), QIcon::Normal, QIcon::Off);
        button_matrix_add->setIcon(icon1);
        button_matrix_add->setCheckable(false);
        button_matrix_add->setAutoRepeat(false);
        button_matrix_add->setAutoExclusive(false);
        button_matrix_add->setAutoRepeatInterval(99);
        button_matrix_add->setAutoDefault(false);
        button_matrix_add->setFlat(true);

        alg_ra->addWidget(button_matrix_add);

        button_matrix_sub = new QPushButton(verticalLayoutWidget);
        button_matrix_sub->setObjectName("button_matrix_sub");
        button_matrix_sub->setEnabled(true);
        sizePolicy1.setHeightForWidth(button_matrix_sub->sizePolicy().hasHeightForWidth());
        button_matrix_sub->setSizePolicy(sizePolicy1);
        button_matrix_sub->setMinimumSize(QSize(0, 0));
        button_matrix_sub->setFont(font1);
        button_matrix_sub->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    text-align: left;\n"
"    padding-left: 10px;\n"
"}"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/img/ing_photo/sub.png"), QSize(), QIcon::Normal, QIcon::Off);
        button_matrix_sub->setIcon(icon2);
        button_matrix_sub->setCheckable(false);
        button_matrix_sub->setAutoRepeat(false);
        button_matrix_sub->setAutoExclusive(false);
        button_matrix_sub->setAutoRepeatInterval(99);
        button_matrix_sub->setAutoDefault(false);
        button_matrix_sub->setFlat(true);

        alg_ra->addWidget(button_matrix_sub);

        button_matrix_mul = new QPushButton(verticalLayoutWidget);
        button_matrix_mul->setObjectName("button_matrix_mul");
        button_matrix_mul->setEnabled(true);
        sizePolicy1.setHeightForWidth(button_matrix_mul->sizePolicy().hasHeightForWidth());
        button_matrix_mul->setSizePolicy(sizePolicy1);
        button_matrix_mul->setMinimumSize(QSize(0, 0));
        button_matrix_mul->setFont(font1);
        button_matrix_mul->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    text-align: left;\n"
"    padding-left: 10px;\n"
"}"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/img/ing_photo/mul.png"), QSize(), QIcon::Normal, QIcon::Off);
        button_matrix_mul->setIcon(icon3);
        button_matrix_mul->setCheckable(false);
        button_matrix_mul->setAutoRepeat(false);
        button_matrix_mul->setAutoExclusive(false);
        button_matrix_mul->setAutoRepeatInterval(99);
        button_matrix_mul->setAutoDefault(false);
        button_matrix_mul->setFlat(true);

        alg_ra->addWidget(button_matrix_mul);

        button_matrix_det = new QPushButton(verticalLayoutWidget);
        button_matrix_det->setObjectName("button_matrix_det");
        button_matrix_det->setEnabled(true);
        sizePolicy1.setHeightForWidth(button_matrix_det->sizePolicy().hasHeightForWidth());
        button_matrix_det->setSizePolicy(sizePolicy1);
        button_matrix_det->setMinimumSize(QSize(0, 0));
        button_matrix_det->setFont(font1);
        button_matrix_det->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    text-align: left;\n"
"    padding-left: 10px;\n"
"}"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/img/ing_photo/d.png"), QSize(), QIcon::Normal, QIcon::Off);
        button_matrix_det->setIcon(icon4);
        button_matrix_det->setCheckable(false);
        button_matrix_det->setAutoRepeat(false);
        button_matrix_det->setAutoExclusive(false);
        button_matrix_det->setAutoRepeatInterval(99);
        button_matrix_det->setAutoDefault(false);
        button_matrix_det->setFlat(true);

        alg_ra->addWidget(button_matrix_det);

        button_matrix_transpore = new QPushButton(verticalLayoutWidget);
        button_matrix_transpore->setObjectName("button_matrix_transpore");
        button_matrix_transpore->setEnabled(true);
        sizePolicy1.setHeightForWidth(button_matrix_transpore->sizePolicy().hasHeightForWidth());
        button_matrix_transpore->setSizePolicy(sizePolicy1);
        button_matrix_transpore->setMinimumSize(QSize(0, 0));
        button_matrix_transpore->setFont(font1);
        button_matrix_transpore->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    text-align: left;\n"
"    padding-left: 10px;\n"
"}"));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/img/ing_photo/t.png"), QSize(), QIcon::Normal, QIcon::Off);
        button_matrix_transpore->setIcon(icon5);
        button_matrix_transpore->setCheckable(false);
        button_matrix_transpore->setAutoRepeat(false);
        button_matrix_transpore->setAutoExclusive(false);
        button_matrix_transpore->setAutoRepeatInterval(99);
        button_matrix_transpore->setAutoDefault(false);
        button_matrix_transpore->setFlat(true);

        alg_ra->addWidget(button_matrix_transpore);

        button_matrix_rnk = new QPushButton(verticalLayoutWidget);
        button_matrix_rnk->setObjectName("button_matrix_rnk");
        button_matrix_rnk->setEnabled(true);
        sizePolicy1.setHeightForWidth(button_matrix_rnk->sizePolicy().hasHeightForWidth());
        button_matrix_rnk->setSizePolicy(sizePolicy1);
        button_matrix_rnk->setMinimumSize(QSize(0, 0));
        button_matrix_rnk->setFont(font1);
        button_matrix_rnk->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    text-align: left;\n"
"    padding-left: 10px;\n"
"}"));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/img/ing_photo/rnk.png"), QSize(), QIcon::Normal, QIcon::Off);
        button_matrix_rnk->setIcon(icon6);
        button_matrix_rnk->setCheckable(false);
        button_matrix_rnk->setAutoRepeat(false);
        button_matrix_rnk->setAutoExclusive(false);
        button_matrix_rnk->setAutoRepeatInterval(99);
        button_matrix_rnk->setAutoDefault(false);
        button_matrix_rnk->setFlat(true);

        alg_ra->addWidget(button_matrix_rnk);

        button_matrix_clay = new QPushButton(verticalLayoutWidget);
        button_matrix_clay->setObjectName("button_matrix_clay");
        button_matrix_clay->setEnabled(true);
        sizePolicy1.setHeightForWidth(button_matrix_clay->sizePolicy().hasHeightForWidth());
        button_matrix_clay->setSizePolicy(sizePolicy1);
        button_matrix_clay->setMinimumSize(QSize(0, 0));
        button_matrix_clay->setFont(font1);
        button_matrix_clay->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    text-align: left;\n"
"    padding-left: 10px;\n"
"}"));
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/img/ing_photo/clay.png"), QSize(), QIcon::Normal, QIcon::Off);
        button_matrix_clay->setIcon(icon7);
        button_matrix_clay->setCheckable(false);
        button_matrix_clay->setAutoRepeat(false);
        button_matrix_clay->setAutoExclusive(false);
        button_matrix_clay->setAutoRepeatInterval(99);
        button_matrix_clay->setAutoDefault(false);
        button_matrix_clay->setFlat(true);

        alg_ra->addWidget(button_matrix_clay);

        layoutWidget = new QWidget(dockWidgetContents_3);
        layoutWidget->setObjectName("layoutWidget");
        layoutWidget->setGeometry(QRect(0, 37, 201, 151));
        vec_al = new QVBoxLayout(layoutWidget);
        vec_al->setObjectName("vec_al");
        vec_al->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(layoutWidget);
        label->setObjectName("label");
        label->setFont(font2);
        label->setAlignment(Qt::AlignCenter);

        vec_al->addWidget(label);

        button_dec = new QPushButton(layoutWidget);
        button_dec->setObjectName("button_dec");
        sizePolicy1.setHeightForWidth(button_dec->sizePolicy().hasHeightForWidth());
        button_dec->setSizePolicy(sizePolicy1);
        button_dec->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    text-align: left;\n"
"    padding-left: 10px;\n"
"}"));
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/img/ing_photo/kol-.png"), QSize(), QIcon::Normal, QIcon::Off);
        button_dec->setIcon(icon8);
        button_dec->setFlat(true);

        vec_al->addWidget(button_dec);

        button_scal = new QPushButton(layoutWidget);
        button_scal->setObjectName("button_scal");
        sizePolicy1.setHeightForWidth(button_scal->sizePolicy().hasHeightForWidth());
        button_scal->setSizePolicy(sizePolicy1);
        button_scal->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    text-align: left;\n"
"    padding-left: 10px;\n"
"}"));
        QIcon icon9;
        icon9.addFile(QString::fromUtf8(":/img/ing_photo/ckal-.png"), QSize(), QIcon::Normal, QIcon::Off);
        button_scal->setIcon(icon9);
        button_scal->setFlat(true);

        vec_al->addWidget(button_scal);

        button_vec = new QPushButton(layoutWidget);
        button_vec->setObjectName("button_vec");
        sizePolicy1.setHeightForWidth(button_vec->sizePolicy().hasHeightForWidth());
        button_vec->setSizePolicy(sizePolicy1);
        button_vec->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    text-align: left;\n"
"    padding-left: 10px;\n"
"}"));
        QIcon icon10;
        icon10.addFile(QString::fromUtf8(":/img/ing_photo/vec-.png"), QSize(), QIcon::Normal, QIcon::Off);
        button_vec->setIcon(icon10);
        button_vec->setFlat(true);

        vec_al->addWidget(button_vec);

        verticalLayoutWidget_2 = new QWidget(dockWidgetContents_3);
        verticalLayoutWidget_2->setObjectName("verticalLayoutWidget_2");
        verticalLayoutWidget_2->setGeometry(QRect(0, 540, 201, 151));
        oth_er = new QVBoxLayout(verticalLayoutWidget_2);
        oth_er->setObjectName("oth_er");
        oth_er->setContentsMargins(0, 0, 0, 0);
        label_2 = new QLabel(verticalLayoutWidget_2);
        label_2->setObjectName("label_2");
        label_2->setFont(font2);
        label_2->setAlignment(Qt::AlignCenter);

        oth_er->addWidget(label_2);

        button_rot = new QPushButton(verticalLayoutWidget_2);
        button_rot->setObjectName("button_rot");
        button_rot->setEnabled(true);
        sizePolicy1.setHeightForWidth(button_rot->sizePolicy().hasHeightForWidth());
        button_rot->setSizePolicy(sizePolicy1);
        button_rot->setMinimumSize(QSize(0, 0));
        button_rot->setFont(font1);
        button_rot->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    text-align: left;\n"
"    padding-left: 10px;\n"
"}"));
        QIcon icon11;
        icon11.addFile(QString::fromUtf8(":/img/ing_photo/rot-removebg-preview.png"), QSize(), QIcon::Normal, QIcon::Off);
        button_rot->setIcon(icon11);
        button_rot->setCheckable(false);
        button_rot->setAutoRepeat(false);
        button_rot->setAutoExclusive(false);
        button_rot->setAutoRepeatInterval(99);
        button_rot->setAutoDefault(false);
        button_rot->setFlat(true);

        oth_er->addWidget(button_rot);

        button_about = new QPushButton(verticalLayoutWidget_2);
        button_about->setObjectName("button_about");
        button_about->setEnabled(true);
        sizePolicy1.setHeightForWidth(button_about->sizePolicy().hasHeightForWidth());
        button_about->setSizePolicy(sizePolicy1);
        button_about->setMinimumSize(QSize(0, 0));
        button_about->setFont(font1);
        button_about->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    text-align: left;\n"
"    padding-left: 10px;\n"
"}"));
        QIcon icon12;
        icon12.addFile(QString::fromUtf8(":/img/ing_photo/aboat.png"), QSize(), QIcon::Normal, QIcon::Off);
        button_about->setIcon(icon12);
        button_about->setCheckable(false);
        button_about->setAutoRepeat(false);
        button_about->setAutoExclusive(false);
        button_about->setAutoRepeatInterval(99);
        button_about->setAutoDefault(false);
        button_about->setFlat(true);

        oth_er->addWidget(button_about);

        menu->setWidget(dockWidgetContents_3);
        MainWindow->addDockWidget(Qt::LeftDockWidgetArea, menu);

        retranslateUi(MainWindow);

        menu_button->setDefault(true);
        button_matrix_add->setDefault(false);
        button_matrix_sub->setDefault(false);
        button_matrix_mul->setDefault(false);
        button_matrix_det->setDefault(false);
        button_matrix_transpore->setDefault(false);
        button_matrix_rnk->setDefault(false);
        button_matrix_clay->setDefault(false);
        button_rot->setDefault(false);
        button_about->setDefault(false);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        new_window->setText(QCoreApplication::translate("MainWindow", "new window", nullptr));
        open_log->setText(QCoreApplication::translate("MainWindow", "open log file", nullptr));
        hello1->setText(QCoreApplication::translate("MainWindow", "Hello", nullptr));
        hello2->setText(QCoreApplication::translate("MainWindow", "<------------choose a category", nullptr));
        menu->setWindowTitle(QCoreApplication::translate("MainWindow", "menu", nullptr));
        menu_button->setText(QString());
        label_3->setText(QCoreApplication::translate("MainWindow", "Linear algebra", nullptr));
        button_matrix_add->setText(QCoreApplication::translate("MainWindow", "          matrix addition", nullptr));
        button_matrix_sub->setText(QCoreApplication::translate("MainWindow", "       matrix subtraction", nullptr));
        button_matrix_mul->setText(QCoreApplication::translate("MainWindow", "      matrix multiplication", nullptr));
        button_matrix_det->setText(QCoreApplication::translate("MainWindow", "            determinant", nullptr));
        button_matrix_transpore->setText(QCoreApplication::translate("MainWindow", "        transpose matrix", nullptr));
        button_matrix_rnk->setText(QCoreApplication::translate("MainWindow", "           rank of matrix", nullptr));
        button_matrix_clay->setText(QCoreApplication::translate("MainWindow", "   sustem of linear equations", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Vector algebra", nullptr));
        button_dec->setText(QCoreApplication::translate("MainWindow", "     orthogonal/collinear", nullptr));
        button_scal->setText(QCoreApplication::translate("MainWindow", "      scalar multiplication", nullptr));
        button_vec->setText(QCoreApplication::translate("MainWindow", "      vector multiplication", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "Other", nullptr));
        button_rot->setText(QCoreApplication::translate("MainWindow", "         algorithm rota", nullptr));
        button_about->setText(QCoreApplication::translate("MainWindow", "                about", nullptr));
        (void)MainWindow;
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
