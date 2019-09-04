#ifndef FILEHANDLER_H_INCLUDED
#define FILEHANDLER_H_INCLUDED
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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
#endif // FILEHANDLER_H_INCLUDED
