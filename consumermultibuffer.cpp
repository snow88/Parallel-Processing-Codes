#include<iostream>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/sem.h>
#include<unistd.h>
#include<sys/types.h>
using namespace std;
#define BUF_SIZE 5

void* addr;
int *beg, *last;

void print()
{
    int* s = (int*)addr;
    for (int i=0; i<BUF_SIZE; i++, s++)
        cout<<*s<<" ";
    cout<<endl;
}

int main()
{
    key_t kshm = ftok("shmfile", 65);
    int shmid = shmget(kshm, 1024, 0666|IPC_CREAT);
    addr = shmat(shmid, (void*)0, 0);

    key_t ksem = ftok("semfile", 67);
    int semid = semget(ksem, 3, 0666);

    beg = (int*)addr;
    last = beg;

    fork();

    for (int i=0; i<7; i++)
    {
        sembuf s[1];
        s[0].sem_num = 2;
        s[0].sem_op = -1;
        s[0].sem_flg = 0;
        semop(semid, s, 1);

        cout<<"Consumed : "<<*beg<<endl;
        *beg = 0;
        if (beg == (int*)addr + BUF_SIZE - 1)
            beg = (int*)addr;
        else
            beg++;
        print();


        s[0].sem_num = 1;
        s[0].sem_op = 1;
        s[0].sem_flg = 0;
        semop(semid, s, 1);

    }

    return 0;
}
