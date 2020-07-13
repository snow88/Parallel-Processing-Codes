#include<iostream>
#include<fstream>
#include<mpi.h>
#include<sys/ipc.h>
#include<sys/types.h>
#include<sys/shm.h>
#include<sys/time.h>
using namespace std;

int main(int argc, char** argv)
{                                         
    MPI_Init(&argc, &argv);
    int r, s;
    MPI_Comm_rank(MPI_COMM_WORLD, &r);
    MPI_Comm_size(MPI_COMM_WORLD, &s);

    key_t key = ftok("shmfile", 65);
    int shmid = shmget(key, 1024, 0666|IPC_CREAT);
    void* addr = shmat(shmid, (void*)0, 0);

    int* n = (int*)addr;
    int* span = n+1;
    int* ar = span+1;
    int* prefixsum = ar+50;          ///max 50 length ar
    int* z = prefixsum+50;            ///max 50 length prefixsum

    ofstream fout;
    timeval tvstart;

    if (r == 0)
    {
        fout.open("mpi.txt", ios::out);

        cout<<"Enter no. of terms: ";             ///divisible by no. of threads
        cin>>*n;
        for (int i=0; i<*n; i++)
            ar[i] = rand()%100;

        *span = *n/s;
                           
        gettimeofday(&tvstart, 0);
    }

    MPI_Barrier(MPI_COMM_WORLD);

    prefixsum[r*(*span)] = ar[r*(*span)];
    for (int i=r*(*span)+1; i<(r+1)*(*span); i++)
    {
        prefixsum[i] = prefixsum[i-1] + ar[i];
    }

    MPI_Barrier(MPI_COMM_WORLD);

    if (r == 0)
    {
        for (int i=1; i<=s; i++)
        {
            int st = i*(*span)-1;
            for (int j=st; j<*n; j+=(*span))
                z[j] += prefixsum[st];
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);

    for (int i=r*(*span); i<(r+1)*(*span)-1; i++)
    {
        prefixsum[i] = prefixsum[i] + z[(r+1)*(*span)-1] - prefixsum[(r+1)*(*span)-1];
    }
    prefixsum[(r+1)*(*span)-1] = z[(r+1)*(*span)-1];

    MPI_Barrier(MPI_COMM_WORLD);

    if (r == 0)
    {
        timeval tvend;
        gettimeofday(&tvend, 0);
        fout<<"Time taken: "<<tvend.tv_usec-tvstart.tv_usec<<" microseconds"<<endl;

        fout<<"Input:"<<endl;
        for (int i=0; i<*n; i++)
            fout<<ar[i]<<" ";
        fout<<"\nOutput:"<<endl;
        for (int i=0; i<*n; i++)
            fout<<prefixsum[i]<<" ";

        cout<<"Output file generated."<<endl;
        fout.close();
    }

    MPI_Finalize();
    shmctl(shmid, IPC_RMID, NULL);      
    return 0;
}

