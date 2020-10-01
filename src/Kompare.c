#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <table.c>



/* void CondenseRanges(ranges):

     for i in range(len(ranges)-1):
         try:
             if ranges[i][1] >= ranges[i+1][0]:
                 ranges[i] = (ranges[i][0], max(ranges[i+1][1], ranges[i][1]))
                 del ranges[i+1]
                 CondenseRanges(ranges)
         except:
            continue
*/             

float max(float a, float b){
    if (a > b){
        return a;
    }
    return b;
}

float condenseRanges(float input[], int size){
    int i;
    float result[];
    
    for (i=; i<size; i++){
        if (input[i][1] >= input[i+1][0]){
            float val = max(input[i+1][1], input[i][1]);
            result[-1] = ({input[i][0], val})
        }
    }
    for 

}

int main()
{   
    float ranges[4][2] = {{0.1, 5.0},{0.3, 10.0},{12.0, 15.0},{14.0, 20.0}};

    int i, j;
    for(i = 0; i<4; i++){
        for (j=0; j<2; j++){
            printf("ranges[%d][%d] = %f\n", i,j, ranges[i][j] );
        }
    }
    return 0;
}

