#ifndef USER_H
#define USER_H
#include "../all.h"
#include "../util/progressbar.h"


user* initUser(int id);

user* createUsersTable(movie* moviesTable);

// Function to serialize the user array into a binary file
void serializeUsers(user* users, int numUsers, const char* filename);

// Function to deserialize the user array from a binary file
user* deserializeUsers(const char* filename, int* numUsers);


void freeUsers(user* users, int numUsers);


#endif // USER_H