#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"shifts.h"


int main(int argc, char** argv)
{
    if (argc != 4)
    {
        fprintf(stderr, "Usage: %s <max_shift> <ciphered_text> <bigrams>\n", argv[0]);
        return EXIT_FAILURE;
    }
    int max_shift = atoi(argv[1]);
    if(max_shift < 0){
        fprintf(stderr, "The maximum number of shifts should be a positive integer\n");
        return EXIT_FAILURE;
    }
    if(shifts((unsigned int) max_shift, argv[2], argv[3]) != 0){
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}