#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<stdbool.h>

#include"Matrix.h"
#include"segment.h"

int main(int argc, char** argv){

   if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <line_txt> <words>\n", argv[0]);
        return EXIT_FAILURE;
    }
   if(segment(argv[1], argv[2]) != 0){
       return EXIT_FAILURE;
   }

return EXIT_SUCCESS;
}
