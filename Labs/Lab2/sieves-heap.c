#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

#define COLUMNS 6

void print_number(int n){
  printf("%10d", n);
}

void print_sieves(int n){
    bool* sieves = (bool*) malloc(sizeof(bool) * (n-2));

    for (int i = 0; i < n ; i++){
        *(sieves + i) = true; 
    }

    for (int i = 0; i <= sqrt(n); i++){
        if (*(sieves + i) == true){  
            for(int j = (i+2)*(i+2); j <= n; j += (i+2)){
                *(sieves + (j-2)) = false;
            }

        }
    }

    int* columnCounter = (int*) malloc(sizeof(int));
    *columnCounter = 0;
    for (int i = 0; i <= n-2; i++){
        if (*(sieves + i) == true){
            printf("%10d", i+2);
            *columnCounter = *columnCounter + 1;
            if (*columnCounter == COLUMNS) {
                printf("\n");
                *columnCounter = 0;
            }
        }
    }

    free(columnCounter);
    free(sieves);
}

// 'argc' contains the number of program arguments, and
// 'argv' is an array of char pointers, where each
// char pointer points to a null-terminated string.
int main(int argc, char *argv[]){
  if(argc == 2)
    print_sieves(atoi(argv[1]));
  else
    printf("Please state an interger number.\n");
  return 0;
}

