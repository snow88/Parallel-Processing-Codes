///run producer in background first

#include<iostream>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/sem.h>
#include<sys/types.h>
#include<sys/time.h>
#include<unistd.h>
using namespace std;

int shmid, semid;
void *addr;
union semun {
        int val;
        struct semid_ds *buf;
        ushort *ar;
    };
timeval tv, tvcur;

void init()
{
    gettimeofday(&tv, 0);

    key_t keyshm = ftok("shmfile", 65);
    shmid = shmget(keyshm, 1024, 0666|IPC_CREAT);
    addr = shmat(shmid, (void*)0, 0);

    key_t keysem = ftok("semfile", 67);
    semid = semget(keysem, 3, 0666|IPC_CREAT);
    semun su;
    su.val = 1;
    semctl(semid, 0, SETVAL, su);      ///set mutex semaphore (no.0) to 1
    su.val = 5;
    semctl(semid, 1, SETVAL, su);      ///set empty semaphore (no.1) to 5        ///size of buffer is 5 here
    su.val = 0;
    semctl(semid, 2, SETVAL, su);      ///set full semaphore (no.2) to 0
}

void destroy()
{
    semctl(semid, 0, IPC_RMID);
    shmdt(addr);
    shmctl(shmid, IPC_RMID, NULL);
}

int main()
{
    init();

    int *val = (int*)addr;
    while(true)
    {
        gettimeofday(&tvcur, 0);
        if (tvcur.tv_sec-tv.tv_sec>7)
            break;

        sembuf s[1];          ///wait(empty)
        s[0].sem_num = 1;
        s[0].sem_op = -1;
        s[0].sem_flg = 0;
        semop(semid, s, 1);

        s[0].sem_num = 0;     ///lock(mutex)
        s[0].sem_op = -1;
        s[0].sem_flg = 0;
        semop(semid, s, 1);

        semun sg;
        int emp = semctl(semid, 1, GETVAL, sg);
        val[5-emp] = rand()%100;
        cout<<"Value Produced: "<<val[5-emp]<<endl;

        s[0].sem_num = 0;     ///unlock(mutex)
        s[0].sem_op = 1;
        s[0].sem_flg = 0;
        semop(semid, s, 1);

        s[0].sem_num = 2;     ///signal(full)
        s[0].sem_op = 1;
        s[0].sem_flg = 0;
        semop(semid, s, 1);
    }

    sleep(4); 
    destroy();

    return 0;
}
