#ifndef USER_H
#define USER_H
#include "../all.h"
#include "../util/progressbar.h"

//Creates an empty user structure given its id
user* initUser(int id);

//Given the movies table, creates the whole table of users with all their ratings
user* createUsersTable(movie* moviesTable);

// Function to serialize the user array into a binary file
void serializeUsers(user* users, int numUsers, const char* filename);

// Function to deserialize the user array from a binary file
user* deserializeUsers(const char* filename, int* numUsers);

//Frees the Usertable
void freeUsers(user* users, int numUsers);


#endif // USER_H