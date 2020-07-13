#include<iostream>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>
#include<pthread.h>             
using namespace std;

int sockid;

void *thsend(void *ptr)
{
    char buf[1024];
    while (true)
    {
        cout<<"Client >> Enter message to send: ";
        memset(&buf, 0, sizeof(buf));                      
        cin.getline(buf,1024);
        send(sockid, buf, strlen(buf), 0);
        if (strcmp(buf, "close") == 0)
            break;
    }
}

void *threcv(void *ptr)
{
    char buf[1024];
    while (true)
    {
        memset(&buf, 0, sizeof(buf));               
        recv(sockid, buf, 1024, 0);
        cout<<"Client >> Received: "<<buf<<endl;
        if (strcmp(buf, "close") == 0)
            break;
    }
}

int main()
{
    sockid = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in serversockinfo;
    memset(&serversockinfo, 0, sizeof(serversockinfo));
    serversockinfo.sin_family = AF_INET;
    serversockinfo.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &serversockinfo.sin_addr);

    cout<<"Requesting connection..."<<endl;
    while (connect(sockid, (struct sockaddr*)&serversockinfo, sizeof(serversockinfo)) < 0);

    pthread_t ts, tr;
    pthread_create(&ts, NULL, thsend, NULL);
    pthread_create(&tr, NULL, threcv, NULL);

    pthread_join(ts, NULL);
    pthread_join(tr, NULL);

    close(sockid);

    return 0;
}


