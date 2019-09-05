#ifndef FILEHANDLER_H_INCLUDED
#define FILEHANDLER_H_INCLUDED
#include <iostream>
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
char **splitcmd(char *arr){
    char *argu[1000];int i=0;
    char *token = strtok(arr, " ");
    while (token != NULL)
    {
        argu[i] = token;
        cout << argu[i] << endl;
        token = strtok(NULL, " ");
        i++;
    }
    return argu;
}
void piped(char ** argv, int argc) {
    int i;
    for( i=0; i<argc-1; i++)
    {
        int pd[2];
        pipe(pd);
        if (fork()) {
            dup2(pd[1], 1); // remap output back to parent
            execlp(argv[i], argv[i], NULL);
            perror("exec");
            abort();
        }
        // remap output from previous child to input
        dup2(pd[0], 0);
        close(pd[1]);
    }

    execlp(argv[i], argv[i], NULL);
    perror("exec");
    abort();
}

#endif // FILEHANDLER_H_INCLUDED
