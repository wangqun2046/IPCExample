#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>

#define NUM_OF_SEMS 1
#define SEM_NAME "sem_test"

int main(int argc, char** argv)
{
    bool is_owner = true;
    sem_t *p_sem = sem_open(SEM_NAME, O_CREAT | O_EXCL, 0666, NUM_OF_SEMS);
    if (p_sem == SEM_FAILED)
    { 
        if (errno != EEXIST)
        {
            perror("call sem_open failed!");
            exit(EXIT_FAILURE);
        }
        else
        {
            is_owner = false;
            p_sem = sem_open(SEM_NAME, 0);
            printf("exit: sem is %p\n", p_sem);
        }
    }
    for (int i = 0; i < 5; i++)
    {
        printf("start wait: %d\n", getpid());
        if (sem_wait(p_sem))
        {
            perror("semop: ");
            exit(EXIT_FAILURE);
        }
        sleep(1);
        printf("start post: %d\n", getpid());
        if (sem_post(p_sem))
        {
            exit(EXIT_FAILURE);
        }
        sleep(1);
    }
    sem_close(p_sem);
    if (is_owner)
    {
        sem_unlink(SEM_NAME);
    }
    return EXIT_SUCCESS;
}
