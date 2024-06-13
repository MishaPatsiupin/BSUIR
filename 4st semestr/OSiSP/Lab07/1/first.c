#define _XOPEN_SOURCE 700
#define _GNU_SOURCE
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stddef.h>
#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <string.h>
#include <sys/mman.h>

#define DATA_MAX (((256 + 3) / 4) * 4)
#define MSG_MAX 4096
#define MSG_MAX_START 10
#define PROC_MAX 1024

typedef struct{
    int type;
    int hash;
    int size;
    char data[DATA_MAX];
}mes; //само сообщение 

typedef struct{
    mes buf[MSG_MAX];
    int curr_max_count;
    int head;
    int tail;
    int counter;
    int injected;
    int extracted;
}message_queue; //очередь сообщений

message_queue *queue;
pthread_mutex_t mutex;

pthread_cond_t condp = PTHREAD_COND_INITIALIZER;
pthread_cond_t condc = PTHREAD_COND_INITIALIZER;

pthread_t  prods[PROC_MAX];
int prod_num;

pthread_t  cons[PROC_MAX];
int cons_num;

mes message;
int counter;

bool is_deleted_prod[MSG_MAX];
bool is_deleted_cons[MSG_MAX];


void init_queue(){
    queue->extracted = 0;
    queue->injected = 0;
    queue->counter = 0;
    queue->head = 0;
    queue->tail = 0;
    queue->curr_max_count = MSG_MAX_START;
    memset(queue->buf, 0, sizeof(queue->buf));
}

void sig_handler(int signal){
}

int hash(mes* msg)
{
    unsigned long hash = 5381;
    for (int i = 0; i < msg->size + 4; i++)
        hash = ((hash << 5) + hash) + i;
    return (int) hash;
}

void prod_mes(mes* msg){
    int val = rand() % 257;
    if (val == 256)
        msg->type = -1;
    else{
        msg->type = 0;
        msg->size = val;
    }
    for (int i = 0; i < val; ++i)
        msg->data[i] = (char) (rand() % 256);
    msg->hash = 0;
    msg->hash = hash(msg);
}

void consume_mes(mes* msg){
    int mes_hash = msg->hash;
    msg->hash = 0;
    int check = hash(msg);
    if(check != mes_hash){
        fprintf(stderr, "Check sum (= %d) not equal msg_hash (= %d)\n",check, mes_hash);
    }
    msg->hash = mes_hash;
}

int put_msg(mes* msg){
    if(queue->counter == queue->curr_max_count){
        fprintf(stdout, "Buf is full!\n");
        exit(1);
    }
    if(queue->tail == queue->curr_max_count)
        queue->tail = 0;
    queue->buf[queue->tail] = *msg;
    queue->tail++;
    queue->counter++;
    return ++queue->injected;
}

int get_msg(mes* msg){
    if(queue->counter == 0){
        fprintf(stderr, "There are no messages in queue\n");
        exit(1);
    }
    if(queue->head == queue->curr_max_count)
        queue->head = 0;
    queue->buf[queue->head] = *msg;
    queue->head++;
    queue->counter--;
    return ++queue->extracted;
}

