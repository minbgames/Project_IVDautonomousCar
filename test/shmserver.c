#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <semaphore.h>
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
    sem_unlink("mysem");
    if((mysem = sem_open("mysem", O_CREAT, 0777, 1)) == NULL) {
        perror("Sem Open Error");
        exit(1);
    }

    if((shmid = shmget((size_t)key, shsize, IPC_CREAT|0666))<0) {
        perror("shmget");
        exit(1);
    }

    if((shm = (char*)shmat(shmid, NULL, 0)) == (char*)-1) {
        perror("shmat");
        exit(1);
    }

    for(;;) {
        sem_wait(mysem);
        for(int i=0; i<100; i++) {
            shm[i] = 'a';
        }
        usleep(10*1000*1000);
        sem_post(mysem);
        //usleep(1*1000);
        sem_wait(mysem);
        for(int i=0; i<100; i++) {
            shm[i] = 'Z';
        }
        //usleep(1*1000);
        sem_post(mysem);
    }


    putchar('e');
    putchar('\n');

    getchar();

    if(shmdt(shm) == -1) {
       perror("shmdt");
       exit(1);
    }

    if (shmctl(shmid, IPC_RMID, 0) == -1) {
       perror("shmctl");
       exit(1);
    }

    return 0;
}
