#include<iostream>
#include<omp.h>
using namespace std;

int main()
{
    int ar[] = {2,6,3,7,9,8,1,4};   
    int n = sizeof(ar)/sizeof(int);
    int localsum[10] = {0};

    #pragma omp parallel num_threads(4)
    {
        int part = omp_get_thread_num();
        int span = n/omp_get_num_threads();
        int s=0;
        #pragma omp parallel for reduction(+:s)
        for (int i=part*span; i<(part+1)*span; i++)
            s+=ar[i];
        localsum[part]=s;
    }

    int globalsum=0;
    #pragma omp parallel for reduction(+:globalsum)
    for (int i=0; i<4; i++)
        globalsum+=localsum[i];

    cout<<"sum "<<globalsum<<endl;

    return 0;
}
