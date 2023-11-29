#ifndef USER_H
#define USER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include "movies.h"

#define NBUSERS 480189
#define IDMAXUSER 2649429

typedef struct rating_temp{
     int id_user;
     int id_film;
     int year;
     int day;
     int month;
     int star;
} rating;

typedef struct user_temp{
    int id;
    int nb_ratings;
    rating* ratings;
} user;

// Function to serialize the user array into a binary file
void serializeUsers(user* users, int numUsers, const char* filename);

// Function to deserialize the user array from a binary file
user* deserializeUsers(const char* filename, int* numUsers);

#endif // USER_H