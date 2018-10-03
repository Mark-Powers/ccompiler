#include "io.h"
#include "../global.h"

#define ISIZE 40

char inputbuff[ISIZE];
int  i_index;
int  max_index; // Max index read into file
int  real_index; // Real i_index (no looping)

void fill(int front){
    int i;
    if(front){
        for(i = 0; i < ISIZE/2-1; i++) {
            inputbuff[i] = fgetc(file);
        }
    } else {
        for(i = 0; i < ISIZE/2-1; i++) {
            inputbuff[ISIZE/2 + i] = fgetc(file);
        }
    }
    max_index += ISIZE/2-1;
}

void setupbuff() {
    i_index = -1; 
    real_index = 0;
    max_index = 0;
    
    inputbuff[ISIZE/2-1] = EOF; 
    inputbuff[ISIZE-1] = EOF;
    fill(1);
}

char nextchar() {
    i_index++;
    if(inputbuff[i_index] == EOF){
        if(i_index == ISIZE/2-1) {
            if(real_index == max_index){
                fill(0); 
            }
            i_index++;
        } else if(i_index == ISIZE-1) { 
            if(real_index == max_index){
                fill(1); 
            }
            i_index = 0; 
        }
        // Else EOF, so return it anyway
    }

    real_index++;
    return inputbuff[i_index];
}

void ungetchar(char c) {
    inputbuff[i_index] = c;
    i_index--;
    real_index--;
    if(i_index == ISIZE/2-1){
        i_index-=1;
    } else if(i_index == -1) { 
        i_index = ISIZE-2; 
    }
    // We shouldn't need to ungetc, because hopefully
    // our buffer is larger than the max lookahead needed
//    ungetc(c, file);
}

