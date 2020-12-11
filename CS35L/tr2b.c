//Written by Rosemary He
//this program takes a file and translates certain characters according to a dictionary the user provides
//this version uses buffered IO

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    for(i = 0; i < len; i++){
      int j;
      for(j = i+1; j < len; j++){
	if(from[i] == from[j]){
	  fprintf(stderr, "Duplicates in from");
          exit(1);
         }
      }
    }
    //get input and check error
    char in = getchar();
    if(ferror(stdin)){
        fprintf(stderr, "Stdin error");
        exit(1);
    }
    //keep reading
    while(!feof(stdin)){
        int match = 0;
        //check if match in from set, if match then transliterate
        for(i = 0; i < len; i++){
            if(from[i] == in){
                putchar(to[i]);
                if(ferror(stdin)){
                    fprintf(stderr, "Stdout error");
                    exit(1);
                }
                match = 1;
            }
        }
        //no match, then output original char
        if(match == 0){
            putchar(in);
            if(ferror(stdin)){
                fprintf(stderr, "Stdout error");
                exit(1);
            }
        }
        //get next char
        in = getchar();
        if(ferror(stdin)){
            fprintf(stderr, "Stdin error");
            exit(1);
        }
    }
    return 0;
}
