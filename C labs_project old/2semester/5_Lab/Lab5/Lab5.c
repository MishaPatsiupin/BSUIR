#include "func.h"

int main()
{
    setlocale(LC_ALL, "Russian");
    Cache cache;
    init_cache(&cache);
    menu(&cache);
    free_cache(&cache);
    return 0;
}
