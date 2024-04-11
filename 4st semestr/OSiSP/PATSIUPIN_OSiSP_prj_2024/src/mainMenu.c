#include "mainMenu.h"
#include <gtk/gtk.h>

#include <stdio.h>

// Функция для обработки события "Включить защиту"
void enable_protection(GtkWidget *widget, gpointer user_data)
{
    printf("Кнопка 'Включить защиту' была нажата.\n");
}

// Функция для обработки события "Прекратить защиту"
void disable_protection(GtkWidget *widget, gpointer user_data)
{
    printf("Кнопка 'Прекратить защиту' была нажата.\n");
}

// Функция для обработки события "Получить информацию об защищаемых файлах"
void get_file_info(GtkWidget *widget, gpointer user_data)
{
    printf("Кнопка 'Получить информацию об защищаемых файлах' была нажата.\n");
}

// Функция для обработки события "Принудительное восстановление файлов"
void restore_files(GtkWidget *widget, gpointer user_data)
{
    printf("Кнопка 'Принудительное восстановление файлов' была нажата.\n");
}

// Функция для обработки события "Обновить файлы"
void update_files(GtkWidget *widget, gpointer user_data)
{
    printf("Кнопка 'Обновить файлы' была нажата.\n");
}

// Функция для обработки события "Настроить защиту"
void configure_protection(GtkWidget *widget, gpointer user_data)
{
    printf("Кнопка 'Настроить защиту' была нажата.\n");
}

// Функция для обработки события "Справка"
void show_help(GtkWidget *widget, gpointer user_data)
{
    printf("Кнопка 'Справка' была нажата.\n");
}

// Функция для обработки события "Выход"
void exit_program(GtkWidget *widget, gpointer user_data)
{
    printf("Кнопка 'Выход' была нажата.\n");
    gtk_main_quit();
}

// Функция для обработки события "Указать путь к файлу, директории"
void path_configuration(GtkWidget *widget, gpointer user_data)
{
    printf("Пункт меню 'Указать путь к файлу, директории' был выбран.\n");
}

// Функция для обработки события "Изменить интервал проверки файлов"
void check_interval_configuration(GtkWidget *widget, gpointer user_data)
{
    printf("Пункт меню 'Изменить интервал проверки файлов' был выбран.\n");
}

// Функция для обработки события "Указать тип резервного копирования"
void backup_type_configuration(GtkWidget *widget, gpointer user_data)
{
    printf("Пункт меню 'Указать тип резервного копирования' был выбран.\n");
}

// Функция для обработки события "Указать алгоритм хеширования"
void hash_algorithm_configuration(GtkWidget *widget, gpointer user_data)
{
    printf("Пункт меню 'Указать алгоритм хеширования' был выбран.\n");
}

// Функция для обработки события "Изменить права доступа"
void access_rights_configuration(GtkWidget *widget, gpointer user_data)
{
    printf("Пункт меню 'Изменить права доступа' был выбран.\n");
}

// Функция для обработки события "Настроить канал уведомлений"
void notification_channel_configuration(GtkWidget *widget, gpointer user_data)
{
    printf("Пункт меню 'Настроить канал уведомлений' был выбран.\n");
}

