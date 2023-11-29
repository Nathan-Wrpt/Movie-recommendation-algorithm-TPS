#include <stdio.h>
#include <stdlib.h>
#include "user.h"
#include "movies.h"

user* initUser(int id) {
    user* u = (user*) malloc(sizeof(user));
    u->id = id;
    u->nb_ratings = 0;
    u->ratings = NULL;
    return u;
}

user* createUsersTable(char* folderpath, movie* moviesTable) {
    user usersTable[NBUSERS];
    int nbUserSeen = 0;

    // struct to know in o(1) if an user is already seen
    // and know its place in the list of users
    // problem : IDMAXUSER - NBUSERS will stay not seen
    // but the struct will be destroyed when he userTable
    // will be created
    int* seenUserTable = (int*) malloc(IDMAXUSER * sizeof(int)); //size 4 * 2 600 000 = 10.4 Mo oof
    if (seenUserTable == NULL) return NULL;
    for (int id = 0; id < IDMAXUSER - 1; id++) seenUserTable[id] = -1;

    // for each film, check the id of reviewers and group the review
    // per reviewers
    for (int movie_id = 0; movie_id < NBMOVIES; movie_id++) {
        int nb_ratings = moviesTable[movie_id].nb_ratings;

        for (int num_rating; num_rating < nb_ratings; num_rating)
        {
            int id_user = moviesTable[movie_id].ratings[num_rating].id_user;

            // Check if user has been seen before to
            // attribute / find its place in the usersTable

            int placeInTable;
            if (seenUserTable[id_user] == -1) {
                placeInTable = ++nbUserSeen;
                seenUserTable[id_user] = placeInTable;
                usersTable[placeInTable] = *initUser(id_user);
            } else {
                placeInTable = seenUserTable[id_user];
            }
            
            int newNbRatings = usersTable[placeInTable].nb_ratings + 1;
            usersTable[placeInTable].nb_ratings = newNbRatings;

            realloc(usersTable[placeInTable].ratings, newNbRatings * sizeof(rating));
            usersTable[placeInTable].ratings[newNbRatings - 1] = moviesTable[movie_id].ratings[num_rating];
        }
    }

    free(seenUserTable);
    return usersTable;
}

// Function to serialize the user array into a binary file
void serializeUsers(user* users, int numUsers, const char* filename) {
    FILE* file = fopen(filename, "wb"); // Open the file in write mode (binary)
    if (file != NULL) {
        fwrite(&numUsers, sizeof(int), 1, file); // Write number of users first
        fwrite(users, sizeof(user), numUsers, file); // Write users data
        fclose(file); // Close the file
    } else {
        printf("Failed to open the file for writing.\n");
    }
}

// Function to deserialize the user array from a binary file
user* deserializeUsers(const char* filename, int* numUsers) {
    FILE* file = fopen(filename, "rb"); // Open the file in read mode (binary)
    user* users = NULL;

    if (file != NULL) {
        fread(numUsers, sizeof(int), 1, file); // Read number of users first
        users = (user*)malloc(sizeof(user) * (*numUsers)); // Allocate memory for users
        fread(users, sizeof(user), *numUsers, file); // Read users data
        fclose(file); // Close the file
    } else {
        printf("Failed to open the file for reading.\n");
    }

    return users;
}

int main() {
    return 0;
}