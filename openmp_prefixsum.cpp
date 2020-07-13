#include<iostream>           
#include<fstream>
#include<omp.h>
#include<sys/time.h>
using namespace std;

int main(int argc, char** argv)
{
    int np = stoi(argv[1]);            ///get no. of threads

    ofstream fout;
    fout.open("openmp.txt", ios::out);

    int n;
    cout<<"Enter no. of terms: ";             ///divisible by no. of threads
    cin>>n;
    int ar[100];
    for (int i=0; i<n; i++)
        ar[i] = rand()%100;

    int prefixsum[100] = {};
    int span = n/np;
                         
    timeval tvstart;
    gettimeofday(&tvstart, 0);

    #pragma omp parallel num_threads(np)
    {
        int part = omp_get_thread_num();
        prefixsum[part*span] = ar[part*span];
        for (int i=part*span+1; i<(part+1)*span; i++)
        {
            prefixsum[i] = prefixsum[i-1] + ar[i];
        }
    }

    int z[50] = {};
    for (int i=1; i<=np; i++)
    {
        int st = i*span-1;
        #pragma omp for
        for (int j=st; j<n; j+=span)
            z[j] += prefixsum[st];
    }

    #pragma omp parallel num_threads(np)
    {
        int part = omp_get_thread_num();
        for (int i=part*span; i<(part+1)*span-1; i++)
        {
            prefixsum[i] = prefixsum[i] + z[(part+1)*span-1] - prefixsum[(part+1)*span-1];
        }
        prefixsum[(part+1)*span-1] = z[(part+1)*span-1];
    }

    timeval tvend;
    gettimeofday(&tvend, 0);
    fout<<"Time taken: "<<tvend.tv_usec-tvstart.tv_usec<<" microseconds"<<endl;

    fout<<"Input:"<<endl;
    for (int i=0; i<n; i++)
        fout<<ar[i]<<" ";
    fout<<"\nOutput:"<<endl;
    for (int i=0; i<n; i++)
        fout<<prefixsum[i]<<" ";

    cout<<"Output file generated."<<endl;
    fout.close();
    return 0;
}
