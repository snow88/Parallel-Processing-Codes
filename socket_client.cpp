#include<iostream>
#include<string.h>                  
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>                        
#include<unistd.h>                 
using namespace std;

int main()
{
    int sockid = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in serversockinfo;
    memset(&serversockinfo, 0, sizeof(serversockinfo));         
    serversockinfo.sin_family = AF_INET;
    serversockinfo.sin_port = htons(8000);  
    inet_pton(AF_INET, "127.0.0.1", &serversockinfo.sin_addr);   
                                                              
    connect(sockid, (struct sockaddr*)&serversockinfo, sizeof(serversockinfo));

    const char* msg = "hi from client";                
    send(sockid, msg, strlen(msg), 0);
    cout<<"msg sent from client"<<endl;

    close(sockid);

    return 0;
}
