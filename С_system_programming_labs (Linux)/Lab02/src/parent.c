#define _XOPEN_SOURCE 500

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <locale.h>
#include <sys/wait.h>
#include "util.h"

void makeFork(char *path, char **argv, char **envp) {
	pid_t child = fork();
	if (child == 0) 
		if(execve(path, argv, envp)) {
			perror("Execve error");
			exit(1);
		}

	wait(NULL);
}

void sort(char **arr) {
	for(char **a = arr; *a; a++) {
		for(char **b = a + 1; *b; b++) {
			if(strcmp(*a, *b) > 0) {
				char *temp = *a;
				*a = *b;
				*b = temp;
			}
		}
	}
}

int main(int argc, char **argv, char **envp) {
	if (argc != 2) {
		printf("Use parent <child_path>\n");
		return 0;
	}
	
	for(char **env = envp; *env; env++)
		printf("%s\n", *env);

	int processCounter = 0;

	while (1) {
		char action = getch();

		char childAction[2] = {action, 0};
		char *childArgv[4] = {NULL, argv[1], childAction, NULL};

		char childPath[4096];
		char *childDir;

		switch (action) {
		case 'q':
			return 0;
		case '+':
			childDir = getenv("CHILD_PATH");
			break;
		case '*':
			childDir = scanEnv(envp, "CHILD_PATH");
			break;
		case '&':
			childDir = scanEnv(__environ, "CHILD_PATH");
			break;
		default:
			printf("Unknown action: %c\n", action);
			continue;
		}

		if (!childDir) {
			printf("CHILD_PATH not found.\n");
			exit(-1);
		}

		strcpy(childPath, childDir);
		strcat(childPath, "child");
		
		char childName[sizeof("child_00")];
		sprintf(childName, "child_%02d", abs(processCounter++) % 100);
		childArgv[0] = childName;

		printf("Staring %s\n", childName);
		makeFork(childPath, childArgv, envp);
	}
}
