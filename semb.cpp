#include<iostream>
#include<sys/ipc.h>
#include<sys/types.h>
#include<sys/sem.h>
using namespace std;

int main()
{
    key_t key = ftok("semfile",67);
    int semid = semget(key, 1, 0666);             
    if (semid < 0)
    {
        cout<<"Cannot find required semaphore"<<endl;
        exit(0);
    }

    cout<<"Semb.cpp => doing p-operation (wait)"<<endl;

    struct sembuf s[1];
    s[0].sem_num = 0;
    s[0].sem_op = -1;
    s[0].sem_flg = 0;
    int retval = semop(semid,s,1);          
    if (retval == 0)
        cout<<"done"<<endl;
    else
    {
        cout<<"not done"<<endl;
        perror("reason");
    }

    return 0;
}