void *prod_handler(){
    int ind = prod_num - 1;
    while(!is_deleted_prod[ind]){
        sleep(3);
        prod_mes(&message);
        pthread_mutex_lock(&mutex);
        if(queue->counter == queue->curr_max_count){
            fprintf(stderr, "Queue is full, prod with id = %lu is waiting\n", pthread_self());
            pthread_cond_wait(&condp, &mutex);
        }else{
            counter = put_msg(&message);
            pthread_cond_broadcast(&condc);
            fprintf(stdout, "Id = %lu\nMessage = %X\nMessages injected counter = %d\ncounter = %d max = %d \n",
                    pthread_self(), message.hash, counter, queue->counter, queue->curr_max_count);
        }
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void create_prod() {
    if(prod_num == PROC_MAX - 1){
        fprintf(stderr, "Max count of prods");
        return;
    }
    int res = pthread_create(&prods[prod_num], NULL, prod_handler, NULL);
    if(res){
        fprintf(stderr, "Can't create a prod\n");
        return;
    }
    is_deleted_prod[prod_num] = false;
    prod_num++;
}

void* cons_handler(){
    int ind = cons_num - 1;
    while(!is_deleted_cons[ind]){
        sleep(3);
        pthread_mutex_lock(&mutex);
        if(queue->counter == 0){
            fprintf(stderr, "Queue is empty, cons with id = %lu is waiting\n", pthread_self());
            pthread_cond_wait(&condc, &mutex);
        }else{
            counter = get_msg(&message);
            consume_mes(&message);
            pthread_cond_broadcast(&condp);
            fprintf(stdout, "Id = %lu\nMessage = %X\nMessages extracted counter = %d\ncounter = %d max = %d \n",
                    pthread_self(), message.hash, counter, queue->counter, queue->curr_max_count);
        }
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void create_cons() {
    if(prod_num == PROC_MAX - 1){
        fprintf(stderr, "Max count of prods");
        return;
    }
    int res = pthread_create(&cons[cons_num], NULL, cons_handler, NULL);
    if(res){
        fprintf(stderr, "Can't create a prod\n");
        return;
    }
    is_deleted_cons[cons_num] = false;
    cons_num++;
}

void del_prod(){
    if(prod_num == 0){
        fprintf(stderr, "No prods to delete\n");
        return;
    }
    prod_num--;
    is_deleted_prod[prod_num] = true;
    pthread_kill(prods[prod_num] ,SIGUSR1);
    usleep(30000);
    pthread_cond_broadcast(&condp);
    pthread_join(prods[prod_num], NULL);
}

void del_cons(){
    if(cons_num == 0){
        fprintf(stderr, "No cons to delete\n");
        return;
    }
    cons_num--;
    is_deleted_cons[cons_num] = true;
    pthread_kill(cons[cons_num] ,SIGUSR1);
    usleep(30000);
    pthread_cond_broadcast(&condc);
    pthread_join(cons[cons_num], NULL);
}

void decrease_buf(){
    if(queue->curr_max_count != 0){
        if(queue->counter == queue->curr_max_count) {
            fprintf(stderr, "Cant decrease buff. Consume the message first\n");
        }
        queue->curr_max_count--;
    }
}

void increase_buf(){
    if(queue->curr_max_count != MSG_MAX)
        queue->curr_max_count++;
}

void atexit_handler(){
    for(int i = 0; i < prod_num; i++) del_prod();
    for(int i = 0; i < prod_num; i++) del_cons();

    pthread_cond_destroy(&condp);
    pthread_cond_destroy(&condc);

    int res = pthread_mutex_destroy(&mutex);
    if(res){
        fprintf(stderr, "Cant destroy mutex %d\n", errno);
        exit(1);
    }

    if(shm_unlink("queue")){
        fprintf(stderr, "shm_unlink\n");
        abort();
    }
}

void init_threads(){
    atexit(atexit_handler);
    int file_descr = shm_open("queue", (O_RDWR | O_CREAT | O_TRUNC),
                              (S_IRUSR | S_IWUSR));
    if(file_descr < 0){
        fprintf(stderr, "shm_open");
        exit(1);
    }
    if(ftruncate(file_descr, sizeof(message_queue))){
        fprintf(stderr, "ftruncate");
        exit(1);
    }
    void* ptr = mmap(NULL, sizeof(message_queue),
                     PROT_READ | PROT_WRITE, MAP_SHARED, file_descr, 0);
    if(ptr == MAP_FAILED){
        fprintf(stderr, "mmap");
        exit(1);
    }
    queue = (message_queue *) ptr;
    init_queue();
    int res = pthread_mutex_init(&mutex, NULL);
    if(res){
        fprintf(stderr, "Cant init mutex");
        exit(1);
    }
    pthread_cond_init(&condp, NULL);
    pthread_cond_init(&condc, NULL);
    if(close(file_descr)){
        fprintf(stderr, "Cant close file");
        exit(1);
    }
}


int main(){
    signal(SIGUSR1, sig_handler);
    init_threads();
    fprintf(stdout, "p - create producer\n");
    fprintf(stdout, "c - create consumer\n");
    fprintf(stdout, "d - delete producer\n");
    fprintf(stdout, "r - delete consumer\n");
    fprintf(stdout, "l - show processes\n");
    fprintf(stdout, "+ - increase buf\n");
    fprintf(stdout, "- - decrease buf\n");
    fprintf(stdout, "q - quit program\n");
    while(true){
        switch(getchar()){
            case 'p' : { create_prod(); break; }
            case 'c' : { create_cons(); break; }
            case 'd' : { del_prod(); break; }
            case 'r' : { del_cons(); break; }
            case '+' : { increase_buf(); break; }
            case '-' : { decrease_buf(); break; }
            case 'l' : {
                for (int i = 0; i < prod_num; i++)
                    fprintf(stdout,"Producer %d: %lu\n", i + 1, prods[i]);
                fprintf(stdout,"\n");
                for (int i = 0; i < cons_num; i++)
                    printf("Consumer %d: %lu\n", i + 1, cons[i]);
                fprintf(stdout,"\n");
                break; }
            case 'q' : exit(0);
            default : break;
        }
    }

}
