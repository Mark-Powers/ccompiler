#include <stdlib.h>
#include "global.h"
#include "init.h"
#include "parser.h"

int main(int argc, char **argv)
{
    if(argc > 1){
        debug = 1;
    } else {
        debug = 0;
    }

    init();
    parse();
    exit(0);
}
