
#include "deamon_works.h"

void daemonize() {
    pid_t pid;

    /* Создаем дочерний процесс */
    pid = fork();

    /* Если не удалось создать дочерний процесс */
    if (pid < 0)
        exit(EXIT_FAILURE);

    /* Если мы получили положительный PID, это означает, что мы являемся родительским процессом */
    if (pid > 0)
        exit(EXIT_SUCCESS);

    /* Изменяем маску файла, чтобы записи могли быть прочитаны и записаны правильно */
    umask(0);

    /* Создаем новый сеанс, делая текущий процесс его лидером */
    if (setsid() < 0)
        exit(EXIT_FAILURE);

    /* Изменяем рабочий каталог на корневой каталог */
    if (chdir("/") < 0)
        exit(EXIT_FAILURE);

    /* Закрываем стандартные файловые дескрипторы */
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
}

bool is_process_running(const char* process_name) {
    char buffer[128];
    std::string command = std::string("pgrep ") + process_name;
    std::string result = "";
    FILE* pipe = popen(command.c_str(), "r");

    if (!pipe) throw std::runtime_error("popen() failed!");

    try {
        while (fgets(buffer, sizeof buffer, pipe) != NULL) {
            result += buffer;
        }
    } catch (...) {
        pclose(pipe);
        throw;
    }

    pclose(pipe);


    return !result.empty();
}