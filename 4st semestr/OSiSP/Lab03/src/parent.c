#define _GNU_SOURCE
#define CAPACITY 8

#include <signal.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

typedef struct process_info
{   
    pid_t pid;
    bool is_stopped;
    char name[CAPACITY*2];
} process_info;

size_t child_processes_size = 0;
size_t child_processes_capacity = CAPACITY;
process_info *child_processes = NULL;
char child_name[CAPACITY] = "./child";

int input_option(char *option, int *option_index);
int str_to_int(char* str);
void allocate_child_processes();
void init_child_process(pid_t pid);
void case_plus();
void case_minus();
void case_l();
void case_k();
void case_s(int index);
void case_g(int index);
void case_p(int index);
void case_q();
process_info last_child();
void delete_child_process();
void delete_all_child_processes();
void init_signals_handling();
void signal_handler(int signo, siginfo_t *info, void *context);
char *find_process_name_by_pid(pid_t pid);
int find_process_index_by_pid(pid_t pid);
void print_menu();

int main(int argc, char *argv[])
{    
    init_signals_handling();
    allocate_child_processes();
    print_menu();
    while(true)
    {
        char option;
        int option_index;
        if(option != 'g') printf("-------------------------------------------\n");
        fflush(stdin);
        if(input_option(&option, &option_index) == -1) continue;
        if(option != 'm') printf("-------------------------------------------\n");
        switch (option)
        {
            case 'm': {
                print_menu();
                break;
            }
            case '+': {
                case_plus();
                break;
            }
            case '-': {
                case_minus();
                break;
            }
            case 'l': {
                case_l();
                break;
            }
            case 'k': {
                case_k();
                break;
            }
            case 's': {
                case_s(option_index);
                break;
            }
            case 'g': {
                case_g(option_index);
                break;
            }
            case 'p': {
                case_p(option_index);
                break;
            }
            case 'q': {
                case_q();
            }
        }
    }
    return 0;
}

int find_process_index_by_pid(pid_t pid)
{
    for(int i = 0; i < (int) child_processes_size; i++)
    {
        if(child_processes[i].pid == pid) return i;
    }
    return -1;
}

void print_menu()
{
    printf("-------------------------------------------\n");
    printf("input one of chars: + - m l k s g q\n");
    printf("or input s<num> g<num> p<num>,\n");
    printf("where num - index of process\n");
}

void init_signals_handling()
{
    struct sigaction action = {0};
    sigset_t set;                              
    sigemptyset(&set);
    sigaddset(&set, SIGALRM);
    sigaddset(&set, SIGUSR1);
    sigaddset(&set, SIGUSR2);

    action.sa_flags = SA_SIGINFO;              
    action.sa_mask = set;      	               
    action.sa_sigaction = signal_handler;      

    sigaction(SIGALRM, &action, NULL);     
    sigaction(SIGUSR1, &action, NULL);
    sigaction(SIGUSR2, &action, NULL);
}

void signal_handler(int signo, siginfo_t *info, void *context)
{
    if(signo == SIGUSR1)
    {
        pid_t pid = (*info).si_value.sival_int;
        int index = find_process_index_by_pid(pid);
        if(child_processes[index].is_stopped) kill(pid, SIGUSR1);   
        else kill(pid, SIGUSR2);                                   
    }
    else if(signo == SIGUSR2)
    {
        pid_t pid = (*info).si_value.sival_int;
        char *name = find_process_name_by_pid(pid);
        printf("%s with pid = %d has ended his output\n", name, pid); 
    }
    else if(signo == SIGALRM) case_g(-1);
}

char *find_process_name_by_pid(pid_t pid)
{
    for(int i = 0; i < (int) child_processes_size; i++)
    {
        if(child_processes[i].pid == pid) return child_processes[i].name;
    }
    return NULL;
}

void allocate_child_processes()
{
    child_processes = (process_info *)calloc(child_processes_capacity, sizeof(process_info));
    if(!child_processes)
    {
        exit(errno);
    }
}

void init_child_process(pid_t pid)
{
    sprintf(child_processes[child_processes_size - 1].name, "C_%02d", (int) child_processes_size - 1);
    child_processes[child_processes_size - 1].pid = pid;
    child_processes[child_processes_size - 1].is_stopped = true;
}

process_info last_child()
{
    return child_processes[child_processes_size - 1];
}

void delete_child_process(pid_t pid)
{
    kill(pid, SIGTERM);
    printf("%s with pid = %d was deleted\n", last_child().name, last_child().pid);
    child_processes_size--;
}

void delete_all_child_processes()
{
    while(child_processes_size) delete_child_process(last_child().pid);
    printf("all child processes were successfully deleted\n");
}

