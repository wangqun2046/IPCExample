#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <errno.h>

#define NUM_OF_SEMS 1

int semaphore_wait(int semid);
int semaphore_post(int semid);

int main(int argc, char** argv)
{
    key_t key = ftok(".", 'm');
    if (key < 0)
    {
        perror("call ftok failed!");
        exit(EXIT_FAILURE);
    }
    printf("key: %d.\n", key);
    bool is_owner = true;
    int nsems = NUM_OF_SEMS;
    int semid = semget(key, nsems, 0666 | IPC_CREAT | IPC_EXCL);
    if (semid < 0 && errno != EEXIST)
    {
        perror("call semget failed!");
        exit(EXIT_FAILURE);
    }
    if (semid >= 0)
    {
        printf("create semid is %d\n", semid);
        semun_t sem_union;
        sem_union.val = 1;
        if (semctl(semid, 0, SETVAL, sem_union) == -1)
        {
            perror("semctl");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        is_owner = false;
        semid = semget(key, nsems, 0666);
        printf("exit: semid is %d\n", semid);
    }
    for (int i = 0; i < 5; i++)
    {
        printf("start wait: %d\n", getpid());
        if (semaphore_wait(semid))
        {
            perror("semop: ");
            exit(EXIT_FAILURE);
        }
        sleep(1);
        printf("start post: %d\n", getpid());
        if (semaphore_post(semid))
        {
            exit(EXIT_FAILURE);
        }
        sleep(1);
    }
    if (is_owner)
    {
        semctl(semid, 0, IPC_RMID, semid);
    }
    return EXIT_SUCCESS;
}

int semaphore_wait(int semid)
{
    struct sembuf sem_b;
    sem_b.sem_num = 0;
    sem_b.sem_op = -1;
    sem_b.sem_flg = SEM_UNDO;
    return semop(semid, &sem_b, 1);
}

int semaphore_post(int semid)
{
    struct sembuf sem_b;
    sem_b.sem_num = 0;
    sem_b.sem_op = 1;
    sem_b.sem_flg = SEM_UNDO;
    return semop(semid, &sem_b, 1);
}
