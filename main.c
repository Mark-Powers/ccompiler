#include <stdlib.h>
#include "global.h"
#include "init.h"
#include "parser.h"

int main(int argc, char **argv)
{
    debug = 0;
    char *s;
    while(--argc > 0 && (*++argv)[0] == '-'){
        for(s = argv[0]+1; *s != '\0'; s++){
            if(*s == 'd'){
                debug = 1;
            }
        }
    }
    init();
    parse();
    exit(0);
}