void case_plus()
{
    pid_t pid = fork();
    if (pid == -1) 
    {
        fprintf(stderr, "%s, error code: %d\n", strerror(errno), errno);
        exit(errno);
    }
    if (pid == 0) execl(child_name, child_name, NULL);
    else
    {
        child_processes_size++;
        if(child_processes_size >= child_processes_capacity)
        {
            child_processes_capacity *= 2;
            process_info *tmp = (process_info *)realloc(child_processes, child_processes_capacity);
            if(!tmp)
            {
                exit(errno);
            }
            child_processes = tmp;
            tmp = NULL;
        }
        init_child_process(pid);
        printf("%s with pid = %d was created\n", last_child().name, pid);
        printf("size of child_processes - %d\n", (int) child_processes_size);
    }
}

void case_minus()
{
    delete_child_process(last_child().pid);
    printf("size of child_processes - %d\n", (int) child_processes_size);
}

void case_l()
{
    printf("parent:\nP    with pid = %d\n", (int) getpid());
    printf("children :\n");
    for (int i = 0; i < (int) child_processes_size; i++) 
    {
        printf("%s with pid = %d is ", child_processes[i].name, child_processes[i].pid);        
        if (child_processes[i].is_stopped) printf("stopped\n");
        else printf("running\n");
    }
}

void case_k()
{
    delete_all_child_processes();
}

void case_s(int index)
{
    if(index != -1)       
    {
        kill(child_processes[index].pid, SIGUSR1);
        child_processes[index].is_stopped = true;
        printf("%s with pid = %d was stopped\n", child_processes[index].name, child_processes[index].pid);
        return;
    }
    for(int i = 0; i < (int) child_processes_size; i++)
    {
        kill(child_processes[i].pid, SIGUSR1);
        child_processes[i].is_stopped = true;
        printf("%s with pid = %d was stopped\n", child_processes[i].name, child_processes[i].pid);
    }
}

void case_g(int index)
{
    alarm(0);               
    if(index != -1)        
    {
        kill(child_processes[index].pid, SIGUSR2);
        child_processes[index].is_stopped = false;
        printf("%s with pid = %d is running now\n", child_processes[index].name, child_processes[index].pid);
        return;
    }
    for(int i = 0; i < (int) child_processes_size; i++)
    {
        kill(child_processes[i].pid, SIGUSR2);
        child_processes[i].is_stopped = false;
        printf("%s with pid = %d is running now\n", child_processes[i].name, child_processes[i].pid);
    }
}

void case_p(int index)
{
    case_s(-1);     
    case_g(index);  
    alarm(5);       
}

void case_q()
{
    case_k();
    if(child_processes) free(child_processes);
    child_processes = NULL;
    exit(0);
}

int input_option(char *option, int *option_index)
{
    int tmp_index = -1;                
    char buf[CAPACITY];
    fflush(stdin);
    fgets(buf, sizeof(buf) / sizeof(*buf), stdin);
    char tmp_option = buf[0];
    if ((strstr(buf, "<") && strstr(buf, ">")) && (tmp_option == 's' || tmp_option == 'g' || tmp_option == 'p'))     
    {
        if((tmp_index = str_to_int(buf)) == -1)
        {
            fprintf(stderr, "error, input string in format: o<num>\no - option, num - index of process\n");
            return -1;
        }
        if(tmp_index >= (int) child_processes_size)
        {
            fprintf(stderr, "error, index %d is greater or equal than child_processes_size =% d\n", 
            tmp_index, (int) child_processes_size);
            return -1;
        }
    }
    if(!(tmp_option == '+' || tmp_option == '-' || tmp_option == 'm' || tmp_option == 'l' || tmp_option == 'k' || tmp_option == 's' 
        || tmp_option == 'g' || tmp_option == 'p' || tmp_option == 'q'))
    {
        return -1;
    }
    if(tmp_index == -1 && child_processes_size == 0 && (tmp_option != '+' && tmp_option != 'q' && tmp_option != 'l' && tmp_option != 'm')) 
    {
        fprintf(stderr, "error, there are no child_processes\ncreate them with + option\n");
        return -1;
    }
    if(tmp_index == -1 && tmp_option == 'p')
    {
        fprintf(stderr, "error, option p can be only chosen with num\n");
        return -1;
    }
    *option = tmp_option;
    *option_index = tmp_index;
    return 0;
}

int str_to_int(char *str)
{         
    int length = 0;
    char num[CAPACITY];
    int str_len = strlen(str);
    for(int i = 0; i < str_len; i++)
    {
        if (str[i] >= '0' && str[i] <= '9')
        {
            length++;
            num[length - 1] = str[i];           
        }
    }
    if(length && num[0] == '0') return 0;
    if(length) return atoi(num);               
    return -1;
}