#ifndef BACK_DEAMON_WORKS_H
#define BACK_DEAMON_WORKS_H

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <csignal>
#include <cstdlib>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <string>
#include <stdexcept>
#include <sys/prctl.h>


void daemonize();

#endif //BACK_DEAMON_WORKS_H
