#define _GNU_SOURCE
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>


void enable_protection(GtkWidget *widget, gpointer user_data);
void disable_protection(GtkWidget *widget, gpointer user_data);
void get_file_info(GtkWidget *widget, gpointer user_data);
void restore_files(GtkWidget *widget, gpointer user_data);
void update_files(GtkWidget *widget, gpointer user_data);
void configure_protection(GtkWidget *widget, gpointer user_data);
void show_help(GtkWidget *widget, gpointer user_data);
void exit_program(GtkWidget *widget, gpointer user_data);

// Функции для обработки пунктов подменю кнопки "Настроить защиту"
void path_configuration(GtkWidget *widget, gpointer user_data);
void check_interval_configuration(GtkWidget *widget, gpointer user_data);
void backup_type_configuration(GtkWidget *widget, gpointer user_data);
void hash_algorithm_configuration(GtkWidget *widget, gpointer user_data);
void access_rights_configuration(GtkWidget *widget, gpointer user_data);
void notification_channel_configuration(GtkWidget *widget, gpointer user_data);

void activate(GtkApplication *app, gpointer user_data);     //основное оконное приложение