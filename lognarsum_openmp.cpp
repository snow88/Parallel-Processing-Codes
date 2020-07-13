#include<iostream>
#include<omp.h>
#include<math.h>
using namespace std;

int main()
{
    int ar[] = {2,6,3,7,9,8,1,4};    
    int n = sizeof(ar)/sizeof(int);

    int sum[10];
    int span = log2(n);
    int processors = ceil(n/log2(n));
    #pragma omp parallel num_threads(processors)     ///global phase
    {
        int s = 0;
        int part = omp_get_thread_num();
        #pragma omp parallel for reduction(+:s)
        for (int i=part*span; i<(part+1)*span; i++) {   
            if (i < n)                                
                s+=ar[i];
        }
        sum[part] = s;
    }

    int nn = processors;
    int step = 1;
    while (nn>1)                              ///local phase
    {
        #pragma omp parallel for
        for (int i=0; i<nn-1; i=i+2*step)
            sum[i] = sum[i]+sum[i+step];
        nn = ceil((float)nn/2);
        step = step*2;
    }

    cout<<"sum "<<sum[0]<<endl;

    return 0;
}
