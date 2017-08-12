#include "shmutils.h"

void handler(int signo)
{
    printf("READER: get signal\n");
}

int main(int argc, char** argv)
{
    bool running = true;
    pid_t pid;
    key_t shm_key;
    if ((shm_key = ftok(".", 'm')) < 0)
    {
        perror("READER: call ftok failed!");
        exit(EXIT_FAILURE);
    }
    printf("READER: shm_key: %d\n", shm_key);
    signal(SIGUSR1, handler);
    void* shm_p = NULL;
    shared_data* shm_data = NULL;
    int shm_id = shmget(shm_key, sizeof(shared_data), 0666 | IPC_CREAT | IPC_EXCL);
    if (shm_id == -1)
    {
        if (EEXIST == errno)
        {
            printf("READER: share memory for key[%d] exist!\n", shm_key);
            shm_id = shmget(shm_key, sizeof(shared_data), 0666);
            shm_data = (shared_data*)shmat(shm_id, NULL, 0);
            pid = shm_data->pid;
            shm_data->pid = getpid();
            kill(pid, SIGUSR1);
        }
        else
        {
            perror("READER: call shmget failed!\n");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        shm_p = shmat(shm_id, NULL, 0);
        shm_data = (shared_data*)shm_p;
        shm_data->pid = getpid();
        pause(); // wait for awake
        pid = shm_data->pid;
    }
    while (running)
    {
        pause();
        if (strcmp(shm_data->buffer, END_STR) == 0)
        {
            running = false;
        }
        printf("READER: read from shm: %s\n", shm_data->buffer);
        kill(pid, SIGUSR1);
    }
    shmdt(shm_p);
    shmctl(shm_id, IPC_RMID, NULL);
    return 0;
}
