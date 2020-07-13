#include<iostream>
#include<mpi.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>
using namespace std;

void merge_up(int* ar, int n)           ///sort bitonic input of size n
{
    int compdist = n/2;                ///comparison distance; start with largest; reduces by powers of 2
    while (compdist > 0)
    {
        for(int i=0; i+compdist<n; i++)
        {
            if (ar[i] > ar[i+compdist])
            {
                int temp = ar[i];
                ar[i]=ar[i+compdist];
                ar[i+compdist]=temp;
            }
        }
        compdist /= 2;
    }
}

void merge_down(int* ar, int n)
{
    int compdist = n/2;
    while (compdist > 0)
    {
        for(int i=0; i+compdist<n; i++)
        {
            if (ar[i] < ar[i+compdist])              
            {
                int temp = ar[i];
                ar[i]=ar[i+compdist];
                ar[i+compdist]=temp;
            }
        }
        compdist /= 2;
    }
}

int main(int argc, char** argv)
{
    MPI_Init(&argc, &argv);
    int r,s;
    MPI_Comm_rank(MPI_COMM_WORLD, &r);
    MPI_Comm_size(MPI_COMM_WORLD, &s);

    key_t key = ftok("shmfile", 65);
    int shmid = shmget(key, 1024, 0666|IPC_CREAT);
    void* addr = shmat(shmid, (void*)0, 0);

    int* n = (int*)addr;
    int* ar = n+1;

    if (r == 0)
    {
        *n = 8;                        ///length should be power of 2
        for (int i=0; i<*n; i++)
            ar[i] = rand()%100;
    }

    MPI_Barrier(MPI_COMM_WORLD);

    for (int step=2; step<=*n; step=step*2)
    {
        for (int i=0; i<*n; i=i+step*2)                       ///no. of processors must be >= n/4
        {
            if (r != i/4)
                continue;
            merge_up((ar+i),step);
            if (step != *n)
                merge_down((ar+i+step),step);
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }
    MPI_Barrier(MPI_COMM_WORLD);

    if (r == 0)
    {
    for (int i=0; i<*n; i++)
        cout<<ar[i]<<" ";
    }

    MPI_Finalize();
    return 0;
}

