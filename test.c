#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include"Matrix.h"
#include"IOHandler.h"
#include"shifts.h"


int main(void){

    shifts(5, "ciphered.txt", "bigrams.csv");

    return 0;
}