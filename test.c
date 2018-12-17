#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include"Matrix.h"
#include"IOHandler.h"
#include"shifts.h"


int main(void){

    shifts(3, "test.txt", "bigrams.csv");

    return 0;
}