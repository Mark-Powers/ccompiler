#include <stdlib.h>
#include "global.h"
#include "lexer.h"

void error(char* m)
{
    fprintf(stderr, "line %d: %s\n", lineno, m);
    exit(1);
}

void syntaxError(char* message, char* expected, char* actual)
{
    fprintf(stderr, "line %d (syntax error) %s\n\texpected: %s\n\tactual %s\n", lineno, message, expected, actual);
    skipstatement();
}
