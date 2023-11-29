#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NBFILMS 17770
#define NBUSERS 480189

typedef struct rating_temp{
     int id_film;
     int year;
     int day;
     int month;
     int rating;
} rating;

typedef struct user_temp{
    int id;
    int nb_ratings;
    rating* ratings;
} user;

// Function to create a table of all the users having the path of the training_set folder
user* createUsersTable(char* folderpath);

// Function to free the table of users
void freeUsersTable(user* users);

// Function to serialize the user array into a binary file
void serializeUsers(user* users, int numUsers, const char* filename);

// Function to deserialize the user array from a binary file
user* deserializeUsers(const char* filename, int* numUsers);

