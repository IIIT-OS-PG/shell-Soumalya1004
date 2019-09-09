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
#define MX 1024
using namespace std;
unordered_map<string, string> aliasmap;
int rootflag = 0;
int estatus = 0;
int customcommand(char *command, char *arguments[]){
    int l = 4;
    char* custom[l];
    int i = 0, choice=10;
    int c=0;
    custom[0] = "echo";
    custom[1] = "cd";
    custom[2] = "~";
    custom[3] = "export";
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
                    cout << getpid()<<endl;
                    c=1;
                    break;
                }
                if(*(arguments[1]+1) == '?'){
                    cout << estatus <<endl;
                    c=1;
                    break;
                }
                strcpy(env, arguments[1]+1);
                if(!getenv(env)){
                    cout << "Cant access variable"<<endl;
                }
                else{
                    cout << getenv(env)<<endl;
                }
            }
            c=1;
            break;
        case 1:
            chdir(arguments[1]);
            c = 1;
            break;
        case 2:
            cout << getenv("HOME") << endl;
            c=1;
            break;
        case 3:
            cout << arguments[0]<< endl;
            cout << arguments[1]<< endl;
            if(arguments[1]){
                string x(arguments[1]);
                separate(x, arguments, "=");
                if(!arguments[1]){cout << "Export command syntax is wrong"<<endl;c=1;break;}
                FILE *stream;char* k, *v;char carr[MX];
                stream = fopen(".myenvrc", "ab+");
//                while(fgets(carr,MX,stream)!=NULL){
//                    k = strtok(carr, "=");
//                    v = strtok(0, " ");
//                    if(strcmp(k, arguments[1]) == 0){
//                        setenv(arguments[0], arguments[1], 1);
//                        strcpy(arguments[1], x.c_str());
//                        fputs(arguments[1], stream);
//                        fclose(stream);c=1;break;
//                    }
//                }
                setenv(arguments[0], arguments[1], 1);
                strcpy(arguments[1], x.c_str());
                fputs(arguments[1], stream);
                fclose(stream);
            }
            c=1;
            break;
        default:
            break;
        }
        return c;
}
void forkandexec(char *command, char *arguments[]){  //Forks child and execute command
    pid_t p = fork();
    if (p == 0) {
        string c(command);
        if (aliasmap.find(c) != aliasmap.end()){
            string x(aliasmap.at(c));

            separate(x, arguments, " ");

            command = arguments[0];
        }
        int execute = execvp(command, arguments);

        if (execute < 0) {
            cout << "Could not execute command" << endl;
        }
        exit(0);
    }
    else{
        int stat;
        waitpid(p, &stat, 0);
        estatus = WEXITSTATUS(stat);
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
//    char* t;
    char *argu[MX];
    char arr[str.length()+1];
    strcpy(arr, str.c_str());
    arr[str.length()] = '\0';
//    char *command;
//    t = strtok(arr, " ");
//    command = t;
//    int i=1;
//    argu[0] = command;
//    while(t != 0){
//        t = strtok(0, " ");
//        argu[i] = t;
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
    char *argu[MX];
    char *t;
    pid_t x = fork();
    if(x==0){
    for( j=0; j<(l-1); j++){
        int arrp[2];
        pipe(arrp);
        pid_t p = fork();
        if (p == 0) {
            dup2(arrp[1], 1);
            t = strtok(parts[j], " ");
            char *command = t;
            char *argu[MX];int i=1;
//                    string c(command);
//                    if (aliasmap.find(c) != aliasmap.end()){
//                        string x(aliasmap.at(c));
//                        separate(x, argu, " ");
//                        command = argu[0];
//                    }
            argu[0] = command;
            while(t != 0){
                t = strtok(0, " ");
                argu[i] = t;
                i++;
            }
            argu[i-1]=NULL;

            int execute = execvp(argu[0], argu);
            if (execute < 0) {
                cout << "Could not execute command" <<endl;
            }
        }
        wait(NULL);

        dup2(arrp[0], 0);

        close(arrp[1]);
    }
    t = strtok(parts[j], " ");
    char *command = t;
    int i=1;
    argu[0] = command;
    while(t != 0){
        t = strtok(0, " ");
        argu[i] = t;
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
        int stat;
        waitpid(x, &stat, 0);
        estatus = WEXITSTATUS(stat);
//        wait(NULL);
        return;
    }
}
void pipesplit(string str){
    char arr[str.length()+1];int i=0;
    strcpy(arr, str.c_str());
    arr[str.length()] = '\0';
    char *argu[MX];
    char *t = strtok(arr, "|");
    //argu[0] = token;
    while (t != NULL)
    {   argu[i] = t;
        t = strtok(NULL, "|");
        i++;
    }
    forkandexecPipe(argu, i);
}
int checkAlias(string str){
    int v = isSubstring("alias ", str);char *token;
    if(v == -1){return 0;}
    char *argu[MX];int i=0;
    char arr[str.length()+1];arr[str.length()] = '\0';
    strcpy(arr, str.c_str());
    argu[0] = strtok(arr, "=");argu[1] = strtok(0, "=");//value
    token = strtok(argu[0], " ");argu[2] = strtok(0, " "); //key
    //cout << argu[1];
    if(argu[2] == "" || argu[1] == "")return 1;
    string k(argu[2]);string val(argu[1]);
    characterremove(val, "\"");
    characterremove(val, "\'");
//    if(val.length() == 0){
//        cout << "Wrong use of alias"<<endl;
//        return 1;
//    }
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
        characterremove(s, " ");
        strcpy(fname, s.c_str());
        stream = fopen(fname, "wb");
        out =  fileno(stream);
//        out = open(fname, O_WRONLY | O_CREAT,0666 );
    }
    else if(a == 2){
        separate(str, fileargu, ">>");
        fname = fileargu[1];
        string s(fname);
        characterremove(s, " ");
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
        int stat;
        waitpid(p, &stat, 0);
        estatus = WEXITSTATUS(stat);
//        wait(NULL);
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
        char cwd[MX];
        string prompt ="";
        string us(getenv("USER"));
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            if(rootflag == 1){
                setenv("PS1", "#", 1);
                strcat(cwd, getenv("PS1"));
                prompt = "root@" + us + cwd;
            }
            else{
                setenv("PS1", "$", 1);
                strcat(cwd, getenv("PS1"));
                prompt = "users@" + us + cwd;
            }
            cout << prompt;
        }
        initialProcessing();
    }
    return 0;
}
