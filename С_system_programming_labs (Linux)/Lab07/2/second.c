#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <inttypes.h>
#include <stdbool.h>

#define MAX_LEN 80
#define RECORDS_COUNT 10

typedef struct record_s{
    char name[MAX_LEN];
    char address[MAX_LEN];
    uint8_t semester;
}record_s;

int fd;
struct flock lock;
const char *file_path = "records.txt";
record_s cur_rec = {0};
record_s work_rec = {0};
int rec_no = -1;

void init();
void print_record(int rec_no);
void get_record(int rec_no, record_s *record);
void put_record(int rec_no, record_s *record);
void save_record();
void menu();
void input_option(char *option);
void input_int(int *rec_no, int lower, int higher);
void edit_record();
void print_current_record();
void my_lock();


void menu()
{
    printf("menu:\n");
    printf("l - list all records\n");
    printf("g - get a record\n");
    printf("e - edit a record\n");
    printf("s - save the last read/modified record\n");
    printf("q - quit the program\n");
}

void init(){
    fd = open(file_path, O_RDWR);
    if(fd == -1){
        fprintf(stderr, "Cant open file\n");
        exit(1);
    }
}

void input_option(char* option){
    while(true){
        scanf("%c", option);
        if(*option == 'l' || *option == 'g' || *option == 'e'
           || *option == 's' || *option == 'q' || *option == '1' || *option == '2') break;
    }
}

void input_int(int *rec_no, int lower, int higher)
{
    do{
        if(!scanf("%d", rec_no)){
            fprintf(stderr, "input number\n");
            (void) getchar();
            continue;
        }
        if(*rec_no < lower || *rec_no > higher){
            printf("input number from %d to %d\n", lower, higher);
            continue;
        }
        (void) getchar();
        break;
    }while(true);
}

void print_record(int rec_no)
{
    get_record(rec_no, &cur_rec);
    printf("rec_no: %d\nname: %s\naddress: %s\nsemester: %d\n",
           rec_no, cur_rec.name,
           cur_rec.address, cur_rec.semester);
}

void get_record(int rec_no, record_s *record)
{
    off_t offset = rec_no * sizeof(*record);
    lseek(fd, offset, SEEK_SET);
    if(read(fd, record, sizeof(*record)) == -1)
    {
        fprintf(stderr, "read");
        exit(1);
    }
}

void put_record(int rec_no, record_s *record)
{
    off_t offset = rec_no * sizeof(*record);
    lseek(fd, offset, SEEK_SET);
    if(write(fd, record, sizeof(*record)) == -1)
    {
        fprintf(stderr,"write");
        exit(1);
    }
}

void my_lock()
{
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = rec_no * sizeof(record_s);
    lock.l_len = sizeof(record_s);
    while (fcntl(fd, F_SETLK, &lock) == -1)
    {
        if (errno == EAGAIN)
        {
            fprintf(stdout, "record with index = %d is locked. waiting...\n", rec_no);
            sleep(1);
        }
        else
        {
            fprintf(stderr,"fcntl");
            exit(errno);
        }
    }
}

void save_record()
{
    char opt;
    bool is_stop = false;
    record_s rec_sav = {0};
    get_record(rec_no, &rec_sav);
        if(memcmp(&rec_sav, &cur_rec, sizeof(rec_sav)) != 0){
            fprintf(stdout,"Record %d was modified\n", rec_no);
            my_lock();
            if(memcmp(&work_rec, &rec_sav, sizeof(rec_sav))){
                while(!is_stop) {
                    fprintf(stdout, "Record already was changed by the other process\n"
                                    "1.Accept current changes\n2.Make new changes\n");
                    input_option(&opt);
                    switch (opt) {
                        case '1': {
                            fprintf(stdout,"1\n");
                            get_record(rec_no, &cur_rec);
                            is_stop = true;
                            break;
                        }
                        case '2': {
                            fprintf(stdout,"2\n");
                            get_record(rec_no, &cur_rec);
                            edit_record();
                            put_record(rec_no, &cur_rec);
                            is_stop = true;
                            break;
                        }
                        default:
                            continue;
                    }
                }
                lock.l_type = F_UNLCK;
                fcntl(fd, F_SETLK, &lock);
                return;
            }
            put_record(rec_no, &cur_rec);
            lock.l_type = F_UNLCK;
            fcntl(fd, F_SETLK, &lock);
            fprintf(stdout, "Record was saved\n");
            return;
        }
        fprintf(stdout,"record with rec_no = %d wasn't modified\n", rec_no);
}

void print_current_record()
{
    printf("\nCurrent record:\n");
    printf("rec_no: %d\nname: %s\naddress: %s\nsemester: %d\n",
           rec_no, cur_rec.name,
           cur_rec.address, cur_rec.semester);
}

void edit_record()
{
    int option;
    while(true)
    {
        print_current_record();
        printf("input option:\n0 - exit\n1 - edit name\n2 - edit " \
               "address\n3 - edit semester\n");
        input_int(&option, 0, 3);
        switch(option)
        {
            case 1:
            {
                printf("new name:\n");
                fgets(cur_rec.name, MAX_LEN, stdin);
                cur_rec.name[strlen(cur_rec.name) - 1] = '\0';
                break;
            }
            case 2:
            {
                printf("new address:\n");
                fgets(cur_rec.address, MAX_LEN, stdin);
                cur_rec.address[strlen(cur_rec.address) - 1] = '\0';
                break;
            }
            case 3:
            {
                printf("new semester:\n");
                input_int((int *)&cur_rec.semester, 1, 8);
                break;
            }
            case 0:
            {
                get_record(rec_no, &work_rec);
                return;
            }
        }
    }
}

int main(){
    init();
    menu();
    char opt;
    while(true){
        if(rec_no != -1) print_current_record();
        input_option(&opt);
        switch (opt) {
            case 'l' :{
                fprintf(stdout, "Records: \n");
                for(rec_no = 0; rec_no < RECORDS_COUNT; rec_no++)
                    print_record(rec_no);
                rec_no--;
                break;
            }
            case 'g' :{
                fprintf(stdout, "Enter record index : \n");
                input_int(&rec_no, 0, RECORDS_COUNT - 1);
                get_record(rec_no, &cur_rec);
                break;
            }
            case 's' :{
                if(rec_no == -1)
                {
                    printf("read/modify record first\n");
                    break;
                }
                save_record();
                break;
            }
            case 'e' :{
                if(rec_no == -1)
                {
                    printf("Get record first\n");
                    break;
                }
                edit_record();
                break;
            }
            case 'q' :{
                printf("Program ended \n");
                close(fd);
                return 0;
            }
            default :
                continue;
        }
    }
}