#include<iostream>
#include<unordered_map>
#include<sys/types.h>
#include<sys/sem.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>
#include<sys/time.h>
using namespace std;

int main()
{
    key_t key = ftok("semfile", 66);
    int semid = semget(key, 1, 0666|IPC_CREAT);
    if (semid < 0)
    {
        cout<<"Unable to create semaphore"<<endl;
        exit(0);
    }

    union semun
    {
        int val;
        struct semid_ds *buf;
        ushort *ar;
    };
    semun su;
    su.val = 1;
    semctl(semid,0,SETVAL,su);  
    cout<<"Semaphore created & initialized to 1"<<endl;

    timeval tv;
    gettimeofday(&tv,0);
    cout<<tv.tv_usec<<": WAR Launched"<<endl;

    unordered_map<int,int> pctr;        ///win counter

    int pid = fork();

    while(true)
    {
        if (pid != 0)      ///parent, process 1
        {
            if (pctr.find(getpid()) == pctr.end())
            {
                pctr[getpid()]=0;
                gettimeofday(&tv,0);
                cout<<tv.tv_usec<<": Process "<<getpid()<<" created."<<endl;
            }
            sleep(rand()%5);
            struct sembuf s[1];
            s[0].sem_num = 0;
            s[0].sem_op = -1;
            s[0].sem_flg = 0;
            semop(semid,s,1);
            cout<<"Process "<<getpid()<<" executed."<<endl;
            gettimeofday(&tv,0);
            cout<<tv.tv_usec<<": Process "<<pid<<" terminated."<<endl;
            kill(pid,9);
            pctr[getpid()]++;
            if (pctr[getpid()] == 3)
            {
                cout<<"Process "<<getpid()<<" won."<<endl;
                break;
            }
            pid = fork();
            if (pid != 0)
            {
                s[0].sem_num = 0;
                s[0].sem_op = 1;
                s[0].sem_flg = 0;
                semop(semid,s,1);
            }
        }

        else             ///child, process 2
        {
            if (pctr.find(getpid()) == pctr.end())
            {
                pctr[getpid()]=0;
                gettimeofday(&tv,0);
                cout<<tv.tv_usec<<": Process "<<getpid()<<" created."<<endl;
            }
            sleep(rand()%5);
            struct sembuf s[1];
            s[0].sem_num = 0;
            s[0].sem_op = -1;
            s[0].sem_flg = 0;
            semop(semid,s,1);
            cout<<"Process "<<getpid()<<" executed."<<endl;
            gettimeofday(&tv,0);
            cout<<tv.tv_usec<<": Process "<<getppid()<<" terminated."<<endl;
            kill(getppid(),9);
            pctr[getpid()]++;
            if (pctr[getpid()] == 3)
            {
                cout<<"Process "<<getpid()<<" won."<<endl;
                break;
            }
            pid = fork();
            if (pid != 0)
            {
                s[0].sem_num = 0;
                s[0].sem_op = 1;
                s[0].sem_flg = 0;
                semop(semid,s,1);
            }
        }
    }

    semctl(semid,0,IPC_RMID);

    return 0;
}
