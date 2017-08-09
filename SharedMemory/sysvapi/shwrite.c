#include "shutils.h"

void handler(int signo)
{
    printf("WRITER: get signal\n");
}

int main(int argc, char** argv)
{
    bool running = true;
    key_t shm_key;
    if ((shm_key = ftok(".", 'm')) < 0)
    {
        perror("WRITER: call ftok failed!");
        exit(EXIT_FAILURE);
    }
    printf("WRITER: shm_key: %d\n", shm_key);
    signal(SIGUSR1, handler);
    pid_t pid;
    void* shm_p = NULL;
    shared_data* shm_data = NULL;
    int shm_id = shmget(shm_key, sizeof(shared_data), 0666 | IPC_CREAT | IPC_EXCL);
    if (shm_id == -1)
    {
        if (errno == EEXIST)
        {
            printf("WRITER: share memory for key[%d] exist!\n", shm_key);
            shm_id = shmget(shm_key, sizeof(shared_data), 0666);
            shm_data = (shared_data*)shmat(shm_id, NULL, 0);
            pid = shm_data->pid;
            shm_data->pid = getpid();
            kill(pid, SIGUSR1);
        } 
        else
        {
            perror("WRITER: call shmget failed!\n");
            exit(EXIT_FAILURE);
        }
    }
    else 
    {
        shm_p = shmat(shm_id, NULL, 0);
        shared_data* shm_data = (shared_data*)shm_p;
        shm_data->pid = getpid();
        pause();
        pid = shm_data->pid;
    }
    while (running)
    {
        printf("WRITER: write to shm: ");
        fgets(shm_data->buffer, BUFFER_SIZE, stdin);
        kill(pid, SIGUSR1);
        if (strcmp(shm_data->buffer, END_STR) == 0)
        {
            running = false;
            break;
        }
        pause();
    }
    shmdt(shm_p);
    shmctl(shm_id, IPC_RMID, NULL);
    return 0;
}