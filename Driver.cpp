#include <iostream>
#include "filehandler.h"
#include <bits/stdc++.h>
#include<sys/wait.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
using namespace std;
int customcommand(char *command, char *arguments[]){
    int l = 2, i = 0, choice=10,c=0;
    char* custom[l];
    custom[0] = "echo";
    custom[1] = "cd";
    if(!command){
        return c;
    }
    for (i = 0; i < l; i++) {
        if (strcmp(command, custom[i]) == 0) {
            choice = i;
            break;
        }
    }
    switch(choice){
        case 0:
            char env[10];
            if(!arguments[1] || arguments[1][0] != '$'){
                c=0;
                break;
            }
            else{
                strcpy(env, arguments[1]+1);
                if(!getenv(env)){
                    cout << "Cant access variable"<<endl;
                }
                else{
                    cout << getenv(env);
                }
            }
            c=1;
            break;
        case 1:
            char cwd[1000];
            if (getcwd(cwd, sizeof(cwd)) != NULL) {
            //printf("Current working dir: %s\n", cwd);
            if(!arguments[1]){
                cout << "Current working dir: " << cwd << endl;
            }
            else if(strcmp(arguments[1],"..")==0){
                cout << "GO BACK" <<endl;
            }
            else{
            strcat(cwd, arguments[1]);
            cout << cwd << endl;
            chdir(cwd);
            }
            }
            c = 1;
            break;
        default:
            break;
        }
        return c;
}
void forkandexec(char *command, char *arguments[]){  //Forks child and executs command
    pid_t p = fork();
    if (p == 0) {
            int execute = execvp(command, arguments);
            if (execute < 0) {
                printf("\nCould not execute command..");
            }
        exit(0);
    }
    else{
        wait(NULL);
        return;
    }
}
void welcomenote(){
    string username;
    cout << "Welcome to My shell"<<endl;
    username = getenv("USER");
    cout << username <<endl;
}
void splitcommands(string str){
    char* token;
    char arr[str.length()+1];
    strcpy(arr, str.c_str());
    arr[str.length()] = '\0';
    token = strtok(arr, " ");
    char *command = token;
    char *argu[1000];int i=1;
    argu[0] = command;
    while(token != 0){
        token = strtok(0, " ");
        argu[i] = token;
        i++;
    }
    if(!customcommand(command, argu)){
          forkandexec(command, argu);
    }
}
bool ispiped(string str){
    int i=0;
    while(i < str.length()){
        if (str[i] == '|'){
            return true;
        }
        i++;
    }
    return false;
}
void forkandexecPipe(char **parts, int l){
    int j;
    char *argu[1000];char *token;
    pid_t x = fork();
    if(x==0){
    for( j=0; j<(l-1); j++){
        int pd[2];
        pipe(pd);
        pid_t p = fork();
        if (p == 0) {
            dup2(pd[1], 1);
                        token = strtok(parts[j], " ");
                        char *command = token;
                        char *argu[1000];int i=1;
                        argu[0] = command;
                        while(token != 0){
                        //cerr << token << " ";
                            token = strtok(0, " ");
                            argu[i] = token;
                            i++;
                        }
                        argu[i-1]=NULL;
            int execute = execvp(command, argu);
            if (execute < 0) {
                printf("\nCould not execute command..");
            }
        }
        wait(NULL);
        dup2(pd[0], 0);
        close(pd[1]);
    }
    token = strtok(parts[j], " ");
    char *command = token;
    int i=1;
    argu[0] = command;
    while(token != 0){
        token = strtok(0, " ");
        argu[i] = token;
        i++;
    }
    argu[i-1]=NULL;
    int execute = execvp(command, argu);
    if(execute < 0){cout << "err2";}
    perror("exec");
    abort();
    }else{
        wait(NULL);
        return;
    }
}
void pipesplit(string str){
    char arr[str.length()+1];int i=0;
    strcpy(arr, str.c_str());
    arr[str.length()] = '\0';
    char *argu[1000];
    char *token = strtok(arr, "|");
    //argu[0] = token;
    while (token != NULL)
    {   argu[i] = token;
        token = strtok(NULL, "|");
        i++;
    }

        forkandexecPipe(argu, i);

}
void initialProcessing(){
    string str;
    getline(cin, str);
    if(!ispiped(str)){
        splitcommands(str);
    }
    else{
        pipesplit(str);
    }
}
int main(){
    hello();
    welcomenote();
    while(true){
        char cwd[1000];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            strcat(cwd, "$");
            cout << cwd;
        }
        initialProcessing();
    }
    return 0;
}
