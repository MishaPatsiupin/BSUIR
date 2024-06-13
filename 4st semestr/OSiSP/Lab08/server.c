#define _GNU_SOURCE
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <stdbool.h>
#include <dirent.h>
#include <sys/stat.h>

//порты для передачи данных
#define SERVER_FTP_PORT 1231
#define DATA_CONNECTION_PORT SERVER_FTP_PORT +1
#define MAX_CLIENTS 10

//статус коды
#define OK 0
#define ER_INVALID_HOST_NAME (-1)
#define ER_CREATE_SOCKET_FAILED (-2)
#define ER_BIND_FAILED (-3)
#define ER_CONNECT_FAILED (-4)
#define ER_SEND_FAILED (-5)
#define ER_RECEIVE_FAILED (-6)



int svc_init_server(int *s);
int send_mes (int s, char *msg, int  msg_size);
int receive_message(int s, char *buf, int  buffer_size, int *msg_size);
int data_connect(char *server_name, int *s);
void *client_handler(void *args);

void process_cmd(int msg_size, const char *user_cmd, char *argument, char *cmd);
void process_cwd(char *proc_cwd);
void remove_substr(char *, char *);

void opt_pwd(char *buffer, char *reply_msg, char *cwd);
void opt_help(char *reply_msg);
void opt_ls(char *reply_msg, char *root_dir);
void opt_mkdir(char *argument, char *reply_msg);
void opt_rmdir(char *argument, char *reply_msg);
void opt_del(char *argument, char *reply_msg);
void opt_cd(char *argument, char *reply_msg, int *cd_counter, char *root_dir);
void opt_info(char *buffer, char *reply_msg);
/*void opt_recv(char *argument, char *reply_msg);
void opt_put(char *argument, char *reply_msg, int msg_size);*/

FILE *my_file;
pthread_mutex_t mutex;

struct client_socket{
    int client_sock;
    char root_dir[4096];
};

int main(int argc, char *argv[]){
    int server_sock;   //сокет для коннекта с клиентом
    int client_sock;  //сокет взаимодействия с клиентом
    int status;
    if(argc < 2) {
        fprintf(stderr, "USAGE:\n ./server <имя_каталога>ъ\n");
        exit(1);
    }
    fprintf(stdout, "Начало запуска FTP-сервера\n");
    fprintf(stdout, "Инициализация FTP-сервера\n");
    if(strcmp(argv[1], "./") != 0)
        if (chdir(argv[1]) < 0) {
            fprintf(stderr, "Переданный каталог не найден\n");
            exit(1);
        }
    status = svc_init_server(&server_sock);
    if (status != 0) {
        fprintf(stdout, "Закрытие FTP-сервера из-за ошибки\n");
        exit(status);
    }

    fprintf(stdout, "FTP-сервер ждет подключения клиента\n");

    pthread_mutex_init(&mutex, NULL);
    while (1) {
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        client_sock = accept(server_sock, (struct sockaddr*) &client_addr, &client_addr_len);
        if (client_sock < 0) {
            perror("Невозможно настроить соединение : ");
            fprintf(stderr, "FTP-сервер аварийно завершает работу\n");
            close(server_sock);
            continue;
        }
        fprintf(stdout, "Получен доступ к новому клиенту\n");
        pthread_t thread;
        struct client_socket client;
        client.client_sock = client_sock;
        strcpy(client.root_dir, argv[1]);
        if ((pthread_create(&thread, NULL, client_handler, &client)) != 0) {
            fprintf(stderr, "Failed to accept client connection\n");
            break;
        }
        pthread_detach(thread);
    }
    pthread_mutex_destroy(&mutex);
    fprintf(stdout,"Закрытие сокета сервера\n");
    close(server_sock);
    return 0;
}

