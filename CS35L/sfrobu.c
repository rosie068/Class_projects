//Written by Rosemary He
//this program takes in an input file of obfuscated data and sorts the file
//this program uses unbuffered IO

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <ctype.h>
#include <string.h>

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

//Case insensitive compare a and b: 1 if a > b; 0 if a == b; -1 if a < b.
int frobcmpU(char const *a, char const *b){
    if(a == NULL || b == NULL){
        fprintf(stderr, "Memory allocation error");
        exit(1);
    }
    char tmpa = unfrobnicate(*a);
    char tmpb = unfrobnicate(*b);
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
        else if(toupper(tmpa) < toupper(tmpb)){
            return -1;
        }
        else if(toupper(tmpa) > toupper(tmpb)){
            return 1;
        }
    }
}

int compareU(const void* a, const void* b){
    return frobcmpU(*(const char**) a, *(const char**) b);
}

//print all words
void wprint(char** word_arr, int word_count){
    int i,j;
    char space = ' ';
    ssize_t stat;
    for(i = 0; i < word_count; i++){
        for(j = 0; word_arr[i][j] != ' '; j++){
            stat = write(STDOUT_FILENO, &word_arr[i][j],1);
            if(stat < 0){
                fprintf(stderr, "read error");
                free(word_arr);
                exit(1);
            }
        }
        stat = write(STDOUT_FILENO, &space,1);
        if(stat < 0){
            fprintf(stderr, "read error");
            free(word_arr);
            exit(1);
        }
    }
}

int main(int argc, char* argv[]){
    int case_ignore = 0;
    //check for f flag
    if(argc == 2){
        if(strcmp(argv[1], "-f") == 0){
            case_ignore = 1;
        }
        else{
            fprintf(stderr, "Only -f can be provided");
            exit(1);
        }
        
    }
    else if(argc > 2){
        fprintf(stderr, "Only one argument can be provided");
        exit(1);
    }

    struct stat fileData;
    //STDIN_FILENO = 0
    if (fstat(STDIN_FILENO, &fileData) < 0){
        fprintf(stderr, "fstate error");
        exit(1);
    }

    //check if STDIN is regular file； if not allocate 8KiB buffer
    int buff_size = 0;
    if(S_ISREG(fileData.st_mode)){
        buff_size = fileData.st_size;
    }
    else{
        buff_size = 8192;
    }

    char* word;
    char** word_arr;
    word = (char*)malloc(buff_size * sizeof(char));
    //check for malloc error
    if(word == NULL){
        fprintf(stderr, "Malloc error");
        free(word);
        exit(1);
    }
    word_arr = (char**)malloc(sizeof(char*));
    //check malloc error
    if(word_arr == NULL){
        fprintf(stderr, "Malloc error");
        free(word);
        free(word_arr);
        exit(1);
    }
    int char_count = 0;
    int word_count = 0;

    char in;
    ssize_t read_stat;
    read_stat = read(STDIN_FILENO, &in, sizeof(char));
    if(read_stat < 0){
        fprintf(stderr, "read error");
        free(word);
        free(word_arr);
        exit(1);
    }

    //get input
    while(read_stat > 0){
        //if buffer more than half full, resize to 2x
        if(buff_size <= char_count * 2){
            buff_size = buff_size * 2;
            word = (char*)realloc(word, buff_size * sizeof(char));
            //check for realloc error
            if(word == NULL){
                fprintf(stderr, "Realloc error");
                free(word);
                free(word_arr);
                exit(1);
            }
        }
        if(in == ' ' && char_count == 0){
            continue;
        }
        else if(in == ' ' && char_count > 0 && word[char_count-1] == ' '){
            continue;
        }
        word[char_count] = in;
        char_count++;
        //check for read error
        read_stat = read(STDIN_FILENO, &in, sizeof(char));
        if(read_stat < 0){
            fprintf(stderr, "read error");
            free(word);
            free(word_arr);
            exit(1);
        }
    }

    if(word[char_count-1] != ' '){
        buff_size = buff_size * 2;
        word = (char*)realloc(word, buff_size * sizeof(char));
        //check for realloc error
        if(word == NULL){
            fprintf(stderr, "Realloc error");
            free(word);
            free(word_arr);
            exit(1);
        }
        word[char_count] = ' ';
        char_count++;
    }

    //parse all words in word to word_arr
    int k;
    int tmp = 0;
    for(k = 0; k < char_count; k++){
        if(word[k] == ' '){
            word_arr = (char**)realloc(word_arr, (word_count+1) * sizeof(char*));
            //check for realloc error
            if(word_arr == NULL){
                fprintf(stderr, "Realloc error");
                free(word);
                free(word_arr);
                exit(1);
            }
            word_arr[word_count] = word + tmp;
            word_count++;
            tmp = k+1;
        }
    }

    //sort the words and output
    if (case_ignore==1){
        qsort(word_arr, word_count, sizeof(char*), compareU);
    }
    else{
        qsort(word_arr, word_count, sizeof(char*), compare);
    }
    wprint(word_arr, word_count);

    //free
    free(word);
    int i;
    for(i = 0; i < word_count; i++){
        free(word_arr[i]);
    }
    free(word_arr);
    return 0;
}
