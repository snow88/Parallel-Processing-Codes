#include<iostream>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>
using namespace std;

int main()
{
    int sockid = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in serversockinfo;
    memset(&serversockinfo, 0, sizeof(serversockinfo));
    serversockinfo.sin_family = AF_INET;
    serversockinfo.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &serversockinfo.sin_addr);

    cout<<"Client >> requesting connection..."<<endl;
    while (connect(sockid, (struct sockaddr*)&serversockinfo, sizeof(serversockinfo)) < 0);

    char buf[1024];
    while (true)
    {
        cout<<"Client >> enter message to send: ";
        memset(&buf, 0, sizeof(buf));                       
        cin.getline(buf,1024);
        send(sockid, buf, strlen(buf), 0);
        if (strcmp(buf, "close") == 0)
            break;

        memset(&buf, 0, sizeof(buf));
        recv(sockid, buf, 1024, 0);
        cout<<"Client >> received: "<<buf<<endl;
        if (strcmp(buf, "close") == 0)
            break;
    }

    close(sockid);

    return 0;
}

