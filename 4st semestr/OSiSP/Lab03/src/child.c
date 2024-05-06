#define _GNU_SOURCE

#include <signal.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

typedef struct 
{
    int first;
    int second;
} pair;

void init_signals_handling();
void user_signal_handler(int signo);
void alarm_signal_handler(int signo);

bool can_print = false;
bool received_signal = false;
pair stats;
size_t c00 = 0;                                      
size_t c01 = 0;
size_t c10 = 0;
size_t c11 = 0;

void update_stats()
{      
    static int counter;
    switch (counter)
    {
        case 0:
        {
            stats.first = 0;
            stats.second = 0;
            counter++;
            break;
        }
        case 1:
        {
            stats.first = 1;
            stats.second = 0;
            counter++;  
            break;
        }
        case 2:
        {
            stats.first = 0;
            stats.second = 1; 
            counter++;
            break;
        }
        case 3:
        {
            stats.first = 1;
            stats.second = 1;  
            counter++;
            break;
        }
        default:
        {
            counter = 0;
            break;
        }
    }
}

int main(int argc, char *argv[])
{
    srand(time(NULL));
    init_signals_handling();
    alarm(rand() % 1 + 1);
    for(int i = 0;;i++)
    {
        sleep(1);
        update_stats();
        received_signal = false;
        if (i >= 5 && can_print)                        
        {
            alarm(0);                                   
            union sigval info;                          
            info.sival_int = getpid();
            received_signal = false;
            while (!received_signal)                    
            {                       
                sigqueue(getppid(), SIGUSR1, info);     
                sleep(10);
            }
            alarm(rand() % 1 + 1);                      
            if (!can_print)                             
            {
                i = 0;
                can_print = true;
                continue;
            }
            printf("-------------------------------------------\n");
            printf("ppid - %5d\npid  - %5d\n", (int) getppid(), (int) getpid());
            printf("00   - %5d\n01   - %5d\n10   - %5d\n11   - %5d\n", (int) c00, (int) c01, (int) c10, (int) c11);
            i = 0;
            sigqueue(getppid(), SIGUSR2, info);             
        }
    }
    return 0;
}

void init_signals_handling()
{
    struct sigaction action = {0};
    sigset_t set;                                       
    sigemptyset(&set);
    sigaddset(&set, SIGALRM);
    sigaddset(&set, SIGUSR1);
    sigaddset(&set, SIGUSR2);

    action.sa_flags = 0;                               
    action.sa_mask = set;      	                        
    action.sa_handler = user_signal_handler;            

    sigaction(SIGUSR1, &action, NULL);
    sigaction(SIGUSR2, &action, NULL);

    action.sa_handler = alarm_signal_handler;          
    sigaction(SIGALRM, &action, NULL);         
}

void user_signal_handler(int signo)
{
    if(signo == SIGUSR1)
    {
        can_print = false;
        received_signal = true;
    }
    else if(signo == SIGUSR2)
    {
        can_print = true;
        received_signal = true;
    }
}

void alarm_signal_handler(int signo)
{
    if      (stats.first == 0 && stats.second == 0)  c00++;
    else if (stats.first == 1 && stats.second == 0)  c01++;
    else if (stats.first == 0 && stats.second == 1)  c10++;
    else if (stats.first == 1 && stats.second == 1)  c11++;
    alarm(rand() % 1 + 1);
}