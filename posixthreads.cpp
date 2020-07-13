#include<iostream>
#include<pthread.h>
#include<semaphore.h>
#include<sys/time.h>
using namespace std;

pthread_mutex_t mutex;
sem_t psem, csem;
int val;
timeval tv, tvcur;

void init()
{
    mutex = PTHREAD_MUTEX_INITIALIZER;
    sem_init(&psem, 0, 0);    
    sem_init(&csem, 0, 1);
    gettimeofday(&tv, 0);
}

void *producer(void* ptr)       
{
    while(true)
    {
        gettimeofday(&tvcur, 0);
        if (tvcur.tv_sec-tv.tv_sec > 5)
            break;

        sem_wait(&csem);
        pthread_mutex_lock(&mutex);
        val = rand()%1000;
        cout<<"Value Produced: "<<val<<endl;
        pthread_mutex_unlock(&mutex);
        sem_post(&psem);
    }
}

void *consumer(void *ptr)
{
    while(true)
    {
        sem_wait(&psem);
        pthread_mutex_lock(&mutex);
        cout<<"Value Consumed: "<<val<<endl;
        val = 0;
        pthread_mutex_unlock(&mutex);
        sem_post(&csem);

        gettimeofday(&tvcur, 0);
        if (tvcur.tv_sec-tv.tv_sec > 5)
            break;
    }
}

int main()
{
    init();

    pthread_t prodth, consth;
    const char *msgp = "Producer Thread";
    const char *msgc = "Consumer Thread";
    int pret = pthread_create(&prodth, NULL, producer, (void*)msgp);
    int cret = pthread_create(&consth, NULL, consumer, (void*)msgc);

    pthread_join(prodth, NULL);
    pthread_join(consth, NULL);

    pthread_mutex_destroy(&mutex);
    sem_destroy(&psem);
    sem_destroy(&csem);

    return 0;
}
