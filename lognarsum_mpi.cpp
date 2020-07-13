#include<iostream>
#include<mpi.h>
#include<math.h>
using namespace std;

int main(int argc, char** argv)         ///call with -np (ceil(n/logn) processors)
{
    MPI_Init(&argc, &argv);
    int r,s;
    MPI_Comm_rank(MPI_COMM_WORLD, &r);
    MPI_Comm_size(MPI_COMM_WORLD, &s);

    int ar[] = {2,6,3,7,9,8,1,4};    
    int n = sizeof(ar)/sizeof(int);

    int span = log2(n);             
    int localsum=0;
    for (int i=r*span; i<(r+1)*span && i<n; i++)
        localsum+=ar[i];

    int globalsum=0;
    MPI_Reduce(&localsum, &globalsum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (r==0)
        cout<<"sum "<<globalsum<<endl;

    MPI_Finalize();
    return 0;
}
