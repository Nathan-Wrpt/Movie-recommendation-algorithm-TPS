#include "user.h"
#include "../all.h"

user* initUser(int id) {
    user* u = (user*) malloc(sizeof(user));
    u->id = id;
    u->nb_ratings = 0;
    u->ratings = NULL;
    return u;
}

user* createUsersTable(movie* moviesTable) {
    user* usersTable = (user*) malloc(NBUSERS * sizeof(user));
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
        // printf("Movie %d : %s\n", movie_id, moviesTable[movie_id].title);

        int nb_ratings = moviesTable[movie_id].nb_ratings;

        for (int num_rating = 0; num_rating < nb_ratings - 1; num_rating++)
        {
            int id_user = moviesTable[movie_id].ratings[num_rating].id_user;

            // Check if user has been seen before to
            // attribute / find its place in the usersTable

            int placeInTable;
            if (seenUserTable[id_user] == -1) {
                placeInTable = nbUserSeen;
                nbUserSeen++;
                seenUserTable[id_user] = placeInTable;
                usersTable[placeInTable] = *initUser(id_user);
            } else {
                placeInTable = seenUserTable[id_user];
            }
            
            int newNbRatings = usersTable[placeInTable].nb_ratings + 1;
            usersTable[placeInTable].nb_ratings = newNbRatings;

            usersTable[placeInTable].ratings = realloc(usersTable[placeInTable].ratings, newNbRatings * sizeof(rating));
            usersTable[placeInTable].ratings[newNbRatings - 1] = moviesTable[movie_id].ratings[num_rating];
        }
    }

    free(seenUserTable);
    return usersTable;
}

// Function to serialize the user array into a binary file
void serializeUsers(user* users, int numUsers, const char* filename) {
    FILE* file = fopen(filename, "wb");
    if (file != NULL) {
        fwrite(&numUsers, sizeof(int), 1, file); // Write number of users first

        for (int i = 0; i < numUsers; ++i) {
            fwrite(&(users[i].id), sizeof(int), 1, file);
            fwrite(&(users[i].nb_ratings), sizeof(int), 1, file);

            // Serialize each rating within the user
            for (int j = 0; j < users[i].nb_ratings; ++j) {
                fwrite(&(users[i].ratings[j].id_user), sizeof(int), 1, file);
                fwrite(&(users[i].ratings[j].id_film), sizeof(int), 1, file);
                fwrite(&(users[i].ratings[j].year), sizeof(int), 1, file);
                fwrite(&(users[i].ratings[j].day), sizeof(int), 1, file);
                fwrite(&(users[i].ratings[j].month), sizeof(int), 1, file);
                fwrite(&(users[i].ratings[j].star), sizeof(int), 1, file);
            }
        }
        fclose(file);
    } else {
        printf("Failed to open the file for writing.\n");
    }
}


// Function to deserialize the user array from a binary file
user* deserializeUsers(const char* filename, int* numUsers) {
    FILE* file = fopen(filename, "rb");
    if (file != NULL) {
        fread(numUsers, sizeof(int), 1, file); // Read the number of users

        user* users = (user*)malloc((*numUsers) * sizeof(user));
        if (users == NULL) {
            fclose(file);
            return NULL; // Memory allocation failed
        }

        for (int i = 0; i < *numUsers; ++i) {
            fread(&(users[i].id), sizeof(int), 1, file);
            fread(&(users[i].nb_ratings), sizeof(int), 1, file);

            users[i].ratings = (rating*)malloc(users[i].nb_ratings * sizeof(rating));
            if (users[i].ratings == NULL) {
                // Handle memory allocation failure
                fclose(file);
                for (int j = 0; j < i; ++j) {
                    free(users[j].ratings);
                }
                free(users);
                return NULL;
            }

            // Deserialize each rating for the user
            for (int j = 0; j < users[i].nb_ratings; ++j) {
                fread(&(users[i].ratings[j].id_user), sizeof(int), 1, file);
                fread(&(users[i].ratings[j].id_film), sizeof(int), 1, file);
                fread(&(users[i].ratings[j].year), sizeof(int), 1, file);
                fread(&(users[i].ratings[j].day), sizeof(int), 1, file);
                fread(&(users[i].ratings[j].month), sizeof(int), 1, file);
                fread(&(users[i].ratings[j].star), sizeof(int), 1, file);
            }
        }

        fclose(file);
        return users;
    } else {
        printf("Failed to open the file for reading.\n");
        return NULL;
    }
}


void freeUsers(user* users, int numUsers) {
    for (int i = 0; i < numUsers; i++) {
        free(users[i].ratings);
    }
    free(users);
}