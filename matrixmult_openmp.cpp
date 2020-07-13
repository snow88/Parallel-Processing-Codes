#include<iostream>
#include<omp.h>
using namespace std;
#define M 2                     ///number of rows should be greater than or equal to the number of threads
#define N 3                     
#define P 4                    
#define N_TH 2

int mata[M][N], matb[N][P], matc[M][P] = {0};

void generatemats()
{
    for (int i=0; i<M; i++)
        for (int j=0; j<N; j++)
            mata[i][j] = rand()%10;

    for (int i=0; i<N; i++)
        for (int j=0; j<P; j++)
            matb[i][j] = rand()%10;
}

void printmats()
{
    cout<<"\nMatrix A:"<<endl;
    for (int i=0; i<M; i++)
    {
        for (int j=0; j<N; j++)
            cout<<mata[i][j]<<" ";
        cout<<endl;
    }

    cout<<"\nMatrix B:"<<endl;
    for (int i=0; i<N; i++)
    {
        for (int j=0; j<P; j++)
            cout<<matb[i][j]<<" ";
        cout<<endl;
    }

    cout<<"\nMatrix C:"<<endl;
    for (int i=0; i<M; i++)
    {
        for (int j=0; j<P; j++)
            cout<<matc[i][j]<<" ";
        cout<<endl;
    }
}

int main()
{
    generatemats();
    int span = M/N_TH;

    #pragma omp parallel num_threads(N_TH)
    {
        int part = omp_get_thread_num();
        int i = part*span;
        while (true)
        {
            for (int j=0; j<P; j++)             
                for (int k=0; k<N; k++)
                    matc[i][j] += mata[i][k]*matb[k][j];

            i++;
            if (i==(part+1)*span && part!=N_TH-1)
                break;
            if (i==M)
                break;
        }
    }

    printmats();
    return 0;
}
