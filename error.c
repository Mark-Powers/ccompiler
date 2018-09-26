#include <stdlib.h>
#include "global.h"

void error(char* m)
{
    fprintf(stderr, "line %d: %s\n", lineno, m);
    exit(1);
}

void syntaxError(char* message, char* expected, char* actual)
{
    fprintf(stderr, "line %d (syntax error) %s\nexpected: %s\nactual %s\n%d: '%c'\n", lineno, message, expected, actual, tokenval, tokenval);
    exit(1);
}
