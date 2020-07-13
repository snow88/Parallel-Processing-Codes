#include<iostream>
#include<string.h>
#include<readline/readline.h>
#include<readline/history.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<signal.h>        
using namespace std;

void initshell()
{
    cout<<"----------------MY SHELL----------------"<<endl;
    char *user = getenv("USER");
    cout<<"User: "<<user<<endl;
    cout<<endl;
}

void printdirectory()
{
    char dir[1000];
    getcwd(dir, sizeof(dir));
    cout<<"\n"<<dir<<">"<<endl;
}

int checkspecialcommands(char **commandargs)
{
    int ret = 0;
    if (strcmp(commandargs[0],"cd") == 0)
    {
        chdir(commandargs[1]);
        ret = 1;
    }
    else if (strcmp(commandargs[0],"exit") == 0)
    {
        exit(0);
        ret = 1;
    }
    return ret;
}

void exec(char **commandargs)
{
    int ret = checkspecialcommands(commandargs);
    if (ret == 0)
    {
        int pid = fork();
        if (pid == 0)       
        {
            execvp(commandargs[0], commandargs);         
            exit(0);                                   
        }
        else                
        {
            wait(NULL);       
        }
    }
}

static void sigfunc (int sig)
{
	cout<<"Signal Received: "<<sig<<endl;
    printdirectory();
}

int main()
{
    initshell();
    while(1)
    {
        printdirectory();
        struct sigaction act;
        memset(&act, 0, sizeof(act));
        act.sa_handler = &sigfunc;
        sigaction(SIGQUIT, &act, NULL);
        sigaction(SIGINT, &act, NULL);
        char *command;
        command = readline(">");       
        if (strlen(command) == 0)
            continue;
        char *commandargs[50];
        int i=0;
        while(1)
        {
            commandargs[i] = strsep(&command, " ");
            if (commandargs[i] == NULL)
                break;
            i++;
        }
        i--;
        if (strcmp(commandargs[i],"&") == 0)
            cout<<"running in background..."<<endl;
        exec(commandargs);
    }
    return 0;
}
