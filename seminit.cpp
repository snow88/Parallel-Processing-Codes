#include<iostream>
#include<sys/ipc.h>
#include<sys/types.h>
#include<sys/sem.h>
using namespace std;

int main()
{
    key_t key = ftok("semfile",67);
    int semid = semget(key, 1, 0666|IPC_CREAT);
    if (semid < 0)
    {
        cout<<"Unable to create semaphore"<<endl;
        exit(0);
    }

    union semun {
        int val;
        struct semid_ds *buf;
        ushort *ar;
    };
    semun su;
    su.val = 0;
    semctl(semid,0,SETVAL,su);  
    cout<<"Semaphore created & initialized to 0"<<endl;

    return 0;
}

