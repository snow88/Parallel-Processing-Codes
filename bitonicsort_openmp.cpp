#include<iostream>
#include<omp.h>
using namespace std;

void merge_up(int ar[], int n)           ///sort bitonic input of size n
{
    int compdist = n/2;                ///comparison distance; start with largest; reduces by powers of 2
    while (compdist > 0)
    {
        #pragma omp parallel for
        for(int i=0; i<n-compdist; i++)           
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

void merge_down(int ar[], int n)
{
    int compdist = n/2;
    while (compdist > 0)
    {
        #pragma omp parallel for
        for(int i=0; i<n-compdist; i++)
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

int main()
{
    int ar[] = {5,6,2,7,1,8,0,2};                ///length should be power of 2
    int n = sizeof(ar)/sizeof(int);

    for (int step=2; step<=n; step=step*2)
    {
        #pragma omp parallel for
        for (int i=0; i<n; i=i+step*2)
        {
            merge_up((ar+i),step);
            if (step != n)
                merge_down((ar+i+step),step);
        }
    }

    for (int i=0; i<n; i++)
        cout<<ar[i]<<" ";

    return 0;
}
