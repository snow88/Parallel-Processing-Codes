#include<iostream>
#include<pthread.h>
#include<utility>
using namespace std;

pthread_t th[10];
int ar[] = {6,1,2,4,3,8,0,2};
int n = sizeof(ar)/sizeof(n);

void unshuffle(int *ar, int b, int l)
{
    int m = b+1;
    int ns = (l-b+1)/2 - 1;        ///no. of comparisons = no.of elements/2 - 1
    while (m <= (b+l)/2)
    {
        for (int i=m, n=0; n<ns ; i=i+2, n++)
            swap(ar[i], ar[i+1]);
        ns--;
        m++;
    }
}

void shuffle(int *ar, int b, int l)
{
    int m = (b+l)/2;
    int ns = 1;      
    while (m>b)
    {
        for (int i=m, n=0; n<ns ; i=i+2, n++)
            swap(ar[i], ar[i+1]);
        ns++;
        m--;
    }
}

void comparators(int *ar, int b, int l)
{
    for (int i=b+1; i<l; i=i+2)
    {
        if (ar[i]>ar[i+1])
            swap(ar[i], ar[i+1]);
    }
}

void batchermerge(int *ar, int b, int l)
{
    cout<<b<<" "<<l<<endl;
    for(int i=0; i<n; i++)
        cout<<ar[i]<<" ";
    cout<<endl;
    if (l == b+1) {                 ///2 element base case, just a comparator
        if (ar[b]>ar[l])
            swap(ar[b],ar[l]);
        return;
    }
    unshuffle(ar,b,l);
    int mid = (b+l)/2;
    batchermerge(ar,b,mid);
    batchermerge(ar,mid+1,l);
    shuffle(ar,b,l);
    comparators(ar,b,l);
}

int thnum;
int step;                   
void* batchermerge(void *p)
{
    int part = thnum++;
    batchermerge(ar,part*step, (part+1)*step-1);
}

int main()
{
    for (step=2; step<=n; step*=2)
    {
        thnum=0;                       
        for(int i=0; i<n/step; i++)
            pthread_create(&th[i], NULL, batchermerge, (void*)NULL);
        for(int i=0; i<n/step; i++)
            pthread_join(th[i], NULL);      
    }

    for(int i=0; i<n; i++)
        cout<<ar[i]<<" ";

    return 0;
}
