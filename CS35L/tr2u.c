//Written by Rosemary He
//this program takes a file and translates certain characters according to a dictionary the user provides
//this version uses unbuffered IO

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char* argv[]){
    //check number of input
    if(argc != 3){
        fprintf(stderr,"Wrong number of operands");
        exit(1);
    }

    char* from = argv[1];
    char* to = argv[2];
    //check if from and to match in length
    if(strlen(from) != strlen(to)){
        fprintf(stderr,"Mismatch in length between from and to");
        exit(1);
    }
    //check for duplicates in from string
    int len = strlen(from);
    int i;
    int j;
    for(i = 0; i < len; i++){
        for(j = i+1; j < len; j++){
            if(from[i] == from[j]){
                fprintf(stderr, "Duplicates in from");
                exit(1);
            }
        }
    }
    //get input and check error
    char in;
    while(read(STDIN_FILENO, &in, 1) > 0){
        int match = 0;
        //check if match in from set, if match then transliterate
        for(i = 0; i < len; i++){
            if(from[i] == in){
                write(STDOUT_FILENO, &to[i], 1);
                match = 1;
            }
        }
        //no match, then output original char
        if(match == 0){
            write(STDOUT_FILENO, &in, 1);
        }
    }
    return 0;
}
