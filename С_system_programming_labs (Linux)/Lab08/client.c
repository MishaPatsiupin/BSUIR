#define _GNU_SOURCE
#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

#define h_addr h_addr_list[0]

#define SERVER_FTP_PORT 1231
#define DATA_CONNECTION_PORT SERVER_FTP_PORT +1

#define OK 0                                        //коды ошибок
#define ER_INVALID_HOST_NAME -1
#define ER_CREATE_SOCKET_FAILED -2
#define ER_BIND_FAILED -3
#define ER_CONNECT_FAILED -4
#define ER_SEND_FAILED -5
#define ER_RECEIVE_FAILED -6

int make_connection(char *, int *);
int send_message(int, char *, int);
int receive_message(int, char *, int, int *);
void print_message(char *, int);

char user_input[1024];	    //ввод пользователя
char server_reply[4096];    //ответ сервера
int is_file = 0;
FILE *my_file;

int main()
{
    int msg_size;	        //размер сообщения
    int control_socket;	    //номер сокета
    int status = OK;

    printf("FTP-Client начал работу.\n");

    do
    {
        printf("\nВведите IP-адрес сервера (quit для выхода): ");
        scanf("%s", user_input);
        getc(stdin);

        if (strcmp(user_input,"quit") == 0)
            return(OK);

        printf("Подключение к серверу...\n");
        status = make_connection(user_input, &control_socket);  		//переделать с таймером
    } while (status != OK);

    status = receive_message(control_socket, server_reply,
                             sizeof(server_reply), &msg_size);
    if(status != OK)
        fprintf(stderr, "Ошибка при получении ответа от сервера!\n");
    else
        print_message(server_reply, msg_size);
    while (true)
    {
        printf("FTP-Client>");
        if(!is_file) {
            fgets(user_input, 1024, stdin);
            if(user_input[0] == '@'){
                char buf[1024];
                strcpy(buf, user_input + 1);
                if((my_file = fopen("file.txt", "r")) == NULL){
                    fprintf(stdout, "No such file\n");
                }
                is_file = 1;
                continue;
            }
        }
        else{
            memset(user_input, 0, 1024);
            sleep(2);
            if(!fgets(user_input, 1024, my_file)){
                fclose(my_file);
                is_file = 0;
                continue;
            }
            fprintf(stdout, "%s", user_input);
        }
        if (strlen(user_input) <= 1)
            continue;
        status = send_message(control_socket, user_input, strlen(user_input) + 1);
        if(status != OK)
        {
            fprintf(stderr, "Ошибка при посылке команды!\n");
            continue;
        }

        status = receive_message(control_socket, server_reply,
                                 sizeof(server_reply), &msg_size);
        if(status != OK)
            fprintf(stderr, "Ошибка при получении ответа от сервера!\n");
        else
            print_message(server_reply, msg_size);

        if (strcmp(user_input, "quit\n") == 0)
            break;
    }

    close(control_socket);

    printf("FTP-Client завершил работу.\n");

    return (status);
}

int make_connection (char *server_name, int *s)
{
    int sock;	                            		//номер сокета
    struct sockaddr_in client_address;  			//IP клиента
    struct sockaddr_in server_address;	    		//IP сервера
    struct hostent	   *server_IP_structure;

    if((server_IP_structure = gethostbyname(server_name)) == NULL)     //получение данных о сервера
    {
        printf("Сервер: %s не найден.\n", server_name);
        return (ER_INVALID_HOST_NAME);
    }

    if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)                //создание сокета
    {
        fprintf(stderr, "Не удалось создать сокет!");
        return (ER_CREATE_SOCKET_FAILED);
    }

    memset((char *) &client_address, 0, sizeof(client_address));    //инициализация памяти

    client_address.sin_family = AF_INET;
    client_address.sin_addr.s_addr = htonl(INADDR_ANY);     //INADDR_ANY = 0 означает, что IP адрес клиента будет определён системой
    client_address.sin_port = 0;                            //0 значит, что порт будет определён системой

    int flag = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(int));
    if(bind(sock, (struct sockaddr *)&client_address, sizeof(client_address)) < 0)  //связать сокет с IP и портом клиента
    {
        fprintf(stderr, "Не удалось связать сокет с IP и портом клиента");
        close(sock);                                                   //закрытие сокета
        return(ER_BIND_FAILED);
    }

    memset((char *) &server_address, 0, sizeof(server_address)); 	//инициализация памяти

    server_address.sin_family = AF_INET;
    memcpy((char *) &server_address.sin_addr, server_IP_structure->h_addr,
           server_IP_structure->h_length);
    server_address.sin_port = htons(SERVER_FTP_PORT);

    if (connect(sock, (struct sockaddr *) &server_address, sizeof(server_address)) < 0) //подключение к серверу
    {
        fprintf(stderr, "Не удалось подключиться к серверу");
        close(sock); 	                        //закрытие сокета
        return(ER_CONNECT_FAILED);
    }

    *s = sock; //сохранение номера сокета

    return(OK);
}

int send_message(int s, char *msg, int msg_size)
{
    if((send(s, msg, msg_size, 0)) < 0)             //пересылка данных через сокет
    {
        fprintf(stderr, "Не удалось отправить команду!");
        return(ER_SEND_FAILED);
    }

    return(OK);
}

int receive_message(int s, char *buffer, int bufferSize, int *msg_size)
{
    *msg_size = recv(s, buffer, bufferSize, 0);         //получение сообщения

    if(*msg_size <= 0)
    {
        fprintf(stderr, "Не удалось получить сообщение!");
        return(ER_RECEIVE_FAILED);
    }

    return (OK);
}

void print_message(char *message, int msg_size)
{
    int i = 0;
    char buffer[4];

    strncpy(buffer, message, 3);

    if (strcmp(buffer, "cmd") == 0)
        printf("LOG: ");

    for(i = 0; i < msg_size; i++)                      //вывод сообщения
        printf("%c", message[i]);
    printf("\n");
}
