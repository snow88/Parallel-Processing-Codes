#include<iostream>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/sem.h>
#include<sys/types.h>
using namespace std;

int shmid, semid;
void *addr;
union semun {
        int val;
        struct semid_ds *buf;
        ushort *ar;
    };

int main()
{
    key_t keyshm = ftok("shmfile", 65);
    shmid = shmget(keyshm, 1024, 0666|IPC_CREAT);
    addr = shmat(shmid, (void*)0, 0);

    key_t keysem = ftok("semfile", 67);
    semid = semget(keysem, 3, 0666);

    int *val = (int*)addr;
    while(true)
    {
        sembuf s[1];          ///wait(full)
        s[0].sem_num = 2;
        s[0].sem_op = -1;
        s[0].sem_flg = 0;
        semop(semid, s, 1);

        if (semget(keysem, 3, 0666) < 0)
            break;

        s[0].sem_num = 0;     ///lock(mutex)
        s[0].sem_op = -1;
        s[0].sem_flg = 0;
        semop(semid, s, 1);

        semun sg;
        int ful = semctl(semid, 2, GETVAL, sg);
        cout<<"Value Consumed: "<<val[ful+1]<<endl;
        val[ful+1] = 0;

        s[0].sem_num = 0;     ///unlock(mutex)
        s[0].sem_op = 1;
        s[0].sem_flg = 0;
        semop(semid, s, 1);

        s[0].sem_num = 1;     ///signal(empty)
        s[0].sem_op = 1;
        s[0].sem_flg = 0;
        semop(semid, s, 1);
    }

    shmdt(addr);

    return 0;
}

