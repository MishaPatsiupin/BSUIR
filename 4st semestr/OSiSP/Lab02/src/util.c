#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char getch()
{
    struct termios new_settings, stored_settings;
   
    tcgetattr(0,&stored_settings);
   
    new_settings = stored_settings;
     
    new_settings.c_lflag &= ~(ICANON | ECHO);
    new_settings.c_cc[VTIME] = 0;
    new_settings.c_cc[VMIN] = 1;
    
    tcsetattr(0, TCSANOW, &new_settings);
    char result = getchar();
    tcsetattr(0, TCSANOW, &stored_settings);
    return result;
}

char *scanEnv(char **env, char *key) {
    for (char **envp = env; *envp; envp++) {
        char *buff = malloc((strlen(*envp) + 1) * sizeof(char));
        strcpy(buff, *envp);

        char *currentKey = buff;
        char *value = strchr(buff, '=') + 1;
        value[-1] = 0;

        if(!strcmp(currentKey, key)) {
            static char result[4096];
            strcpy(result, value);
            free(buff);

            return result;
        }

        free(buff);
    }
    return NULL;
}