#include <stdlib.h>
#include <getopt.h>

#include "global.h"
#include "init.h"
#include "parser/parser.h"
#include "util/NFA.h"
#include "util/regex.h"

int main(int argc, char **argv)
{
    file = stdin;
    out = stdout;
    debug = 0;

    static struct option long_options[] = 
    {
        {"debug", no_argument, NULL, 'd'},
        {"out", required_argument, NULL, 'o'},
//        {"o", required_argument, NULL, 'o'},
        {0, 0, 0, 0}
    };
    int c;
    int option_index = 0;
    while((c = getopt_long(argc, argv, "do:", long_options, &option_index)) != -1) {
        switch(c) 
        {
            case 'd':
                debug = 1;
                break;
            case 'o':
                fprintf(stderr, "Outputting '%s'\n", optarg);
                out = fopen(optarg, "w");
                break;
        }
    }
    while(optind < argc){
        fprintf(stderr, "Compiling '%s'\n", argv[optind]);
        file = fopen(argv[optind++], "r");
        break;
    }
    if(file == NULL){
        fprintf(stderr, "Cannot read from input file!\n");
        exit(1);
    }
    if(out == NULL){
        fprintf(stderr, "Cannot write to output file!\n");
        exit(1);
    }

    printf("compiling 'abc'\n");
    struct NFA* n = compileRE("abc");
    printf("printing NFA\n");
    printNFA(n);
    printf("toDFA\n");
    toDFA(n);

    init();
    parse();
    exit(0);
}
