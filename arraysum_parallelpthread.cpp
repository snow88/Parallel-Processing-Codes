#include<iostream>
#include<pthread.h>
using namespace std;

int ar[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21};
pthread_t th[4];
int sum[4] = {0};
int thnum = 0;

void* getsum(void* ptr)
{
    int part = thnum++;
    int n = sizeof(ar)/sizeof(int);
    int j = part*(n/4);
    while (true)
    {
        sum[part]+=ar[j];
        j++;
        if (j==(part+1)*n/4 && part!=3)
            break;
        if (j==n)
            break;
    }

}

int main()
{
    for (int i=0; i<4; i++)
        pthread_create(&th[i], NULL, getsum, (void*)NULL);

    for (int i=0; i<4; i++)
        pthread_join(th[i], NULL);

    int totsum = 0;
    for (int i=0; i<4; i++)
        totsum += sum[i];
    cout<<"Array sum is: "<<totsum<<endl;

    return 0;

}
