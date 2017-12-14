#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>

int main(void)
{
    int shmid;
    size_t shsize = 1024;
    const int key = 16000;
    char *shm;

    sem_t *mysem;
    if((mysem = sem_open("mysem", 0, 0777, 0)) == SEM_FAILED) {
        perror("Sem Open Error");
        exit(1);
    }

    if((shmid = shmget((key_t)key, shsize, IPC_CREAT|0666))<0) {
        perror("shmget");
        exit(1);
    }

    if((shm = (char*)shmat(shmid, NULL, 0)) == (char*)-1) {
        perror("shmat");
        exit(1);
    }

    struct timespec ts;
    if (clock_gettime(CLOCK_REALTIME, &ts) == -1) {
        perror("clock_gettime");
        exit(EXIT_FAILURE);
    }
    ts.tv_sec += 1;

    int ret;
    for(;;) {
        if((ret = sem_timedwait(mysem, &ts)) == 0) {
            for(int i=0; i<100; i++) {
                putchar(shm[i]);
            }

            putchar('\n');
            sem_post(mysem);
        }
        else {
            switch(errno) {
                case EDEADLK:
                    printf("EDEADLK\n");
                    break;
                case ETIMEDOUT:
                    printf("ETIMEDOUT\n");
                    break;
                case EINVAL:
                    printf("EINVAL\n");
                    break;
                default:
                    printf("DEF\n");
            }
        }
        printf("[%d]", ret);



        usleep(100*1000);
    }

    if(shmdt(shm) == -1) {
       perror("shmdt");
       exit(1);
    }

    return 0;
}
