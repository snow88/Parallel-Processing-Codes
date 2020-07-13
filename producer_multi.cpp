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
    int semid = semget(keysem, 2, 0666);

    int *val = (int*)addr;
    while(true)
    {
        if (semget(keysem, 2, 0666) < 0)
            break;
        sembuf s[1];
        s[0].sem_num = 1;
        s[0].sem_op = -1;
        s[0].sem_flg = 0;
        semop(semid, s, 1);

        *val = rand()%1000;
        cout<<"Value Produced: "<<*val<<endl;

        if (semget(keysem, 2, 0666) < 0)
            break;
        s[0].sem_num = 0;
        s[0].sem_op = 1;
        s[0].sem_flg = 0;
        semop(semid, s, 1);
    }

    shmdt(addr);
    return 0;
}

