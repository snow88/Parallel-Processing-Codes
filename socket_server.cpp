#include<iostream>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>            
using namespace std;

int main()
{
    int sockid = socket(AF_INET, SOCK_STREAM, 0);      

    int option_val = 1;
    setsockopt(sockid, SOL_SOCKET, SO_REUSEADDR|SO_REUSEPORT, &option_val, sizeof(option_val));

    struct sockaddr_in sockinfo;
    sockinfo.sin_family = AF_INET;
    sockinfo.sin_port = htons(8000);        
    sockinfo.sin_addr.s_addr = INADDR_ANY;

    bind(sockid, (struct sockaddr*)&sockinfo, sizeof(sockinfo));

    listen(sockid, 1);              

    int len = sizeof(sockinfo);
    int clientsockid = accept(sockid, (struct sockaddr*)&sockinfo, (socklen_t*)&len);
    cout<<"client id: "<<clientsockid<<endl;

    char buf[1024] = {0};                      
    int bytes = recv(clientsockid, buf, 1024, 0);
    cout<<"received: "<<buf<<endl;
    cout<<"bytes read: "<<bytes<<endl;

    close(sockid);

    return 0;
}

