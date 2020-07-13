#include<iostream>
#include<pthread.h>
#include<utility>
using namespace std;

int ar[] = {5,8,9,8,0,4,33,2,77,3,1,2,9,7,89,6};
int n = sizeof(ar)/sizeof(int);
pthread_t th[4];
int thnum = 0;

void* evpass(void* ptr)
{
    int part = thnum++;

    int j = part*(n/4);
    if (j%2 != 0)
        j++;
    while(true)
    {
        if (ar[j+1] < ar[j])
            swap(ar[j], ar[j+1]);
        j = j+2;
        if (j>=(part+1)*n/4 && part!=3)
            break;
        if (j >= n || j+1 >= n)
            break;
    }
}

void* odpass(void* ptr)
{
    int part = thnum++;

    int j = part*(n/4);
    if (j%2 == 0)
        j++;
    while(true)
    {
        if (ar[j+1] < ar[j])
            swap(ar[j], ar[j+1]);
        j = j+2;
        if (j>=(part+1)*n/4 && part!=3)
            break;
        if (j >= n || j+1 >= n)
            break;
    }
}

int main()
{
    for (int i=0; i<n; i++)
    {
        thnum = 0;
        if (i%2 == 0)                              ///even pass
        {
            for (int i=0; i<4; i++)
                pthread_create(&th[i], NULL, evpass, (void*)NULL);
            for (int i=0; i<4; i++)
                pthread_join(th[i], NULL);
        }
        else                                         ///odd pass
        {
            for (int i=0; i<4; i++)
                pthread_create(&th[i], NULL, odpass, (void*)NULL);
            for (int i=0; i<4; i++)
                pthread_join(th[i], NULL);
        }
    }

    cout<<"Sorted Array:"<<endl;
    for (int i=0; i<n; i++)
        cout<<ar[i]<<" ";
    cout<<endl;

    return 0;
}
