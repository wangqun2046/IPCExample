#ifndef _SHARED_DATA_H_
#define _SHARED_DATA_H_

#include <stdlib.h>

#define BUFFER_SIZE 1024

typedef struct {
    pid_t pid;
    char buffer[BUFFER_SIZE];
} shared_data;

#endif