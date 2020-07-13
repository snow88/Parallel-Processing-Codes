#include<iostream>
#include<sys/ipc.h>
#include<sys/types.h>
#include<sys/sem.h>
using namespace std;

int main()
{
    key_t key = ftok("semfile",67);
    int semid = semget(key, 1, 0);        
    if (semid == -1)
    {
        perror("semget");
        exit(1);
    }

    semctl(semid,0,IPC_RMID);     

    return 0;
}


