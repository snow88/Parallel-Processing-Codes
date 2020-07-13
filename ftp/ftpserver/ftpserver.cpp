#include<iostream>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<string.h>
using namespace std;

string getcommandoutput(char* command)
{
    string str;
    FILE *f = popen(command, "r");
    char buffer[1024];
    while (!feof(f))
    {
        if(fgets(buffer, 1024, f) != NULL)
            str.append(buffer);
    }
    pclose(f);
    return str;
}

int main(int argc, char* argv[])
{
    if (argc!=2 || strlen(argv[1])!=4)
    {
        cout<<"Port Address not specified correctly."<<endl;
        exit(1);
    }
    char* port = argv[1];

    int sockid = socket(AF_INET, SOCK_STREAM, 0);
    if (sockid == -1)
    {
        perror("socket creation failed.");
        exit(1);
    }

    int opt = 1;
    if (setsockopt(sockid, SOL_SOCKET, SO_REUSEADDR|SO_REUSEPORT, &opt, sizeof(opt)) == -1)
    {
        perror("setsockopt failed.");
        exit(1);
    }

    struct sockaddr_in sinfo;
    sinfo.sin_family = AF_INET;
    sinfo.sin_port = htons(stoi(port));           
    sinfo.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockid, (struct sockaddr*)&sinfo, sizeof(sinfo)) == -1)
    {
        perror("bind failed.");
        exit(1);
    }

    if (listen(sockid, 5) == -1)
    {
        perror("listen failed.");
        exit(1);
    }

    int len = sizeof(sinfo);
    int cskid = accept(sockid, (struct sockaddr*)&sinfo, (socklen_t*)&len);
    if (cskid == -1)
    {
        perror("accept failed.");
        exit(1);
    }

    char buf[1024];
    while (true)
    {
        memset(&buf, 0, sizeof(buf));
        recv(cskid, buf, 1024, 0);
        cout<<"\nserver > received: "<<buf<<endl;

        if (strcmp(buf, "quit") == 0)
            break;
        if (strcmp(buf, "s_ls") == 0)
        {
            string s = getcommandoutput((char*)"ls");
            send(cskid, s.c_str(), strlen(s.c_str()), 0);
            cout<<"data sent."<<endl;
            continue;
        }
        if (strcmp(buf, "s_pwd") == 0)
        {
            string s = getcommandoutput((char*)"pwd");
            send(cskid, s.c_str(), strlen(s.c_str()), 0);
            cout<<"data sent."<<endl;
            continue;
        }

        char* commandargs[10];
        int i=0;
        while(true)
        {
            commandargs[i] = strsep((char**)&buf, " ");
            if (commandargs[i] == NULL)
                break;
            i++;
        }
        i--;

        if (strcmp(commandargs[0], "s_cd") == 0)
        {
            chdir(commandargs[1]);
            char* msg = (char*)"directory changed at server.";
            send(cskid, msg, strlen(msg), 0);
            char dir[1024];
            getcwd(dir, sizeof(dir));
            cout<<"Directory changed to: "<<dir<<endl;
            continue;
        }
        if (strcmp(commandargs[0], "put") == 0)
        {

            continue;
        }
        if (strcmp(commandargs[0], "get") == 0)
        {

            continue;
        }
        char* msg = (char*)"Invalid command.";
        send(cskid, msg, strlen(msg), 0);
    }

    close(sockid);

    return 0;
}
