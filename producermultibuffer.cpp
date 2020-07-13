#include<iostream>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/sem.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>
using namespace std;
#define BUF_SIZE 5

union semun
{
    int val;
    struct semid_ds *buf;
    ushort *ar;
};

int shmid;
void* addr;
int semid;
int *beg, *last;

void init()                  ///run producer first to create shm and sem
{
    key_t kshm = ftok("shmfile", 65);
    shmid = shmget(kshm, 1024, 0666|IPC_CREAT);
    addr = shmat(shmid, (void*)0, 0);

    beg = (int*)addr;
    last = beg;         

    key_t ksem = ftok("semfile", 67);
    semid = semget(ksem, 3, 0666|IPC_CREAT);

    semun su;
    su.val = 1;
    semctl(semid, 0, SETVAL, su);
    su.val = 5;
    semctl(semid, 1, SETVAL, su);   ///empty
    su.val = 0;
    semctl(semid, 2, SETVAL, su);    ///full
}

void destroy()
{
    semctl(semid, 0, IPC_RMID);
    shmdt(addr);
    shmctl(shmid, IPC_RMID, NULL);
}

void print()
{
    int* s = (int*)addr;
    for (int i=0; i<BUF_SIZE; i++, s++)
        cout<<*s<<" ";
    cout<<endl;
}

int main()
{
    init();

    fork();

    for (int i=0; i<7; i++)
    {
        sembuf s[1];
        s[0].sem_num = 1;
        s[0].sem_op = -1;
        s[0].sem_flg = 0;
        semop(semid, s, 1);

        *last = rand()%100 + 1;
        cout<<"Produced : "<<*last<<endl;
        if (last == (int*)addr + BUF_SIZE - 1)
            last = (int*)addr;
        else
            last++;
        print();

        s[0].sem_num = 2;
        s[0].sem_op = 1;
        s[0].sem_flg = 0;
        semop(semid, s, 1);
    }

    wait(NULL);
    sleep(10);     
    destroy();

    return 0;
}

