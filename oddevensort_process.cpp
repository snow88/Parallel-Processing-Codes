#include<iostream>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<utility>
using namespace std;

int *n, *ar;

void init()
{
    key_t key = ftok("shmfile", 65);
    int shmid = shmget(key, 1024, 0666|IPC_CREAT);
    void* addr = shmat(shmid, (void*)0, 0);

    n = (int*)addr;
    cout<<"Enter no. of terms: ";
    cin>>*n;
    ar = n+1;
    cout<<"Enter terms: ";
    for (int i=0; i<*n; i++)
        cin>>ar[i];
}

int main()
{
    init();

    for (int i=0; i<*n; i++)
    {
        int pid = fork();

        if (i%2 == 0 && pid == 0)                              ///even pass, child
        {
            int j = *n/2;
            if (j%2 != 0)
            j++;
            while(true)
            {
                if (ar[j+1] < ar[j])
                    swap(ar[j], ar[j+1]);
                j = j+2;
                if (j >= *n || j+1 >= *n)
                    break;
            }
            exit(0);
        }
        else if (i%2 == 0 && pid != 0)                         ///even pass, parent
        {
            int j = 0;
            while(true)
            {
                if (ar[j+1] < ar[j])
                    swap(ar[j], ar[j+1]);
                j = j+2;
                if (j >= *n/2)
                    break;
            }
            wait(NULL);
        }
        else if (i%2 != 0 && pid == 0)                         ///odd pass, child
        {
            int j = *n/2;
            if (j%2 == 0)
                j++;
            while(true)
            {
                if (ar[j+1] < ar[j]) {
                    swap(ar[j], ar[j+1]); }
                j = j+2;
                if (j >= *n || j+1 >= *n)
                    break;
            }
            exit(0);
        }
        else if (i%2 != 0 && pid != 0)                          ///odd pass, parent
        {
            int j = 1;
            while(true)
            {
                if (ar[j+1] < ar[j])
                    swap(ar[j], ar[j+1]);
                j = j+2;
                if (j >= *n/2)
                    break;
            }
            wait(NULL);
        }
    }

    cout<<"Sorted Array:"<<endl;
    for (int i=0; i<*n; i++)
        cout<<ar[i]<<" ";
    cout<<endl;

    return 0;
}

