#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#include "userFile.h"

/*const char* USER_FILE_NAME = "userFile.txt";
const char* USER_FILE_MODE = "a+";*/
//Serialization
const char* USER_FORMAT_IN = "(%[^,], %[^,], %d)\n";
const char* USER_FORMAT_OUT = "(%s, %s, %d)\n";
#define MAX_LINE_LENGTH 200

User readUser(char *buffer) { // Safer than fscanf directly
    User u;
    sscanf(buffer, USER_FORMAT_IN, u.userId, u.username, &u.c);
    return u;
}

bool addUser(FILE *f, User u) {
    if (f == NULL) return false;
    fprintf(f, USER_FORMAT_OUT, u.userId, u.username, u.c);
    return true;
}

bool userExistsByIdAndCValue(FILE *f, User user) {
    if (f == NULL) return false;
    rewind(f);
    char buffer[MAX_LINE_LENGTH];
    fgets(buffer, MAX_LINE_LENGTH, f);
    while (!feof(f)) {
        User u = readUser(buffer);
        if (!strcmp(u.userId, user.userId) && u.c == user.c) return true;
        fgets(buffer, MAX_LINE_LENGTH, f);
    }
    return false;
}

bool userExistsById(FILE *f, User user) {
    if (f == NULL) return false;
    rewind(f);
    char buffer[MAX_LINE_LENGTH];
    fgets(buffer, MAX_LINE_LENGTH, f);
    while (!feof(f)) {
        User u = readUser(buffer);
        if (!strcmp(u.userId, user.userId)) return true;
        fgets(buffer, MAX_LINE_LENGTH, f);
    }
    return false;
}

bool getUsernameById(FILE *f, User *user) {
    if (f == NULL) return false;
    rewind(f);
    char buffer[MAX_LINE_LENGTH];
    fgets(buffer, MAX_LINE_LENGTH, f);
    while (!feof(f)) {
        User u = readUser(buffer);
        if (!strcmp(u.userId, user->userId)) {
            strcpy(user->username, u.username);
            return true;
        }
        fgets(buffer, MAX_LINE_LENGTH, f);
    }
    return false;
}

bool getUserById(FILE *f, User *user) {
    if (f == NULL) return false;
    fseek(f, 0, SEEK_SET);
    char buffer[MAX_LINE_LENGTH];
    fgets(buffer, MAX_LINE_LENGTH, f);
    while (!feof(f)) {
        User u = readUser(buffer);
        if (!strcmp(u.userId, user->userId)) {
            strcpy(user->username, u.username);
            user->c = u.c;
            return true;
        }
        fgets(buffer, MAX_LINE_LENGTH, f);
    }
    return false;
}

bool updateUsernameById(FILE **f, char *fileName, User user) {
    if (*f == NULL) return false;
    FILE *tmpFile = fopen("__tmpUserFile__.txt", "w");
    if (tmpFile == NULL) return false;
    rewind(*f);

    char buffer[MAX_LINE_LENGTH];
    fgets(buffer, MAX_LINE_LENGTH, *f);
    while (!feof(*f)) {
        User u = readUser(buffer);
        if (!strcmp(u.userId, user.userId)) {
            strcpy(u.username, user.username);
        }
        if (!addUser(tmpFile, u)) return false;
        fgets(buffer, MAX_LINE_LENGTH, *f);
    }

    fclose(*f);
    fclose(tmpFile);
    remove(fileName);
	rename("__tmpUserFile__.txt", fileName);
	*f = fopen(fileName, "a+");
    if (f == NULL) return false;
    return true;
}

bool isEmptyLine(char *buffer){
	return (buffer[0] == ' ');
}

bool readFirstUser(FILE *f, User *user){
	if (f == NULL) return false;
    rewind(f);
    char buffer[MAX_LINE_LENGTH];
    fgets(buffer, MAX_LINE_LENGTH, f);
    if(isEmptyLine(buffer)) return false;
    *user = readUser(buffer);
    return true;
}