void *client_handler(void *args){
    struct client_socket data = *(struct client_socket*) args;
    int cd_counter = 0;
    char user_cmd[1024];	//введенная пользователем строка
    char cmd[1024];		//команда без аргументов
    char argument[1024]; //аргумент команды
    char reply_msg[4115];
    char buffer[4096];
    int msg_size;        //размер принятого сообщения в байтах
    opt_info(buffer, reply_msg);
    process_cwd(data.root_dir);
    if(send_mes(data.client_sock, reply_msg, strlen(reply_msg) + 1) < 0){
        fprintf(stderr, "Ошибка соединения\n");
        close (data.client_sock);  //завершение работы;
        pthread_exit(NULL);
    }
    do{
        memset(reply_msg, '\0', sizeof(reply_msg));
        memset(user_cmd, '\0', sizeof(user_cmd));
        memset(buffer, '\0', sizeof(buffer));
        if(receive_message(data.client_sock, user_cmd, sizeof(user_cmd), &msg_size) < 0){
            fprintf(stdout,"Ошибка получения команды. Закрытие портов \n");
            fprintf(stderr,"FTP-сервера аварийно завершен\n");
            break;
        }
        process_cmd(msg_size, user_cmd, argument, cmd);
        fprintf(stdout,"Команда пользователя : %s\n", cmd);
        fprintf(stdout,"с аргументом : %s\n", argument);
        if(strcmp(cmd, "pwd") == 0)
            opt_pwd(buffer, reply_msg, data.root_dir);
        else if(strcmp(cmd, "ls") == 0)
            opt_ls(reply_msg, data.root_dir);
        else if(strcmp(cmd, "mkdir") == 0)
            opt_mkdir(argument, reply_msg);
        else if(strcmp(cmd, "rmdir") == 0)
            opt_rmdir(argument, reply_msg);
        else if(strcmp(cmd, "del") == 0)
            opt_del(argument, reply_msg);
        else if(strcmp(cmd, "cd") == 0)
            opt_cd(argument, reply_msg, &cd_counter, data.root_dir);
        else if(strcmp(cmd, "info") == 0)
            opt_info(buffer, reply_msg);
        else if(strcmp(cmd, "help") == 0)
            opt_help(reply_msg);
/*        else if(strcmp(cmd, "recv") == 0)
            opt_recv(argument, reply_msg);
        else if(strcmp(cmd, "put") == 0)
            opt_put(argument, reply_msg, msg_size);*/
        else if(strcmp(cmd, "echo") == 0){
            memset(reply_msg, '\0', sizeof(reply_msg));
            strcpy(reply_msg, argument);
        }
        else if(strcmp(cmd, "quit") == 0) {
            memset(reply_msg, '\0', sizeof(reply_msg));
            strcpy(reply_msg, "cmd 231 okay, user logged out\n");
        } else
            sprintf(reply_msg, "cmd 202 that is not a valid command\n");

        if(send_mes(data.client_sock, reply_msg, strlen(reply_msg) + 1) < 0)
            break;
    }while(strcmp(cmd, "quit") != 0);

    fprintf(stdout,"Закрытие сокета клиента\n");
    close (data.client_sock);  //завершение работы;
    pthread_exit(NULL);
}

int svc_init_server (int *s){
    int sock;
    struct sockaddr_in svc_addr;

    if((sock = socket(AF_INET, SOCK_STREAM,0)) < 0){
        perror("cannot create socket");
        return(ER_CREATE_SOCKET_FAILED);
    }
    memset((char *)&svc_addr, 0, sizeof(svc_addr));
    svc_addr.sin_family = AF_INET;
    svc_addr.sin_addr.s_addr = htonl(INADDR_ANY);  /* IP сервера */
    svc_addr.sin_port = htons(SERVER_FTP_PORT);    /* порт считывания сервера */

    int flag = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(int));
    if(bind(sock, (struct sockaddr *)&svc_addr, sizeof(svc_addr)) < 0){
        perror("cannot bind");
        close(sock);
        return(ER_BIND_FAILED);
    }
    listen(sock, MAX_CLIENTS);
    *s = sock;

    return(OK);
}

