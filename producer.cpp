#include<iostream>
#include<sys/types.h>
#include<sys/shm.h>
#include<sys/ipc.h>
#include<unistd.h>
#include<sys/time.h>
using namespace std;

///run this in the background
int main()
{
    key_t key = ftok("shmfile", 65);
    int shmid = shmget(key, 1024, 0666|IPC_CREAT);
    void* addr = shmat(shmid, (void*)0, 0);
    if (addr == (void*)-1)
        perror("shmat");

    int *val = (int*)addr;
    timeval tv, tvcur;
    gettimeofday(&tv,0);
    while(true)
    {
        gettimeofday(&tvcur,0);
        if (tvcur.tv_sec-tv.tv_sec>8)          ///simulate for 8 seconds
            break;
        while (*val != 0)
            sleep(1);
        *val = rand()%1000 + 1;
        cout<<"Value produced: "<<*val<<endl;
    }

    shmdt(addr);
    return 0;
}
