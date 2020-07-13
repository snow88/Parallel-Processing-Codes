#include<iostream>
#include<sys/ipc.h>
#include<sys/types.h>
#include<sys/sem.h>
#include<sys/shm.h>
#include<sys/time.h>
#include<unistd.h>
using namespace std;

///run in background
int main()
{
    key_t keyshm = ftok("shmfile",65);
    int shmid = shmget(keyshm, 1024, 0666|IPC_CREAT);
    void* addr = shmat(shmid, (void*)0, 0);

    key_t keysem = ftok("semfile",67);
    int semid = semget(keysem, 2, 0666|IPC_CREAT);     

    union semun {
        int val;
        struct semid_ds *buf;
        ushort *ar;
    };
    semun su;
    su.val = 0;
    semctl(semid,0,SETVAL,su);  ///sem 0 is for producer
    su.val = 1;
    semctl(semid,1,SETVAL,su);  ///sem 1 is for consumer

    sleep(10);

    semctl(semid, 0, IPC_RMID);  
    shmdt(addr);
    shmctl(shmid, IPC_RMID, NULL);
    return 0;
}
