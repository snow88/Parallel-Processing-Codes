#include<iostream>
#include<pthread.h>
using namespace std;            
#define M 2                     
#define N 3                     
#define P 4                     /// M >= N_TH should be true
#define N_TH 2

int mata[M][N], matb[N][P], matc[M][P] = {0};
pthread_t th[N_TH];
int thnum = 0;

void* multiply(void* ptr)
{
    int part = thnum++;
    int i = part*(M/N_TH);
    while (true)
    {
        for (int j=0; j<P; j++)           
            for (int k=0; k<N; k++)
                matc[i][j] += mata[i][k]*matb[k][j];

        i++;
        if (i==(part+1)*(M/N_TH) && part!=N_TH-1)
            break;
        if (i==M)
            break;
    }
}

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

    for(int i=0; i<N_TH; i++)
        pthread_create(&th[i], NULL, multiply, (void*)NULL);

    for(int i=0; i<N_TH; i++)
        pthread_join(th[i], NULL);

    printmats();
    cout<<endl;

    return 0;
}
