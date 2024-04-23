#include <stdio.h>
#include <sys/types.h>
#include "util.h"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv, char **envp) {
    char *action = argv[2];

    pid_t pid = getpid();
	pid_t ppid = getppid();

	printf("My PID: %d, parent PID: %d\n", pid, ppid);
    printf("My name: %s\n", argv[0]);

    FILE *file = fopen(argv[1], "r");

    if(!file) {
        printf("File not found\n");
    }

    while(!feof(file)) {
        char key[100];
        
        fgets(key, 100, file);

        key[strlen(key) - 1] = 0;

        if(strlen(key) == 0)
            break;

        char *value;

        switch (*action)
        {
        case '+':
            value = getenv(key);
            break;
        case '*':
            value = scanEnv(envp, key);
            break;
        case '&':
            value = scanEnv(__environ, key);//
        }

        printf("%s=%s\n", key, value);
    }
    fclose(file);
}