int data_connect (char *server_name, int *s ){ //создание принимающего сокета
    int sock;

    struct sockaddr_in client_address;  //локальный IP клиента
    struct sockaddr_in server_address;	//IP сервера
    struct hostent *server_IP_structure; //IP сервера в бинарном виде

    if((server_IP_structure = gethostbyname(server_name)) == NULL){ //получаем IP сервера
        fprintf(stdout, "%s неизвестный адрес\n", server_name);
        return (ER_INVALID_HOST_NAME);  /* error return */
    }
    if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("Невозможно создать сокет\n");
        return (ER_CREATE_SOCKET_FAILED);
    }
    memset((char *) &client_address, 0, sizeof(client_address));
    client_address.sin_family = AF_INET;
    client_address.sin_addr.s_addr = htonl(INADDR_ANY);
    client_address.sin_port = 0;
    if(bind(sock, (struct sockaddr *)&client_address, sizeof(client_address)) < 0){
        perror("Невозможно связать сокет с клиентом\n");
        close(sock);
        return(ER_BIND_FAILED);
    }
    memset((char *) &server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    memcpy((char *) &server_address.sin_addr, server_IP_structure->h_addr_list[0],server_IP_structure->h_length);
    server_address.sin_port = htons(DATA_CONNECTION_PORT);
    if (connect(sock, (struct sockaddr *) &server_address, sizeof(server_address)) < 0){
        fprintf(stdout, "\n %s\n", strerror(errno));
        close (sock);
        return(ER_CONNECT_FAILED);
    }
    *s = sock;
    return(OK);
}


int send_mes(int s, char *msg, int msg_size){
    int i;
    for(i=0; i < msg_size; i++)
        fprintf(stdout,"%c",msg[i]);
    fprintf(stdout,"\n");

    if((send(s, msg, msg_size, 0)) < 0){
        perror("unable to send ");
        return(ER_SEND_FAILED);
    }
    return(OK);
}


int receive_message (int s, char *buf, int buffer_size, int *msg_size ){
    int i;
    *msg_size = recv(s, buf, buffer_size, 0);

    if(*msg_size < 0){
        perror("Unable to receive");
        return(ER_RECEIVE_FAILED);
    }
    for(i = 0; i < *msg_size; i++)
        fprintf(stdout, "%c", buf[i]);
    fprintf(stdout,"\n");

    return (OK);
}

void process_cmd(int msg_size, const char *user_cmd, char *argument, char *cmd){
    int i = 0, j = 0;
    memset(cmd, 0, 1024);
    memset(argument, 0, 1024);
    while(user_cmd[i] == ' ')
        i++;
    while(i < msg_size && user_cmd[i] != ' ' && user_cmd[i] != '\n'){
        cmd[j] = user_cmd[i];
        i++;
        j++;
    }
    while(user_cmd[i] == ' ')
        i++;
    j = 0;
    if(user_cmd[i] == '"') {
        i++;
        while(i < msg_size && user_cmd[i] != '"'){
            argument[j] = user_cmd[i];
            i++;
            j++;
        }
    }
    else{
        while(i < msg_size && user_cmd[i] != ' ' && user_cmd[i] != '\n'){
            argument[j] = user_cmd[i];
            i++;
            j++;
        }
    }
}

void remove_substr(char *str1, char *str2)
{
    char* temp;
    temp = strstr(str1, str2);
    strcpy(str1, temp + strlen(str2));
}


void process_cwd(char *proc_cwd){
    size_t slash_pos = 0;
    char cwd[4096];
    getcwd(cwd, sizeof(cwd));
    for(size_t i = 0; i < strlen(cwd); i++)
        if(cwd[i] == '/') slash_pos = i;
    strcpy(proc_cwd, cwd + slash_pos) ;
}

void opt_pwd(char *buffer, char *reply_msg, char *cwd){
    char temp[4096];
    pthread_mutex_lock(&mutex);
    buffer = get_current_dir_name();
    strcpy(buffer,  strcpy(temp, strstr(buffer, cwd)) + strlen(cwd) );
    sprintf(reply_msg, "cmd 250 ok\n/server%s\n", buffer);
    pthread_mutex_unlock(&mutex);
}

