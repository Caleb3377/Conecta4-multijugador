#ifndef USER_FILE_H
#define USER_FILE_H
#include <stdio.h>
#include <stdbool.h>
#define MAX_LENGTH_NAME 50
#define MAX_LENGTH_ID 20

typedef struct user {
	char userId[MAX_LENGTH_ID];
    char username[MAX_LENGTH_NAME];
    int c;
} User;

bool addUser(FILE *f, User u);

bool userExistsByIdAndCValue(FILE *f, User user);

bool userExistsById(FILE *f, User user);

bool getUsernameById(FILE *f, User *user);

bool getUserById(FILE *f, User *user);

bool updateUsernameById(FILE **f, char *fileName, User user);

bool readFirstUser(FILE *f, User *user);

#endif
