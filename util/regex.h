#ifndef REGEX_H
#define REGEX_H

#define ANY_ALPHA -10
#define ANY_ALPHANUM -11
#define ANY_NUM   -12

struct NFA* compileRE(char*);
int next(char*);

#endif // REGEX_H
