#include <stdlib.h>
#include <string.h>

#include "split.h"

char** split(const char* str, const char *delim, int *splitLength) {
    char *strCopy = (char *) malloc( (strlen(str)+1) * sizeof(char)); // +1 for delimiter NULL
    strcpy(strCopy, str);
    int len = 0; 
    for (char *s = strtok(strCopy, delim); s != NULL; s = strtok(NULL, delim)) {
        if (strlen(s) > 0) len++;
    }
    *splitLength = len;
    char **splitStr = (char **) malloc( (len + 1) * sizeof(char *)); // last element = NULL
    splitStr[len] = NULL;

    strcpy(strCopy, str);
    int i = 0;
    for (char *s = strtok(strCopy, delim); s != NULL; s = strtok(NULL, delim)) {
        int curLen = strlen(s);
        if (curLen > 0) {
            splitStr[i] = (char *) malloc((curLen+1) * sizeof(char));
            strcpy(splitStr[i], s);
            i++;
        }
    }
    
    return splitStr;
}

void freeSplitStr(char **splitStr){
    for (int i = 0; splitStr[i] != NULL; i++) {
        free(splitStr[i]);
    }
    free(splitStr);
}
