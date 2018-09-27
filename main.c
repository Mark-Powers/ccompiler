#include <stdlib.h>
#include "global.h"
#include "init.h"
#include "parser.h"

int main(int argc, char **argv)
{
    debug = 0;
    int i;
    for(i = 1; i < argc; i++){
        if(argv[i][0] == '-' && argv[i][1]=='d'){
            debug = 1;
        }
    }
    init();
    parse();
    exit(0);
}
