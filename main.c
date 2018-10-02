#include <stdlib.h>
#include "global.h"
#include "init.h"
#include "parser.h"

int main(int argc, char **argv)
{
    file = 0;
    debug = 0;
    char *s;
    while(--argc > 0){
        if( (*++argv)[0] == '-' ){
            for(s = argv[0]+1; *s != '\0'; s++){
                if(*s == 'd'){
                    debug = 1;
                }
            }
        } else {
            printf("Compiling '%s'\n", argv[0]);
            file = fopen(argv[0], "r");
        }
    }
    if(!file){

        file = stdin;
    }
    init();
    parse();
    exit(0);
}
