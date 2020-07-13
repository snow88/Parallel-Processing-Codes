#include<iostream>
#include<omp.h>
using namespace std;
#define N 4                     ///no. of threads

int main()
{
    int ar[] = {20,13,4,56,33,28,91,23,41,57,80,1,11,45,2,7,8};
    int n = sizeof(ar)/sizeof(int);
    int sum[10] = {0};            
    int thnum = 0;
    int span = n/N;

    #pragma omp parallel num_threads(N)
    {
        int part;
        #pragma omp critical
        {
            part = thnum++;
        }
        int s = 0;
        for (int i=part*span; i<(part+1)*span; i++)
        {
            s += ar[i];
        }
        sum[part] = s;
        #pragma omp critical
        {
            cout<<"Sum by thread "<<part<<" is: "<<s<<endl;
        }
    }

    int totsum = 0;
    for(int i=N*span; i<n; i++)
        totsum += ar[i];
    cout<<"Remaining elements sum is: "<<totsum<<endl;

    for (int i=0; i<N; i++)
        totsum += sum[i];
    cout<<"Total sum is: "<<totsum<<endl;

    return 0;
}
