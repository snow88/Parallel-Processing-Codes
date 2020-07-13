#include<iostream>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<string.h>
#include<wait.h>
using namespace std;

int execute(char* commandargs[])
{
    int ret = 0;

    if (strcmp(commandargs[0], "c_ls") == 0)
    {
        int pid = fork();
        if (pid == 0)
        {
            char* cmd[] = {"ls", NULL};
            execvp(cmd[0], cmd);
            exit(0);
        }
        else
            wait(NULL);
    }
    else if (strcmp(commandargs[0], "c_pwd") == 0)
    {
        int pid = fork();
        if (pid == 0)
        {
            char* cmd[] = {"pwd", NULL};
            execvp(cmd[0], cmd);
            exit(0);
        }
        else
            wait(NULL);
    }
    else if (strcmp(commandargs[0], "c_cd") == 0)
    {
        chdir(commandargs[1]);
        char dir[1024];
        getcwd(dir, sizeof(dir));
        cout<<"Directory changed to: "<<dir<<endl;
    }
    else
        ret = -1;

    return ret;
}

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        cout<<"IP Address or Port Address not specified."<<endl;
        exit(1);
    }
    char* ip = argv[1];                
    char* port = argv[2];
    if (strlen(port)!=4)             
    {
        cout<<"IP Address and Port Address not specified in correct order."<<endl;
        exit(1);
    }

    int sockid = socket(AF_INET, SOCK_STREAM, 0);
    if (sockid == -1)
    {
        perror("socket creation failed.");
        exit(1);
    }

    struct sockaddr_in srvr;
    memset(&srvr, 0, sizeof(srvr));
    srvr.sin_family = AF_INET;
    srvr.sin_port = htons(stoi(port));
    inet_pton(AF_INET, ip, &srvr.sin_addr);

    while (connect(sockid, (struct sockaddr*)&srvr, sizeof(srvr)) == -1);

    char hostname[1024];
    gethostname(hostname, sizeof(hostname));
    char* rollno = (char*)"2016UCO1588";

    char buf[1024];
    while (true)
    {
        cout<<"\n"<<rollno<<"_"<<hostname<<" > ";
        memset(&buf, 0, sizeof(buf));
        cin.getline(buf, 1024);
        char* command = buf;
        char* commandargs[10];
        int i=0;
        while(true)
        {
            commandargs[i] = strsep(&command, " ");
            if (commandargs[i] == NULL)
                break;
            i++;
        }
        i--;
        int ret = execute(commandargs);
        if (ret == 0)
            continue;

        send(sockid, buf, strlen(buf), 0);

        if (strcmp(buf, "quit") == 0)
            break;
        if (strcmp(buf, "put") == 0)
        {

            continue;
        }
        if (strcmp(buf, "get") == 0)
        {

            continue;
        }

        memset(&buf, 0, sizeof(buf));
        recv(sockid, buf, 1024, 0);
        cout<<"> received:\n"<<buf<<endl;

    }

    close(sockid);

    return 0;
}
