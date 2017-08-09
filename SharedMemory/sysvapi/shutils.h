#ifndef _SHUTILS_H_
#define _SHUTILS_H_

#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <sys/shm.h>
#include "shared_data.h"

#define IPC_SHM_KEY 0x99

const char* END_STR = "end\n";

#endif