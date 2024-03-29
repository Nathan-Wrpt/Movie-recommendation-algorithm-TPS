#include "user.h"

/**
 * @file user.c
 * @brief Functions to create the user table thanks to the movie table.
 */

user* initUser(int id) {
    user* u = (user*) malloc(sizeof(user)); // Allocate memory for the user
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
    int* seenUserTable = (int*) malloc((IDMAXUSER + 1) * sizeof(int)); //size 4 * 2 600 000 = 10.4 Mo oof
    if (seenUserTable == NULL){
        printf("Error while allocating memory for seenUserTable\n");
        free(usersTable);
        exit(1);
    }
    for (int id = 0; id < IDMAXUSER; id++) seenUserTable[id] = -1;
    // for each film, check the id of reviewers and group the review
    // per reviewers
    for (int movie_id = 0; movie_id < NBMOVIES; movie_id++) {

        int nb_ratings = moviesTable[movie_id].nb_ratings;
        for (int num_rating = 0; num_rating < nb_ratings; num_rating++)
        {
            int id_user = moviesTable[movie_id].ratings[num_rating].id_user;

            // Check if user has been seen before to
            // attribute / find its place in the usersTable

            int placeInTable;
            if (seenUserTable[id_user] == -1) {
                placeInTable = nbUserSeen;
                nbUserSeen++;
                seenUserTable[id_user] = placeInTable;
                user* temp = initUser(id_user);
                usersTable[placeInTable] = *temp;
                free(temp);
                usersTable[placeInTable].ratings = (rating*) malloc(30 * sizeof(rating));
                usersTable[placeInTable].ratings[0] = moviesTable[movie_id].ratings[num_rating];

            } else {
                placeInTable = seenUserTable[id_user];
                int newNbRatings = usersTable[placeInTable].nb_ratings + 1;
                usersTable[placeInTable].nb_ratings = newNbRatings;
                if(newNbRatings % 30 == 0){ // Why 3O and not more ? Because of RAM usage
                    usersTable[placeInTable].ratings = realloc(usersTable[placeInTable].ratings, (newNbRatings + 30) * sizeof(rating));

                    //Handle allocation failure
                    if(usersTable[placeInTable].ratings == NULL){
                        printf("Error while reallocating memory for user %d\n", id_user);
                        free(usersTable[placeInTable].ratings);
                        free(usersTable);
                        free(seenUserTable);
                        exit(1);
                    }
                }
                usersTable[placeInTable].ratings[newNbRatings - 1] = moviesTable[movie_id].ratings[num_rating];
            }
        }
        
        if(movie_id % 10 == 0){
            updateProgressBar(movie_id * 100 / NBMOVIES);
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
            // Calculate the buffer size needed for each user
            size_t bufferSize = sizeof(int) * 2; // for id and nb_ratings
            bufferSize += sizeof(rating) * users[i].nb_ratings; // for each rating

            // Allocate buffer
            char* buffer = (char*)malloc(bufferSize);
            if (buffer == NULL) {
                printf("Memory allocation failed.\n");
                fclose(file);
                return;
            }

            // Copy user data to buffer
            char* ptr = buffer;
            memcpy(ptr, &(users[i].id), sizeof(int));
            ptr += sizeof(int);
            memcpy(ptr, &(users[i].nb_ratings), sizeof(int));
            ptr += sizeof(int);

            // Check if there are ratings to copy
            if (users[i].nb_ratings > 0 && users[i].ratings != NULL) {
                memcpy(ptr, users[i].ratings, sizeof(rating) * users[i].nb_ratings);
            }

            // Write buffer to file (buffer used to only fwrite once to reduce process time)
            fwrite(buffer, bufferSize, 1, file);

            // Free the buffer
            free(buffer);

            // Progress update
            if(i % 500 == 0) {
                updateProgressBar(i * 100 / numUsers);
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
        fread(numUsers, sizeof(int), 1, file); // Read the number of users and store it in numUsers (useless but found it interesting at first)

        user* users = (user*)malloc((*numUsers) * sizeof(user));
        if (users == NULL) {
            fclose(file);
            return NULL; // Memory allocation failed
        }

        for (int i = 0; i < *numUsers; ++i) {
            fread(&(users[i].id), sizeof(int), 1, file);
            fread(&(users[i].nb_ratings), sizeof(int), 1, file);

            if (users[i].nb_ratings > 0) {
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

                // Read all ratings in a single operation
                fread(users[i].ratings, sizeof(rating), users[i].nb_ratings, file);
            } else {
                users[i].ratings = NULL;
            }
            if(i % 500 == 0){
                updateProgressBar(i * 100 / *numUsers);
            }
        }

        fclose(file);
        return users;
    } else {
        printf("Failed to open %s. It might not exist (normal if you haven't downloaded or created the bin files using -o\n", filename);
        exit(1);
    }
}



void freeUsers(user* users, int numUsers) {
    for (int i = 0; i < numUsers; i++) {
        free(users[i].ratings);
    }
    free(users);
}