void opt_help(char *reply_msg){
    strcpy(reply_msg, "Команды\t\t Функционал \t\t Синтаксис\n"
                      "pwd  \t\t рабочий каталог   \t pwd\n"
                      "echo  \t\t проверка сообщения с клиентом   \t echo mes\n"
                      "info  \t\t  информация о сервере   \t info\n"
                      "cd   \t\t смена каталога  \t cd dir\n"
                      "del \t\t удалить файл     \t del file\n"
                      "mkdir\t\t создать каталог  \t mkdir dir\n"
                      "rmdir\t\t удалить каталог  \t rmdir dir\n"
                      "ls   \t\t вывести файлы каталога\t ls\n"
    );
}

void opt_ls(char *reply_msg, char *root_dir)
{
    char *directory = get_current_dir_name();
    DIR *dir = opendir(directory);                                                              //открытие каталога

    struct dirent *dir_entry;                                                                   //структура, содержащая имя файла
    struct stat file_stat;                                                                      //структура, содержащая тип файла
    while ((dir_entry = readdir(dir)) != NULL)                                                  //чтение каталога
    {
        if (!(strcmp(dir_entry->d_name, ".") && strcmp(dir_entry->d_name, "..")))
            continue;

        char temp_name[255];
        strcpy(temp_name, directory);
        strcat(temp_name, "/");
        strcat(temp_name, dir_entry->d_name);
        lstat(temp_name, &file_stat);

        if (S_ISDIR(file_stat.st_mode))                                                         //если найдена папка
        {
            strcat(reply_msg, dir_entry->d_name);
            strcat(reply_msg, "/\n");
        }

        if (S_ISREG(file_stat.st_mode))                                                         //если найден файл
        {
            strcat(reply_msg, dir_entry->d_name);
            strcat(reply_msg, "\n");
        }

        if (S_ISLNK(file_stat.st_mode))                                                         //если найдена ссылка
        {
            char link_target[1024];
            memset(link_target, 0, 1024);

            ssize_t len = readlink(dir_entry->d_name, link_target, 1024 - 1);
            if (len != -1)
            {
                link_target[len] = '\0';
                char arrow_type[6];
                struct stat st;

                memset(arrow_type, 0, 6);

                if (lstat(link_target, &st) == 0 && S_ISREG(st.st_mode))
                    strcpy(arrow_type, "-->");
                else
                    strcpy(arrow_type, "-->>");

                remove_substr(link_target, root_dir);

                strcat(reply_msg, dir_entry->d_name);
                strcat(reply_msg, " ");
                strcat(reply_msg, arrow_type);
                strcat(reply_msg, " ");
                strcat(reply_msg, link_target);
                strcat(reply_msg, "\n");
            }
        }
    }

    closedir(dir);                                                                              //закрытие каталога
}


void opt_mkdir(char *argument, char *reply_msg){
    pthread_mutex_lock(&mutex);
    if(argument[0] == '\0') {
        fprintf(stderr,"Повторите запрос. В команде отсутствуют аргументы\n");
    }
    char sub_command[1031];
    sprintf(sub_command, "mkdir %s\n", argument);
    system(sub_command);
    sprintf(reply_msg, "cmd 212 successfully created dir %s\n", argument);
    pthread_mutex_unlock(&mutex);
}

void opt_rmdir(char *argument, char *reply_msg){
    pthread_mutex_lock(&mutex);
    char sub_command[1030];
    if(strlen(argument) == 0) {
        sprintf(reply_msg, "Повторите запрос. В команде отсутствуют аргументы\n");
    }
    sprintf(sub_command, "rmdir %s", argument);
    if(system(sub_command) < 0) {
        sprintf(reply_msg, "Ошибка. Попробуйте еще раз.\n");
    }
    sprintf(reply_msg, "cmd 212 successfully removed %s\n", argument);
    pthread_mutex_unlock(&mutex);
}

