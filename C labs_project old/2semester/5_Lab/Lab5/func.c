#include "func.h"


int input_value()
{
    int value;
    while (!scanf("%d", &value) || getchar() != '\n' || value > 4 || value < 0)
    {
        system("cls");
        printf("\nОшибка ввода! Повторите попытку: ");
        rewind(stdin);
    }
    return value;
}

unsigned int hash(const char* str)
{
    unsigned int hash = HASH_SIZE;
    for (int i = 0; str[i] != '\0'; i++)
    {
        hash = 33 * hash + str[i];
    }
    return hash % CACHE_SIZE;
}

void init_cache(Cache* cache)
{
    for (int i = 0; i < CACHE_SIZE; i++)
    {
        cache->entries[i] = NULL;
    }
    cache->head = NULL;
    cache->tail = NULL;
    cache->size = 0;
}

int is_valid_cname(const char* cname)
{
    FILE* file = fopen("‪C:/Users/misha/Desktop/test_l5.txt", "r");
    if (file == NULL)
    {
        return 0;
    }
    char line[MAX_LENGTH];
    while (fgets(line, MAX_LENGTH, file) != NULL)
    {
        char name[MAX_LENGTH];
        char type[MAX_LENGTH];
        sscanf(line, "%s %*s %s", name, type);
        if (strcmp(name, cname) == 0 && strcmp(type, "A") == 0)
        {
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}

void move_to_front(Cache* cache, CacheEntry* entry)
{
    if (entry == cache->head)
    {
        return;
    }
    if (entry == cache->tail)
    {
        if (cache->tail->prev != NULL)
        {
            cache->tail = entry->prev;
            cache->tail->next = NULL;
        }
        else
        {
            cache->tail = NULL;
        }
    }
    else
    {
        entry->prev->next = entry->next;
        entry->next->prev = entry->prev;
    }
    entry->prev = NULL;
    entry->next = cache->head;
    if (cache->head != NULL)
    {
        cache->head->prev = entry;
    }
    cache->head = entry;
}
/*-----------------------------------------------------------------*/
char* find_in_cache(Cache* cache, const char* domain)
{
    unsigned int index = hash(domain);
    CacheEntry* entry = cache->entries[index];
    while (entry != NULL)
    {
        if (strcmp(entry->domain, domain) == 0)
        {
            move_to_front(cache, entry);
            return entry->ip;
        }
        entry = entry->next;
    }
    return NULL;
}

void show_cache(Cache* cache)
{
    printf("\nКэш:\n");
    CacheEntry* entry = cache->head;
    while (entry != NULL)
    {
        printf(ANSI_COLOR_CYAN "%s %s\n" ANSI_COLOR_RESET, entry->domain, entry->ip);
        entry = entry->next;
    }
}

void add_to_cache(Cache* cache, const char* domain, const char* ip)
{
    if (find_in_cache(cache, domain) != NULL)
    {
        return;
    }
    unsigned int index = hash(domain);
    CacheEntry* new_entry = (CacheEntry*)malloc(sizeof(CacheEntry));
    strcpy(new_entry->domain, domain);
    strcpy(new_entry->ip, ip);
    new_entry->prev = NULL;
    new_entry->next = cache->head;
    if (cache->head != NULL)
    {
        cache->head->prev = new_entry;
    }
    cache->head = new_entry;
    if (cache->tail == NULL)
    {
        cache->tail = new_entry;
    }

    if (cache->size == CACHE_SIZE)
    {
        CacheEntry* entry_to_remove = cache->tail;
        cache->tail = entry_to_remove->prev;
        if (cache->tail != NULL)
        {
            cache->tail->next = NULL;
        }
        remove_entry_from_cache(cache, entry_to_remove);
    }
    else
    {
        cache->size++;
    }
    if (cache->entries[index] != NULL)
    {
        new_entry->next = cache->entries[index];
    }
    cache->entries[index] = new_entry;
}

FILE* open_domain_file()
{
    FILE* file = fopen("C:/Users/misha/Desktop/test_l5.txt", "r");
    if (file == NULL)
    {
        printf("Не удалось открыть файл!");
        exit(-1);
    }
    return file;
}

void get_domain(char* domain)
{
    printf(ANSI_COLOR_YELLOW "\nВведите доменное имя: " ANSI_COLOR_RESET);
    scanf("%s", domain);
}


/*----------------------------------------------------------------------------------------------------*/
char* find_ip_address(FILE* file, Cache* cache, char* domain)
{
    char original_domain[MAX_LENGTH];
    strcpy(original_domain, domain);
    char* cached_ip = find_in_cache(cache, domain);
    char* new_cached_ip;
    if (cached_ip != NULL)
    {
        printf(ANSI_COLOR_BLUE "\n Найдено в кеше \n" ANSI_COLOR_RESET);
        printf(ANSI_COLOR_CYAN "IP адресс: %s\n" ANSI_COLOR_RESET, cached_ip);
        new_cached_ip = (char*)malloc(strlen(cached_ip) + 1);
        strcpy(new_cached_ip, cached_ip);
        return new_cached_ip;
    }
    printf(ANSI_COLOR_RED "\nНе содержится в кеше\n" ANSI_COLOR_RESET);
    fseek(file, 0, SEEK_SET);
    char line[MAX_LENGTH];
    while (fgets(line, MAX_LENGTH, file) != NULL)
    {
        char name[MAX_LENGTH];
        char type[MAX_LENGTH];
        char value[MAX_LENGTH];
        char* new_value;
        sscanf(line, "%s %*s %s %s", name, type, value);
        if (strcmp(name, domain) == 0)
        {
            if (strcmp(type, "A") == 0)
            {
                printf(ANSI_COLOR_CYAN "IP адресс: %s\n" ANSI_COLOR_RESET, value);
                add_to_cache(cache, original_domain, value);
                new_value = (char*)malloc(strlen(value) + 1);
                strcpy(new_value, value);
                return new_value;
            }
            else if (strcmp(type, "CNAME") == 0)
            {
                strcpy(domain, value);
                fseek(file, 0, SEEK_SET);
            }
        }
    }
    return NULL;
}

int is_valid_ip(const char* ip)
{
    int octets[4];
    int num_octets = sscanf(ip, "%d.%d.%d.%d", &octets[0], &octets[1], &octets[2], &octets[3]);
    if (num_octets != 4)
    {
        return 0;
    }
    for (int i = 0; i < 4; i++)
    {
        if (octets[i] < 0 || octets[i] > 255)
        {
            return 0;
        }
    }
    return 1;
}

int is_koliz_record(const char* domain, const char* type, const char* value)
{
    FILE* file = fopen("C:/Users/misha/Desktop/test_l5.txt", "r");
    if (file == NULL)
    {
        return 0;
    }
    char line[MAX_LENGTH];
    while (fgets(line, MAX_LENGTH, file) != NULL)
    {
        char name[MAX_LENGTH];
        char record_type[MAX_LENGTH];
        char record_value[MAX_LENGTH];
        sscanf(line, "%s %*s %s %s", name, record_type, record_value);
        if (strcmp(name, domain) == 0 && strcmp(record_type, type) == 0 && strcmp(record_value, value) == 0)
        {
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}

void find_and_print_cname_records(FILE* file, const char* domain)
{
    fseek(file, 0, SEEK_SET);
    char cname_line[MAX_LENGTH];
    while (fgets(cname_line, MAX_LENGTH, file) != NULL)
    {
        char cname_name[MAX_LENGTH];
        char cname_type[MAX_LENGTH];
        char cname_value[MAX_LENGTH];
        sscanf(cname_line, "%s %*s %s %s", cname_name, cname_type, cname_value);
        if (strcmp(cname_type, "CNAME") == 0 && strcmp(cname_value, domain) == 0)
        {
            printf("%s\n", cname_name);
        }
    }
}

void find_domains_by_ip()
{
    printf(ANSI_COLOR_YELLOW "\nВведите IP-адрес: " ANSI_COLOR_RESET);
    char ip[MAX_LENGTH];
    scanf("%s", ip);
    if (!is_valid_ip(ip))
    {
        printf(ANSI_COLOR_RED "Неверный IP-адрес\n\n" ANSI_COLOR_RESET);
        return;
    }
    FILE* file = fopen("C:/Users/misha/Desktop/test_l5.txt", "r");
    if (file == NULL)
    {
        printf(ANSI_COLOR_RED "Ошибка открытия файла!\n\n" ANSI_COLOR_RESET);
        return;
    }
    int found = 0;
    char line[MAX_LENGTH];
    while (fgets(line, MAX_LENGTH, file) != NULL)
    {
        char name[MAX_LENGTH];
        char type[MAX_LENGTH];
        char value[MAX_LENGTH];
        sscanf(line, "%s %*s %s %s", name, type, value);
        if (strcmp(type, "A") == 0 && strcmp(value, ip) == 0)
        {
            printf("%s\n", name);
            found = 1;
            find_and_print_cname_records(file, name);
        }
    }
    fclose(file);
    if (!found)
    {
        printf(ANSI_COLOR_RED "Домены не найдены\n\n" ANSI_COLOR_RESET);
    }
}

int is_valid_domain(const char* domain)
{
    if (domain[0] == '\0')
    {
        return 0;
    }
    for (int i = 0; domain[i] != '\0'; i++)
    {
        if (!isalnum(domain[i]) && domain[i] != '.' && domain[i] != '-')
        {
            return 0;
        }
    }
    int dot_count = 0;
    for (int i = 0; domain[i] != '\0'; i++)
    {
        if (domain[i] == '.')
        {
            dot_count++;
            if (i == 0 || domain[i - 1] == '.' || domain[i + 1] == '\0')
            {
                return 0;
            }
        }
    }
    if (dot_count < 1)
    {
        return 0;
    }
    return 1;
}

void add_record()
{
    char domain[MAX_LENGTH];
    do
    {
        printf(ANSI_COLOR_YELLOW "\nВведите доменное имя: " ANSI_COLOR_RESET);
        scanf("%s", domain);
        if (!is_valid_domain(domain))
        {
            printf(ANSI_COLOR_RED "Недопустимое доменное имя\n" ANSI_COLOR_RESET);
        }
    } while (!is_valid_domain(domain));

    char type[MAX_LENGTH];
    do
    {
        printf(ANSI_COLOR_YELLOW "Введите тип записи (A или CNAME): " ANSI_COLOR_RESET);
        scanf("%s", type);
        if (strcmp(type, "A") != 0 && strcmp(type, "CNAME") != 0)
        {
            printf(ANSI_COLOR_RED "Недопустимый тип записи\n" ANSI_COLOR_RESET);
        }
    } while (strcmp(type, "A") != 0 && strcmp(type, "CNAME") != 0);

    char value[MAX_LENGTH];
    if (strcmp(type, "A") == 0)
    {
        do
        {
            printf(ANSI_COLOR_YELLOW "Введите значение записи: " ANSI_COLOR_RESET);
            scanf("%s", value);
            if (!is_valid_ip(value))
            {
                printf(ANSI_COLOR_RED "Неверный IP-адрес\n" ANSI_COLOR_RESET);
            }
        } while (!is_valid_ip(value));
    }
    else
    {
        do
        {
            printf(ANSI_COLOR_YELLOW "Введите значение записи: " ANSI_COLOR_RESET);
            scanf("%s", value);
            if (!is_valid_cname(value))
            {
                printf(ANSI_COLOR_RED "Недопустимое значение CNAME\n" ANSI_COLOR_RESET);
            }
        } while (!is_valid_cname(value));
    }

    if (is_koliz_record(domain, type, value))
    {
        printf(ANSI_COLOR_RED "Ошибка! Произошла коллизия!\n" ANSI_COLOR_RESET);
        return;
    }

    FILE* file = fopen("C:/Users/misha/Desktop/test_l5.txt", "a");
    if (file == NULL)
    {
        printf(ANSI_COLOR_RED "Ошибка открытия файла!\n\n" ANSI_COLOR_RESET);
        return;
    }
    fprintf(file, "%s IN %s %s\n", domain, type, value);
    fclose(file);

    printf(ANSI_COLOR_BLUE "Запись добавлена\n" ANSI_COLOR_RESET);
}

void remove_entry_from_cache(Cache* cache, CacheEntry* entry_to_remove)
{
    unsigned int index_to_remove = hash(entry_to_remove->domain);
    if (entry_to_remove == cache->entries[index_to_remove])
    {
        cache->entries[index_to_remove] = entry_to_remove->next;
    }
    else
    {
        CacheEntry* current_entry = cache->entries[index_to_remove];
        while (current_entry != NULL)
        {
            if (current_entry->next == entry_to_remove)
            {
                current_entry->next = entry_to_remove->next;
                break;
            }
            current_entry = current_entry->next;
        }
    }
    free(entry_to_remove);
}
/*------------------------------------------------------------------------------------------------------------*/
void free_cache(Cache* cache)
{
    CacheEntry* entry = cache->head;
    while (entry != NULL)
    {
        CacheEntry* next_entry = entry->next;
        free(entry);
        entry = next_entry;
    }
}
void find_ip_by_domain(Cache* cache)
{
    char domain[MAX_LENGTH];
    get_domain(domain);
    char original_domain[MAX_LENGTH];
    strcpy(original_domain, domain);

    FILE* file = open_domain_file();

    char* ip = find_ip_address(file, cache, domain);
    if (ip == NULL)
    {
        printf(ANSI_COLOR_RED "Домен не найден\n" ANSI_COLOR_RESET);
    }
    else
    {
        add_to_cache(cache, original_domain, ip);
        free(ip);
    }
    fclose(file);
}

void menu(Cache* cache)
{
    int value;
    do
    {
        printf("\n1. Найти IP по домену\n");
        printf("2. Показать кеш\n");
        printf("3. Добавить запись\n");
        printf("4. Поиск доменов по IP\n");
        printf("0. Выход\n");
        printf(ANSI_COLOR_YELLOW "\nВаш выбор: " ANSI_COLOR_RESET);

        value = input_value();

        system("cls");
        switch (value)
        {
        case 1:
            find_ip_by_domain(cache);
            break;
        case 2:
            show_cache(cache);
            break;
        case 3:
            add_record();
            break;
        case 4:
            find_domains_by_ip();
            break;
        default:
            break;
        }
    } while (value != 0);
}
