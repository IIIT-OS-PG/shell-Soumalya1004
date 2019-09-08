#include <iostream>
#include <algorithm>
#include "filehandler.h"
#include <bits/stdc++.h>
#include<sys/wait.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#define MX 1000
using namespace std;
unordered_map<string, string> aliasmap;
int rootflag = 0;
int exit_status = 0;
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
                if(*(arguments[1]+1) == '$'){
                    cout << getpid();
                    c=1;
                    break;
                }
                if(*(arguments[1]+1) == '?'){
                    cout << exit_status;
                    c=1;
                    break;
                }
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
    string c(command);
    pid_t p = fork();
    if (p == 0) {
        if (aliasmap.find(c) != aliasmap.end()){
            string x(aliasmap.at(c));
            //cout << x<< endl;
            //char *argu[MX];
//            strcpy(command,x.c_str());
            separate(x, arguments, " ");
//            for(int z=0; arguments[z]; z++){
//                cout << arguments[z] << endl;
//            }
            //arguments = arg;
            command = arguments[0];
        }
        int execute = execvp(command, arguments);
        if (execute < 0) {
            printf("\nCould not execute command..");
        }
        exit(0);
    }
    else{
        int status;
        waitpid(p, &status, 0);
        exit_status = WEXITSTATUS(status);
        //wait(NULL);
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
    char *argu[MX];
    char arr[str.length()+1];
    strcpy(arr, str.c_str());
    arr[str.length()] = '\0';
    char *command;
//    token = strtok(arr, " ");
//    command = token;
//    int i=1;
//    argu[0] = command;
//    while(token != 0){
//        token = strtok(0, " ");
//        argu[i] = token;
//        i++;
//    }
    separate(str, argu, " ");
    if(strcmp(argu[0],"sudo") == 0){
        //separate(str, argu, " ");
        rootflag = 1;
    }else if(strcmp(argu[0],"exit") == 0){
        rootflag = 0;
        return;
    }
    if(!customcommand(argu[0], argu)){
          forkandexec(argu[0], argu);
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
                token = strtok(0, " ");
                argu[i] = token;
                i++;
            }
            argu[i-1]=NULL;
//            string str(parts[j]);
//            char *argu[1000];
//            splitcmd(str,argu);
            int execute = execvp(argu[0], argu);
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
//    string str(parts[j]);
//    char *argu[1000];
//    splitcmd(str,argu);
    int execute = execvp(argu[0], argu);
    if(execute < 0){cout << "err2";}
    perror("exec");
    }
    else{
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
int checkAlias(string str){
    int v = isSubstring("alias ", str);char *token;
    if(v == -1){return 0;}
    char *argu[1000];int i=0;
    char arr[str.length()+1];arr[str.length()] = '\0';
    strcpy(arr, str.c_str());
    argu[0] = strtok(arr, "=");argu[1] = strtok(0, "=");//value
    token = strtok(argu[0], " ");argu[2] = strtok(0, " "); //key
    //cout << argu[1];
    if(argu[2] == "" || argu[1] == "")return 1;
    string k(argu[2]);string val(argu[1]);
    removeCharsFromString(val, "\"");
    removeCharsFromString(val, "\'");
    aliasmap[k] = val;
    //aliasmap.insert({k, val});
    for (auto x : aliasmap){
        cout << x.first << " " << x.second << endl;
    }
    return 1;
}
void redirect(string str, int a){
    pid_t p = fork();
    if(p==0){
        int out;
    char* fname;
    FILE *stream;
    char *fileargu[100],*argu[MX];
    if(a == 1){
        separate(str, fileargu, ">");
        fname = fileargu[1];
        string s(fname);
        removeCharsFromString(s, " ");
        strcpy(fname, s.c_str());
        stream = fopen(fname, "wb");
        out =  fileno(stream);
//        out = open(fname, O_WRONLY | O_CREAT,0666 );
    }
    else if(a == 2){
        separate(str, fileargu, ">>");
        fname = fileargu[1];
        string s(fname);
        removeCharsFromString(s, " ");
        strcpy(fname, s.c_str());
        stream = fopen(fname, "ab+");
        out =  fileno(stream);
//        out = open(fname, O_WRONLY | O_APPEND | O_CREAT,0666 );
    }
    dup2(out,STDOUT_FILENO);
    separate(fileargu[0], argu, " ");
    execvp(argu[0], argu);
    close(out);
    }
    else{
        wait(NULL);
    }
}
void initialProcessing(){
    string str,str1;
    getline(cin, str);
    if(str.empty()){return;}
    int a = ioredirect(str);
    if(a != 0){
        redirect(str, a);
        return;
    }
    if(checkAlias(str) != 0){return;}
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
        string prompt ="";
        string us(getenv("USER"));
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            if(rootflag == 1){
                strcat(cwd, "#");
                prompt = "roots@" + us + cwd;
            }
            else{
                strcat(cwd, "$");
                prompt = "users@" + us + cwd;
            }
            cout << prompt;
        }
        initialProcessing();
    }
    return 0;
}
