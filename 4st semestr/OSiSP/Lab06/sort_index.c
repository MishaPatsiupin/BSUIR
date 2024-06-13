#define _GNU_SOURCE
#include <sys/mman.h>
#include <fcntl.h>
#include <pthread.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <unistd.h>
#include <errno.h>
#include <stdbool.h>
#include <string.h>

#define BUFFER_FILENAME "buffer.bin"
#define SORTED_FILENAME "result.bin"
#define N 8192

typedef struct index_s {
    double time_mark;       // временная метка (модифицированная юлианская дата)
    uint64_t recno;         // первичный индекс в таблице БД
} index_s;

typedef struct index_hdr_s {
    uint64_t records;       // количество записей
    index_s idx[];          // массив записей в количестве records
} index_hdr_s;


int compare_records(const void *, const void *);
size_t count_records(const char *, const char *);
void merge(long, long, index_s *);
void merge_sorted(char *);
void *func(void *);

int size;
int amount_of_blocks;
int size_of_block;
long size_of_record;
int *busy;
int amount_of_threads;
int sorted_in_buffer;
long recs_in_buffer;
size_t amount_of_recs;

char *filename;
int fd;
void *file_map;
long size_of_file;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_barrier_t bar;

void print_data(char *filename, long num)
{
    index_s index;
    index_hdr_s header;
    size_t read_size;
    long rec_count = 0;
    FILE *f = fopen(filename, "rb");

    if (num == 0 && (read_size = fread(&header, sizeof(index_hdr_s), 1, f)) > 0)
    {
        printf("\nКоличество записей: %lu\n", header.records);
        printf("\n");
    }

    while ((read_size = fread(&index, sizeof(index_s), 1, f)) > 0)
    {
        printf("Временная метка: \"%f\" с индексом: %lu\n", index.time_mark, index.recno);
        ++rec_count;
    }
    printf("\n");

    if(num == 1)
        printf("Записей отсортировано: %lu\n", rec_count);

    fclose(f);
}

int main(int argc, char *argv[])
{
    if (argc != 5)
    {
        fprintf(stderr, "Корректный формат: %s memsize blocks threads filename\n", argv[0]);
        return EXIT_FAILURE;
    }

    size = (int) strtol(argv[1], NULL, 10);
    amount_of_blocks = (int) strtol(argv[2], NULL, 10);
    amount_of_threads = (int) strtol(argv[3], NULL, 10);
    filename = argv[4];

    int pagesize = getpagesize();
    if (size % pagesize)
    {
        fprintf(stderr, "Размер буфера (memsize) должен быть кратен размеру страницы\n");
        return EXIT_FAILURE;
    }

    if ((amount_of_blocks & (amount_of_blocks - 1)))
    {
        fprintf(stderr, "Кол-во блоков(blocks) должно быть степенью двойки\n");
        return EXIT_FAILURE;
    }

    if (amount_of_blocks <= amount_of_threads)
    {
        fprintf(stderr, "Кол-во blocks должно быть больше кол-ва threads\n");
        return EXIT_FAILURE;
    }

    int core = sysconf(_SC_NPROCESSORS_ONLN);
    if (amount_of_threads < core || amount_of_threads > N)
    {
        fprintf(stderr, "Кол-во blocks должно быть между %d и %d\n", core, N);
        return -1;
    }

    size_of_block = size / amount_of_blocks;
    size_of_record = sizeof(struct index_s);
    sorted_in_buffer = 0;
    recs_in_buffer = size / size_of_record;
    size_t control_value;

    control_value = count_records(filename, BUFFER_FILENAME);
    fd = open(BUFFER_FILENAME, O_RDWR);
    size_of_file = lseek(fd, 0, SEEK_END);

    amount_of_recs = size_of_file / size_of_record;
    if (control_value != amount_of_recs)
    {
        fprintf(stderr, "Файл повреждён\n");
        return -1;
    }

    pthread_mutex_init(&mutex, NULL);
    pthread_barrier_init(&bar, NULL, amount_of_threads);

    pthread_t *mas_threads = (pthread_t *) malloc(amount_of_threads * sizeof(pthread_t));
    int *numthreads = (int *) malloc(amount_of_threads * sizeof(int));

    for (int i = 0; i < amount_of_threads; i++)
    {
        numthreads[i] = i;
        pthread_create(&mas_threads[i], NULL, func, &numthreads[i]);
    }

    for (int i = 0; i < amount_of_threads; i++)
        pthread_join(mas_threads[i], NULL);

    pthread_mutex_destroy(&mutex);
    pthread_barrier_destroy(&bar);

    free(mas_threads);
    free(numthreads);
    free(busy);

    close(fd);
    remove(BUFFER_FILENAME);

    printf("\n");
    printf("%s был отсортирован и сохранён в %s\n\n", filename, SORTED_FILENAME);

    print_data(SORTED_FILENAME, 1);
    printf("\n");
    return 0;
}

int compare_records(const void *one, const void *two)
{
    const index_s *first = (const index_s *) one;
    const index_s *second = (const index_s *) two;

    if (first->time_mark < second->time_mark)
        return -1;
    else if (first->time_mark > second->time_mark)
        return 1;
    else
        return 0;
}

