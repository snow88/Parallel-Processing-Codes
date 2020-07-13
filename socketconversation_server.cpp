#include<iostream>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<string.h>
using namespace std;

int main()
{
    int sockid = socket(AF_INET, SOCK_STREAM, 0);

    int opt = 1;
    setsockopt(sockid, SOL_SOCKET, SO_REUSEADDR|SO_REUSEPORT, &opt, sizeof(opt));

    struct sockaddr_in sockinfo;
    sockinfo.sin_family = AF_INET;
    sockinfo.sin_port = htons(8080);
    sockinfo.sin_addr.s_addr = INADDR_ANY;
    bind(sockid, (struct sockaddr*)&sockinfo, sizeof(sockinfo));

    listen(sockid, 5);

    int len = sizeof(sockinfo);
    int clientsockid = accept(sockid, (struct sockaddr*)&sockinfo, (socklen_t*)&len);
    cout<<"Server >> received connection request"<<endl;
    struct sockaddr_in clientsockinfo;
    int newlen = sizeof(clientsockinfo);
    getpeername(clientsockid, (struct sockaddr*)&clientsockinfo, (socklen_t*)&newlen);
    cout<<"client IP is "<<clientsockinfo.sin_addr.s_addr<<endl;
    cout<<"client Port is "<<clientsockinfo.sin_port<<endl;
    cout<<"Server >> connection accepted."<<endl;

    char buf[1024];
    while (true)
    {
        memset(&buf, 0, sizeof(buf));              
        recv(clientsockid, buf, 1024, 0);
        cout<<"Server >> received: "<<buf<<endl;
        if (strcmp(buf, "close") == 0)
            break;

        cout<<"Server >> enter message to send: ";
        memset(&buf, 0, sizeof(buf));
        cin.getline(buf, 1024);
        send(clientsockid, buf, strlen(buf), 0);
        if (strcmp(buf, "close") == 0)
            break;
    }

    close(sockid);

    return 0;
}
