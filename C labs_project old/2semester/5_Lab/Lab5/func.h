#ifndef INC_5_1_FUNCTIONS_H
#define INC_5_1_FUNCTIONS_H
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define HASH_SIZE 5381
#define MAX_LENGTH 256
#define CACHE_SIZE 3
#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_CYAN "\x1b[36m"
#define ANSI_COLOR_RESET "\x1b[0m"

typedef struct CacheEntry
{
    char domain[MAX_LENGTH];
    char ip[MAX_LENGTH];
    struct CacheEntry* prev;
    struct CacheEntry* next;
} CacheEntry;

typedef struct Cache
{
    CacheEntry* entries[CACHE_SIZE];
    CacheEntry* head;
    CacheEntry* tail;
    int size;
} Cache;

void find_ip_by_domain(Cache* cache);
int input_value();
unsigned int hash(const char* str);
void init_cache(Cache* cache);
int is_valid_cname(const char* cname);
void move_to_front(Cache* cache, CacheEntry* entry);
char* find_in_cache(Cache* cache, const char* domain);
void remove_entry_from_cache(Cache* cache, CacheEntry* entry_to_remove);
void add_to_cache(Cache* cache, const char* domain, const char* ip);
void get_domain(char* domain);
FILE* open_domain_file();
char* find_ip_address(FILE* file, Cache* cache, char* domain);
void show_cache(Cache* cache);
int is_valid_ip(const char* ip);

int is_koliz_record(const char* domain, const char* type, const char* value);

void find_domains_by_ip();
int is_valid_domain(const char* domain);
void add_record();
void free_cache(Cache* cache);
void find_and_print_cname_records(FILE* file, const char* domain);

#endif