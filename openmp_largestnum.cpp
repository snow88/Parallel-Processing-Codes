#include<iostream>
#include<omp.h>
using namespace std;
#define N 4                  ///no. of threads

int main()
{
    int ar[] = {20,13,4,56,33,28,91,23,41,57,80,1,11,45,2,7,8};
    int n = sizeof(ar)/sizeof(int);
    int lnum[10];
    int thnum = 0;
    int span = n/N;

    #pragma omp parallel num_threads(N)
    {
        int part;
        #pragma omp critical
        {
            part = thnum++;
        }
        int mx = 0;
        for (int i=part*span; i<(part+1)*span; i++)
        {
            mx = max(mx, ar[i]);
        }
        lnum[part] = mx;
        #pragma omp critical
        {
            cout<<"Max. value for thread "<<part<<" is: "<<mx<<endl;
        }
    }

    int finalmax = 0;
    for (int i=N*span; i<n; i++)
        finalmax = max(finalmax, ar[i]);
    cout<<"Max. value for remaining elements is: "<<finalmax<<endl;

    for(int i=0; i<N; i++)
        finalmax = max(finalmax, lnum[i]);
    cout<<"Final largest number is: "<<finalmax<<endl;

    return 0;
}
