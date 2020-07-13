#include<iostream>
#include<mpi.h>
using namespace std;

int main(int argc, char* argv[])
{
    int ar[] = {20,13,4,56,33,28,91,23,41,57,80,1,11,45,2,7};
    int n = sizeof(ar)/sizeof(int);

    MPI_Init(&argc, &argv);             

    int r, s;
    MPI_Comm_rank(MPI_COMM_WORLD, &r);
    MPI_Comm_size(MPI_COMM_WORLD, &s);

    int span = n/s;          

    int sum = 0;
    for (int i=r*span; i<(r+1)*span ; i++)
    {
        sum += ar[i];
    }

    if (r == 0)
    {
        for (int i=s*span; i<n; i++)
            sum += ar[i];
        cout<<"Sum computed by processor 0 is: "<<sum<<endl;

        for(int i=0; i<s-1; i++)
        {
            int part_sum = 0;
            MPI_Status st;
            MPI_Recv(&part_sum, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &st);  
            int sender = st.MPI_SOURCE;
            cout<<"Sum computed by processor "<<sender<<" is: "<<part_sum<<endl;
            sum += part_sum;
        }

        cout<<"Final sum is: "<<sum<<endl;
    }
    else
    {
        MPI_Send(&sum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);  
    }

    MPI_Finalize();

    return 0;
}
