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
//void customcommand(){
//
//}
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
void initialProcessing(){
    string str;
    char* token;
    getline(cin, str);
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
    //customcommand();
    forkandexec(command, argu);
}
int main(){
    hello();
    welcomenote();
    //cout << getenv("PATH");
    while(true){
        string location = getenv("PATH");
        cout << location << "$";
        initialProcessing();

    }

    return 0;
}
