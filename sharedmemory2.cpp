#include<iostream>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/types.h>
using namespace std;

///reader
int main(int argc, char* argv[])
{
    key_t key = ftok("shmfile",65);
    int shmid = shmget(key, 1024, 0644|IPC_CREAT);
    void* addr = shmat(shmid, (void*)0, 0);            

    if(addr == (void*)-1)
        perror("shmat");

    int *s = (int*)addr;
    cout<<"Data from shm"<<endl;
    for (int i=0; i<10; i++,s++)
        cout<<*s<<" ";

    shmdt(addr);  
    shmctl(shmid,IPC_RMID,NULL);   

    return 0;
}
