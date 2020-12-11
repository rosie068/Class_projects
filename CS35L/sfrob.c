//Written by Rosemary He
//this program takes in an input file of obfuscated data and sorts the file
//this program uses buffered IO

#include <stdio.h>
#include <stdlib.h>

//return unforbnicated version
char unfrobnicate(const char a){
    return a^42;
}

//compare a and b: 1 if a > b; 0 if a == b; -1 if a < b.
int frobcmp(char const *a, char const *b){
    if(a == NULL || b == NULL){
        fprintf(stderr, "Memory allocation error");
        exit(1);
    }
    for(;; a++, b++){
        if(*a == ' ' && *b == ' '){
            return 0;
        }
        else if(*a == ' '){
            return -1;
        }
        else if(*b == ' '){
            return 1;
        }
        else if(unfrobnicate(*a) < unfrobnicate(*b)){
            return -1;
        }
        else if(unfrobnicate(*a) > unfrobnicate(*b)){
            return 1;
        }
    }
}

int compare(const void* a, const void* b){
    return frobcmp(*(const char**) a, *(const char**) b);
}

//print all words
void wprint(char** word_arr, int word_count){
    for(int i = 0; i < word_count; i++){
        int j = 0;
        while(word_arr[i][j] != ' '){
            putchar(word_arr[i][j]);
            //check error in output
            if(ferror(stdin)){
                fprintf(stderr, "Stdin error");
                exit(1);
            }
            j++;
        }
        putchar(' ');
        //check error in output
        if(ferror(stdin)){
            fprintf(stderr, "Stdin error");
            exit(1);
        }
    }
}

int main(){
    char* word;
    char** word_arr;
    word = (char*)malloc(sizeof(char));
    //check for malloc error
    if(word == NULL){
        fprintf(stderr, "Malloc error");
        free(word);
        exit(1);
    }

    word_arr = (char**)malloc(sizeof(char*));
    if(word_arr == NULL){
        fprintf(stderr, "Malloc error");
        free(word);
        free(word_arr);
        exit(1);
    }
    int char_count = 0;
    int word_count = 0;
    char in;

    //get input
    while(!feof(stdin)){
        in = getchar();
        //check error from input
        if(ferror(stdin)){
            fprintf(stderr, "Stdin error");
            exit(1);
        }
        //extend word by 1
        word = (char*)realloc(word, (char_count+1) * sizeof(char));
        //check for realloc error
        if(word == NULL){
            fprintf(stderr, "Realloc error");
            free(word);
            exit(1);
        }

        //check if we have reached end of word or file. if so  add to array and reset word
        if(char_count > 0 && (in == EOF || in == ' ')){
            word[char_count] = ' ';

            word_arr = (char**)realloc(word_arr, (word_count+1) * sizeof(char*));
            //check for realloc error
            if(word_arr == NULL){
                fprintf(stderr, "Realloc error");
                free(word_arr);
                exit(1);
            }
            word_arr[word_count] = word;
            word_count++;
            
            word = (char*)malloc(sizeof(char));
            //check malloc error
            if(word == NULL){
                fprintf(stderr, "Malloc error");
                free(word);
                exit(1);
            }
            char_count=0;
        }
        //else add char to end of word if word not NULL
        else if(word != NULL){
            word[char_count] = in;
            char_count++;
        }
        //else memory leak error
        else{
            fprintf(stderr, "Memory alloc error");
            free(word_arr);
            exit(1);
        }
    }

    //sort the words and output
    qsort(word_arr, word_count, sizeof(char*), compare);
    wprint(word_arr, word_count);

    //free
    free(word);
    for(int i=0; i < word_count; i++){
        free(word_arr[i]);
    }
    free(word_arr);
    return 0;
}