void opt_del(char *argument, char *reply_msg){
    pthread_mutex_lock(&mutex);
    char sub_command[1027];
    if(strlen(argument) == 0) {
        sprintf(reply_msg, "Отсутствует аргумент\n");
    }
    sprintf(sub_command, "rm %s", argument);
    if(system(sub_command) < 0 ) {
        sprintf(reply_msg, "Ошибка.\n");
    }
    sprintf(reply_msg, "cmd 211 okay, deleted %s\n", argument);
    pthread_mutex_unlock(&mutex);
}

void opt_cd(char *argument, char *reply_msg, int *cd_counter, char *root_dir){
    char *substr;
    char dir[1024];
    char old_dir[1024];
    int counter_start = *cd_counter;

    memset(dir, '\0', 1024);
    strcpy(old_dir,get_current_dir_name());

    pthread_mutex_lock(&mutex);

    substr = strtok(argument, "/\n");
    while(*substr == ' ') substr++;
    if((strcmp(substr, "..") == 0 && *cd_counter <= 0))
        strcpy(dir, root_dir);
    else if (strcmp(substr, "..") == 0)
    {
        (*cd_counter)--;
        strcat(dir, substr);
    }
    else if (strcmp(substr, ".") != 0)
    {
        (*cd_counter)++;
        strcat(dir, substr);
    }

    while ((substr = strtok(NULL, "/\n")) != NULL)
    {
        if((strcmp(substr, "..") == 0 && *cd_counter <= 0))
            continue;

        if(strcmp(substr, ".") == 0)
            continue;

        strcat(dir, "/");
        if (strcmp(substr, "..") == 0)
        {
            (*cd_counter)--;
            strcat(dir, substr);
        }
        else
        {
            (*cd_counter)++;
            strcat(dir, substr);
        }
    }
    fprintf(stdout,"%s\n", dir);

    int result = chdir(dir);
    if (result == 0)
        sprintf(reply_msg, "cmd 250 ok\n");
    else if (errno == ENOENT)
    {
        sprintf(reply_msg, "Такого каталога не существует\n");
        chdir(old_dir);
        *cd_counter = counter_start;
    }
    fprintf(stdout, "counter = %d\n", *cd_counter);
    pthread_mutex_unlock(&mutex);
}

void opt_info(char *buffer, char *reply_msg){
    pthread_mutex_lock(&mutex);
    system("echo FTP-сервер запущен и готов к работе! >> /tmp/meeting.txt");
    my_file = fopen("/tmp/meeting.txt", "r");
    fread(buffer, 4096, sizeof(char), my_file);
    sprintf(reply_msg, "cmd 250 ok \n%s\n", buffer);
    fclose(my_file);
    system("rm /tmp/meeting.txt");
    pthread_mutex_unlock(&mutex);
}

/*
void opt_recv(char *argument, char *reply_msg) {
    FILE *file;
    char buff[201];
    int s;
    data_connect("localhost", &s);
    file = fopen(argument, "r");
    if (file != NULL) {
        while (!feof(file)) {
            fread(buff, sizeof(char), 200, file);
            if (send_mes(s, buff, strlen(buff) + 1) != OK) break;
        }
        //закрыть файл и сокет
        fclose(file);
        close(s);
    }
}

void opt_put(char *argument, char *reply_msg, int msg_size) {
    char buff[201];
    int s;
    data_connect("localhost", &s);
    process_cwd(argument);
    FILE *new_file = fopen(argument, "w");
    if(new_file == NULL) {
        while(1) {
            if(msg_size == 0) break;
            receive_message(s, buff, sizeof(buff), &msg_size);
            fwrite(buff, sizeof(char), msg_size, new_file);
            fprintf(stdout, "%d\n", msg_size);
            fflush(new_file);
            memset(buff, '\0', sizeof(buff));
        }
        close(s);
        fclose(new_file);
    }
}
*/

