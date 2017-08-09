#include "shutils.h"

void handler(int signo)
{
    printf("READER: get signal\n");
}

int main(int argc, char** argv)
{
    bool running = true;
    pid_t pid;
    signal(SIGUSR1, handler);
    void* shm_p = NULL;
    shared_data* shm_data = NULL;
    int shm_id = shm_open(IPC_SHM_NAME, O_CREAT | O_RDWR | O_EXCL, 0666);
    if (shm_id == -1)
    {
        if (EEXIST == errno)
        {
            printf("READER: share memory for key[%s] exist!\n", IPC_SHM_NAME);
            shm_id = shm_open(IPC_SHM_NAME, O_RDONLY, 0666);
            ftruncate(shm_id, sizeof(shared_data));
            shm_data = (shared_data*)mmap(0, sizeof(shared_data), PROT_READ, MAP_SHARED, shm_id, 0);
            pid = shm_data->pid;
            shm_data->pid = getpid();
            kill(pid, SIGUSR1);
        }
        else
        {
            perror("READER: call shm_open failed!\n");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        ftruncate(shm_id, sizeof(shared_data));
        shm_data = (shared_data*)mmap(0, sizeof(shared_data), PROT_READ, MAP_SHARED, shm_id, 0);
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
    munmap(shm_p, sizeof(shared_data));
    shm_unlink(IPC_SHM_NAME);
    return 0;
}
