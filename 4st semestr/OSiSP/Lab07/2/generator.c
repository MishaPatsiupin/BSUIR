#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <inttypes.h>
#include <stdbool.h>

#define MAX_LEN 80
#define RECORDS_COUNT 10

typedef struct record_s{
    char name[MAX_LEN];
    char address[MAX_LEN];
    uint8_t semester;
}record_s;

const char *file_path = "records.txt";

int main(){
    srand(time(NULL));
    record_s array_recs[RECORDS_COUNT];
    for(int i = 0; i < RECORDS_COUNT; i++){
        sprintf(array_recs[i].name, "name_%d", i);
        sprintf(array_recs[i].address, "name_%d", i);
        array_recs[i].semester = (uint8_t)(rand() % 8 + 1);
    }
    int file_descr = open(file_path, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if(file_descr == -1){
        fprintf(stderr, "Cant open file\n");
        exit(1);
    }
    write(file_descr, &array_recs, sizeof(array_recs));
    return 0;
}