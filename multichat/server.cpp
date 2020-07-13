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

    listen(sockid, 10);                       ///max. 10 pending requests

    fd_set readfds;                        ///set of file descriptors of all sockets
    int maxclients = 10;
    int client_sockid[10] = {};
    char buf[1024];
    int len = sizeof(sockinfo);

    cout<<"Open to incoming connections..."<<endl;
    while (true)
    {
        FD_ZERO(&readfds);          ///clear descriptors set

        FD_SET(sockid, &readfds);       ///add server to set
        int max_sockid = sockid;

        for (int i=0; i<maxclients; i++)
        {
            if(client_sockid[i] > 0)
                FD_SET(client_sockid[i], &readfds);

            max_sockid = max(max_sockid, client_sockid[i]);
        }

        select(max_sockid+1, &readfds, NULL, NULL, NULL);

        if (FD_ISSET(sockid, &readfds))        ///if fd set status changed => new connection made
        {
            int csid = accept(sockid, (struct sockaddr*)&sockinfo, (socklen_t*)&len);
            for (int i=0; i<maxclients; i++)       ///add csid to first empty loc
            {
                if(client_sockid[i] == 0)
                {
                    client_sockid[i] = csid;
                    break;
                }
            }

            cout<<"New list of clients:"<<endl;
            struct sockaddr_in clientsockinfo;
            int newlen = sizeof(clientsockinfo);
            for (int i=0; i<maxclients; i++)
            {
                if (client_sockid[i] == 0)
                    continue;
                getpeername(client_sockid[i], (struct sockaddr*)&clientsockinfo, (socklen_t*)&newlen);
                cout<<"IP "<<clientsockinfo.sin_addr.s_addr<<" Port "<<clientsockinfo.sin_port<<endl;
            }
        }

        for (int i=0; i<maxclients; i++)      ///state of a client socket changed => some I/O happened
        {
            if (FD_ISSET(client_sockid[i], &readfds))
            {
                memset(&buf, 0, sizeof(buf));
                int ret = recv(client_sockid[i], buf, 1024, 0);
                if (ret == 0)                                        ///client i disconnected
                {
                    cout<<"Disconnection detected."<<endl;
                    close(client_sockid[i]);
                    client_sockid[i] = 0;

                    cout<<"New list of clients:"<<endl;
                    struct sockaddr_in clientsockinfo;
                    int newlen = sizeof(clientsockinfo);
                    for (int i=0; i<maxclients; i++)
                    {
                        if (client_sockid[i] == 0)
                            continue;
                        getpeername(client_sockid[i], (struct sockaddr*)&clientsockinfo, (socklen_t*)&newlen);
                        cout<<"IP "<<clientsockinfo.sin_addr.s_addr<<" Port "<<clientsockinfo.sin_port<<endl;
                    }
                }
                else                   ///client i sent a msg
                {
                    cout<<"Received: "<<buf<<endl;
                    for (int i=0; i<maxclients; i++)
                    {
                        if (client_sockid[i] == 0)
                            continue;
                        send(client_sockid[i], buf, sizeof(buf), 0);
                    }
                }
            }
        }
    }

    return 0;
}