void activate(GtkApplication *app, gpointer user_data)
{
    // Объявление переменных для виджетов окна
    GtkWidget *window;
    GtkWidget *button_enable;
    GtkWidget *button_disable;
    GtkWidget *button_get_info;
    GtkWidget *button_restore_files;
    GtkWidget *button_update_files;
    GtkWidget *button_configure;
    GtkWidget *button_help;
    GtkWidget *button_exit;
    GtkWidget *vbox;

    // Создание главного окна и задание его параметров
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "MCIGF");
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);

    // Создание вертикального контейнера для размещения виджетов
    vbox = gtk_box_new(TRUE, 8);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // Создание кнопок и подключение к ним функций обработки событий
    button_enable = gtk_button_new_with_label("Включить защиту");
    button_disable = gtk_button_new_with_label("Прекратить защиту");
    button_get_info = gtk_button_new_with_label("Получить информацию об защищаемых файлах");
    button_restore_files = gtk_button_new_with_label("Принудительное восстановление файлов");
    button_update_files = gtk_button_new_with_label("Обновить файлы");
    button_help = gtk_button_new_with_label("Справка");
    button_exit = gtk_button_new_with_label("Выход");

    g_signal_connect(button_enable, "clicked", G_CALLBACK(enable_protection), NULL);
    g_signal_connect(button_disable, "clicked", G_CALLBACK(disable_protection), NULL);
    g_signal_connect(button_get_info, "clicked", G_CALLBACK(get_file_info), NULL);
    g_signal_connect(button_restore_files, "clicked", G_CALLBACK(restore_files), NULL);
    g_signal_connect(button_update_files, "clicked", G_CALLBACK(update_files), NULL);
    g_signal_connect(button_help, "clicked", G_CALLBACK(show_help), NULL);
    g_signal_connect(button_exit, "clicked", G_CALLBACK(gtk_main_quit), NULL);

    // Создание главного меню
    GtkWidget *menubar = gtk_menu_bar_new();
    gtk_box_pack_start(GTK_BOX(vbox), menubar, FALSE, FALSE, 0);

    // Создание элемента меню "Настроить защиту"
    GtkWidget *menu_configure = gtk_menu_item_new_with_label("Настроить защиту");
    gtk_menu_shell_append(GTK_MENU_SHELL(menubar), menu_configure);

    // Создание подменю для элемента "Настроить защиту"
    GtkWidget *submenu = gtk_menu_new();
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(menu_configure), submenu);

    // Создание пунктов подменю
    GtkWidget *menu_item1 = gtk_menu_item_new_with_label("Указать путь к файлу, директории");
    GtkWidget *menu_item2 = gtk_menu_item_new_with_label("Изменить интервал проверки файлов");
    GtkWidget *menu_item3 = gtk_menu_item_new_with_label("Указать тип резервного копирования");
    GtkWidget *menu_item4 = gtk_menu_item_new_with_label("Указать алгоритм хеширования");
    GtkWidget *menu_item5 = gtk_menu_item_new_with_label("Изменить права доступа");
    GtkWidget *menu_item6 = gtk_menu_item_new_with_label("Настроить канал уведомлений");

    gtk_menu_shell_append(GTK_MENU_SHELL(submenu), menu_item1);
    gtk_menu_shell_append(GTK_MENU_SHELL(submenu), menu_item2);
    gtk_menu_shell_append(GTK_MENU_SHELL(submenu), menu_item3);
    gtk_menu_shell_append(GTK_MENU_SHELL(submenu), menu_item4);
    gtk_menu_shell_append(GTK_MENU_SHELL(submenu), menu_item5);
    gtk_menu_shell_append(GTK_MENU_SHELL(submenu), menu_item6);

    g_signal_connect(menu_item1, "activate", G_CALLBACK(path_configuration), NULL);
    g_signal_connect(menu_item2, "activate", G_CALLBACK(check_interval_configuration), NULL);
    g_signal_connect(menu_item3, "activate", G_CALLBACK(backup_type_configuration), NULL);
    g_signal_connect(menu_item4, "activate", G_CALLBACK(hash_algorithm_configuration), NULL);
    g_signal_connect(menu_item5, "activate", G_CALLBACK(access_rights_configuration), NULL);
    g_signal_connect(menu_item6,"activate", G_CALLBACK(notification_channel_configuration), NULL);

    // Размещение кнопок в вертикальном контейнере
    gtk_box_pack_start(GTK_BOX(vbox), button_enable, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), button_disable, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), button_get_info, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), button_restore_files, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), button_update_files, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), button_help, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), button_exit, TRUE, TRUE, 0);

    // Отображение всех виджетов
    gtk_widget_show_all(window);
}