size_t count_records(const char *s, const char *d)
{
    index_s index;
    index_hdr_s header;
    size_t size;
    size_t count = 0;

    FILE *f = fopen(s, "rb");
    if ((size = fread(&header, sizeof(index_hdr_s), 1, f)) > 0)
        count = header.records;

    FILE *f1 = fopen(d, "w");
    while ((size = fread(&index, sizeof(index_s), 1, f)) > 0)
        fwrite(&index, sizeof(index_s), 1, f1);

    fclose(f);
    fclose(f1);
    return count;
}

void merge(long num, long amount_of_records, index_s *block)
{
    long first = num;
    long second = num + amount_of_records;
    index_s *merged = (index_s*) calloc(2 * amount_of_records, size_of_record);
    long i = first;
    long j = second;
    long k = 0;

    while (i < second && j < second + amount_of_records)
    {
        if (block[i].time_mark <= block[j].time_mark)
        {
            merged[k] = block[i];
            i++;
        }
        else
        {
            merged[k] = block[j];
            j++;
        }
        k++;
    }
    while (i < second)
    {
        merged[k] = block[i];
        i++;
        k++;
    }
    while (j < second + amount_of_records)
    {
        merged[k] = block[j];
        j++;
        k++;
    }
    usleep(1);

    for (int index = 0; index < 2 * amount_of_records; index++)
        block[first + index] = merged[index];

    free(merged);
}

void merge_sorted(char *filename)
{
    index_s mindata;
    int min;
    index_s tmp_val;
    size_t amount = 0;

    FILE *f = fopen(filename, "w");
    file_map = mmap(NULL, size_of_file, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    int *inds = (int *) calloc(sorted_in_buffer, sizeof(*inds));

    while (amount < amount_of_recs)
    {
        min = -1;
        for (int i = 0; i < sorted_in_buffer; i++)
        {
            if (inds[i] < 0)
                continue;

            tmp_val = ((index_s *) file_map)[inds[i] + i * recs_in_buffer];
            if (min == -1)
            {
                mindata = tmp_val;
                min = i;
            }
            if (tmp_val.time_mark < mindata.time_mark)
            {
                mindata = tmp_val;
                min = i;
            }
        }
        fwrite(&mindata, size_of_record, 1, f);
        ++inds[min];
        ++amount;
        if (inds[min] == recs_in_buffer) inds[min] = -1;
    }
    munmap(file_map, size_of_file);
    fclose(f);
}

void *func(void *arg)
{
    int thread_num = *((int *) arg);

    while (true)
    {
        if (!thread_num)
        {
            file_map = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED,
                            fd, size * sorted_in_buffer);
            if (file_map == MAP_FAILED)
            {
                perror("mmap");
                return NULL;
            }
            busy = (int *) calloc(amount_of_blocks, sizeof(int));
            for (int i = 0; i < amount_of_blocks; i++)
            {
                if(i < amount_of_threads)
                    busy[i] = i;
                else
                    busy[i] = -1;
            }
        }

        pthread_barrier_wait(&bar);

        qsort(&(((index_s *) file_map)[thread_num * size_of_block / size_of_record]),
              size_of_block / size_of_record, size_of_record, compare_records);
        int cur_block;
        while (true)
        {
            pthread_mutex_lock(&mutex);
            for (cur_block = 0; cur_block < amount_of_blocks; cur_block++)
            {
                if (busy[cur_block] == -1)
                {
                    busy[cur_block] = thread_num;
                    break;
                }
            }
            pthread_mutex_unlock(&mutex);
            if (cur_block == amount_of_blocks)
                break;

            qsort(&(((index_s *) file_map)[cur_block* size_of_block/ size_of_record]),
                  size_of_block / size_of_record, size_of_record, compare_records);
        }
        pthread_barrier_wait(&bar);

        int merged_blocks = amount_of_blocks;
        int merged_size = size_of_block;
        while (merged_blocks > 1)
        {
            memset(busy, -1, sizeof(*busy) * amount_of_blocks);
            while (true)
            {
                pthread_mutex_lock(&mutex);
                for (cur_block = 0; cur_block < merged_blocks; cur_block++)
                    if (busy[cur_block] == -1)
                    {
                        busy[cur_block] = thread_num;
                        busy[cur_block + 1] = thread_num;
                        break;
                    }

                pthread_mutex_unlock(&mutex);

                if (cur_block == merged_blocks)
                    break;

                long records_count = merged_size / size_of_record;

                pthread_mutex_lock(&mutex);
                merge(cur_block * records_count, records_count, file_map);
                pthread_mutex_unlock(&mutex);
            }

            pthread_barrier_wait(&bar);
            merged_blocks /= 2;
            merged_size *= 2;
        }
        if (!thread_num)
        {
            munmap(file_map, size);
            ++sorted_in_buffer;
        }
        pthread_barrier_wait(&bar);

        if (sorted_in_buffer * size >= size_of_file)
        {
            if (!thread_num)
                merge_sorted(SORTED_FILENAME);

            return NULL;
        }
    }
}