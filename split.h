#ifndef SPLIT_H
#define SPLIT_H
char** split(const char* str, const char *delim, int *splitLength);
void freeSplitStr(char **splitStr);
#endif