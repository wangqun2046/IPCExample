#include "shmutils.h"

void handler(int signo)
{
    printf("WRITER: get signal\n");
}

int main(int argc, char** argv)
{
    bool running = true;
    signal(SIGUSR1, handler);
    pid_t pid;
    void* shm_p = NULL;
    shared_data* shm_data = NULL;
    int shm_id = shm_open(IPC_SHM_NAME, O_CREAT | O_RDWR | O_EXCL, 0666);
    if (shm_id == -1)
    {
        if (errno == EEXIST)
        {
            printf("WRITER: share memory for key[%s] exist!\n", IPC_SHM_NAME);
            shm_id = shm_open(IPC_SHM_NAME, O_RDWR, 0666);
            ftruncate(shm_id, sizeof(shared_data));
            shm_data = (shared_data*)mmap(0, sizeof(shared_data), PROT_WRITE, MAP_SHARED, shm_id, 0);
            pid = shm_data->pid;
            shm_data->pid = getpid();
        } 
        else
        {
            perror("WRITER: call shm_open failed!\n");
            exit(EXIT_FAILURE);
        }
    }
    else 
    {
        ftruncate(shm_id, sizeof(shared_data));
        shm_data = (shared_data*)mmap(0, sizeof(shared_data), PROT_READ, MAP_SHARED, shm_id, 0);
        shm_data->pid = getpid();
    }
    while (running)
    {
        printf("WRITER: write to shm: ");
		fflush(stdout);
        fgets(shm_data->buffer, BUFFER_SIZE, stdin);
		sleep(1);
        kill(pid, SIGUSR1);
        if (strcmp(shm_data->buffer, END_STR) == 0)
        {
            running = false;
            break;
        }
		printf("WRITER: wait for awake...\n");
        pause();
        pid = shm_data->pid;
    	shm_data->pid = getpid();
		printf("WRITER: awake from %0x.\n", pid);
    }
    munmap(shm_p, sizeof(shared_data));
    shm_unlink(IPC_SHM_NAME);
    return 0;
}
