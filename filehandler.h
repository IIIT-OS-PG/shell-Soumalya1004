#ifndef FILEHANDLER_H_INCLUDED
#define FILEHANDLER_H_INCLUDED
#include <iostream>
#include <algorithm>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <bits/stdc++.h>
#include<sys/wait.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#define MX 1000
using namespace std;
void hello(){
    char c[1000];
    char ch;
    char* k, *v;
    FILE *source = fopen("/media/soumalya/New Volume/Ebooks_Mtech/sem1/Operating System/Assignment1/Assignment1/Assignment1/.myenvrc","r");
    if(source == NULL){
        printf("File can\'t be accessed");
    }
    while ( fgets(c,1000,source) != NULL){
        k = strtok(c, "-");
        v = strtok(0, " ");
        setenv(k, v, 1);
    }
}
void separate(string str, char *argu[], char *delimiter){
    char arr[MX];int i=0;
    strcpy(arr, str.c_str());
    char *token = strtok(arr, delimiter);
    argu[i] = token;
    while (token != NULL){
        token = strtok(NULL, delimiter);
        argu[++i] = token;
    }
    //return argu;
}
void splitcmd(string str,char *argu[MX]){
    char* token;
    char arr[str.length()+1];
    strcpy(arr, str.c_str());
    //arr[str.length()] = '\0';
    token = strtok(arr, " ");
    cout << token;
    char *command = token;
    int i=1;
    argu[0] = command;
    while(token != 0){
        token = strtok(0, " ");
        cout << argu[i];
        argu[i] = token;
        i++;
    }

    //return argu;
}
void customsplit(string str,char *argu[1000], char *s){
    char* token;
    char arr[str.length()+1];
    strcpy(arr, str.c_str());
    //arr[str.length()] = '\0';
    token = strtok(arr, s);
    char *command = token;
    int i=1;
    argu[0] = command;
    while(token != 0){
        token = strtok(0, s);
        argu[i] = token;
        i++;
    }
}
int isSubstring(string s1, string s2){
    int b = s2.length();int a = s1.length();
    int i,j;
    for (i = 0; i <= b - a; i++){
        for (j = 0; j < a; j++){
            if (s2[i + j] != s1[j]){
                break;
            }
        }
        if (j == a){return i;}
    }
    return -1;
}
void removeCharsFromString( string &str, char* charsToRemove ) {
    int i;
   for(i = 0; i<strlen(charsToRemove); ++i){
      str.erase(remove(str.begin(), str.end(), charsToRemove[i]), str.end());
   }
}
int ioredirect(string str){
    if(isSubstring(">", str) == -1){
        return 0;
    }
    else if(isSubstring(">>", str) != -1){
        return 2;
    }
    else if(isSubstring(">", str) != -1){
        return 1;
    }
}
#endif // FILEHANDLER_H_INCLUDED
