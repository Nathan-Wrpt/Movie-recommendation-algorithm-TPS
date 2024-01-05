#include "graphcreation.h"

/**
 * @file graphcreation.c
 * @brief Functions to create the graph representing the similitarity between movies.
 */

float** initGraph(int nbMovies){
    float** graph = malloc(nbMovies * sizeof(float*));
    for(int i = 0; i < nbMovies; i++){
        graph[i] = calloc(nbMovies, sizeof(float));
    }
    return graph;
}

void freeGraph(float** graph, int nbMovies){
    for(int i = 0; i < nbMovies; i++){
        free(graph[i]);
    }
    free(graph);
}

float updateWeight(int rating1, int rating2, float weights[5][5]) {
    if (rating1 >= 1 && rating1 <= 5 && rating2 >= 1 && rating2 <= 5) {
        return weights[rating1 - 1][rating2 - 1];
    } else {
        return 0; //Default value
    }
}

void updateGraphUser(user user, float** graph, int limitDate, float weights[5][5], int ratingsConsidered){

    // For each different couple (i, j) of user ratings we update the graph
    for(int i = 0; i < user.nb_ratings && i < ratingsConsidered; i++){
        for(int j = i + 1; j < user.nb_ratings && j < ratingsConsidered; j++){

            // If the two ratings are before the limit date, we update the graph
            if(user.ratings[i].year < limitDate && user.ratings[j].year < limitDate){
                float updatedweight = updateWeight(user.ratings[i].star, user.ratings[j].star, weights);
                graph[user.ratings[i].id_film - 1][user.ratings[j].id_film - 1] += updatedweight;
                graph[user.ratings[j].id_film - 1][user.ratings[i].id_film - 1] += updatedweight;
            }

        }
    }
}

bool isUserIgnored(int* ignoredUsers, int nbIgnoredUsers, int userId) {
    for (int i = 0; i < nbIgnoredUsers; i++){
        if(ignoredUsers[i] == userId){
            return true;
        }
    }
    return false;
}

int findIdUser(user* users, int nbUsers, int idUser){
    for(int i = 0; i < nbUsers; i++){
        if(users[i].id == idUser){
            return i;
        }
    }
    return -1;
}

void updateGraph(float** graph, user* users, int nbUsers, int* ignoredUsers, int nbIgnoredUsers, int* privilegedUser, int nbPrivilegedUser, int minRatings, int limitDate, float weights[5][5], int ratingsConsidered){
    
    // We update the graph only taking in consideration the ratings of the privileged users
    if(privilegedUser != NULL){
        for(int i = 0; i < nbPrivilegedUser; i++){
            int idUser = findIdUser(users, nbUsers, privilegedUser[i]);
            if(idUser != -1){
                updateGraphUser(users[idUser], graph, limitDate, weights, ratingsConsidered);
            }
            updateProgressBar((int) (100 * (float) i / nbPrivilegedUser));
        }
        return;
    }

    // We update the graph for each user that has enough ratings and is not in the ignoredUsers array
    for(int i = 0; i < nbUsers; i++){
        if(isUserIgnored(ignoredUsers, nbIgnoredUsers, users[i].id)){
            continue;
            printf("User %d ignored\n", users[i].id);
        }
        
        else {
            if(users[i].nb_ratings >= minRatings){
                updateGraphUser(users[i], graph, limitDate, weights, ratingsConsidered);
            }
        }
        if(i % 500 == 0){
            updateProgressBar((int) (100 * (float) i / nbUsers));
        }
    }
}

int min(float* array, int size){
    int min = 0;
    for(int i = 1; i < size; i++){
        if(array[i] < array[min]){
            min = i;
        }
    }
    return min;
}

int max (float* array, int size){
    int max = 0;
    for(int i = 1; i < size; i++){
        if(array[i] > array[max]){
            max = i;
        }
    }
    return max;
}

int* getNClosestMovies(int* moviesIDs, int numFilmsID, float** graph, int n){
    int* closestMoviesID = malloc(n * sizeof(int));
    float* minWeights = malloc(n * sizeof(float));
    for (int i = 0; i < n; i++) minWeights[i] = 1000;

    for (int nbMovieInTab = 0; nbMovieInTab < numFilmsID; nbMovieInTab++) {
        for(int movieID = 0; movieID < NBMOVIES; movieID++) {

            // if the movie is already in the moviesIDs array we continue
            bool isMovieInTab = false;
            for (int i = 0; i < numFilmsID; i++) {
                if (movieID + 1 == moviesIDs[i]) {
                    isMovieInTab = true;
                    break;
                }
            }

            // if the movie is already in the closestMoviesID array we update its weight if
            // the weight is lower than the current weight associated to the movie
            bool isMovieInClosest = false;
            for (int i = 0; i < n; i++) {
                if (movieID + 1 == closestMoviesID[i]) {
                    isMovieInClosest = true;
                    if (graph[moviesIDs[nbMovieInTab] - 1][movieID] < minWeights[i]) {
                        minWeights[i] = graph[moviesIDs[nbMovieInTab] - 1][movieID];
                        break; // Update the weight and break the loop
                    }
                }
            }

            if (isMovieInTab || isMovieInClosest) {
                continue;
            }

            // Otherwise, if the weight is lower than the max weight in the array we update the array
            int idMax = max(minWeights, n);
            if (graph[moviesIDs[nbMovieInTab] - 1][movieID] < minWeights[idMax]) {
                minWeights[idMax] = graph[moviesIDs[nbMovieInTab] - 1][movieID];
                closestMoviesID[idMax] = movieID + 1;
            }
        }
    }

    // sort the array
    for (int i = 0; i < n; i++) {
        int min = i;
        for (int j = i + 1; j < n; j++) {
            // If weights are equal, maintain the original order
            if (minWeights[j] == minWeights[min]) {
                if (closestMoviesID[j] < closestMoviesID[min]) {
                    min = j;
                }
            } else if (minWeights[j] < minWeights[min]) {
                min = j;
            }
        }
        float tmp = minWeights[i];
        minWeights[i] = minWeights[min];
        minWeights[min] = tmp;

        int tmp2 = closestMoviesID[i];
        closestMoviesID[i] = closestMoviesID[min];
        closestMoviesID[min] = tmp2;
    }

    free(minWeights);
    return closestMoviesID;
}

float absFloat(float value) {
    if (value < 0) {
        return -1 * value;
    }
    return value;
}

float maxInv(float* values, int size) {
    float maxAbs = 0;
    for (int i = 0; i < size; i++) {
        float absValue = absFloat(values[i]);
        if (absValue > maxAbs) {
            maxAbs = absValue;
        }
    }
    if(maxAbs == 0){
        return 0;
    }
    return 1 / maxAbs;
}

// based on nathan algorithm in order to ponderate the influence of
// each movies liked based on the fame of the movie
int* getNClosestMovies2(int* moviesIDs, int numFilmsID, float** graph, int n) {

    float* ponderation = malloc(numFilmsID * sizeof(float)); 
    for (int i = 0; i < numFilmsID; i++) {
        float valpond = maxInv(graph[moviesIDs[i] - 1], NBMOVIES);
        ponderation[i] = valpond;
    }
    float sumponderation = 0;
    for (int i = 0; i < numFilmsID; i++) {
        sumponderation += ponderation[i];
    }
    float* weights = malloc(NBMOVIES * sizeof(float));
    for (int movies = 0; movies < NBMOVIES; movies++) {
        float sum = 0;
        for (int i = 0; i < numFilmsID; i++) {
            sum += graph[movies][moviesIDs[i] - 1] * ponderation[i];
        }
        weights[movies] = sum;
    }

    int* closestMoviesID = malloc(n * sizeof(int));
    float* minWeights = malloc(n * sizeof(int));
    for(int i = 0; i < n; i++) minWeights[i] = 1000;

    // We update the closestMoviesID array
    for (int movieID = 0; movieID < NBMOVIES; movieID++) {

        // if the movie is already in the moviesIDs array we continue
        bool isMovieInTab = false;
        for (int i = 0; i < numFilmsID; i++) {
            if (movieID + 1 == moviesIDs[i]) {
                isMovieInTab = true;
                break;
            }
        }
    
        // if the movie is already in the closestMoviesID array we update its weight if
        // the weight is lower than the current weight associated to the movie
        bool isMovieInClosest = false;
        for (int i = 0; i < n; i++) {
            if (movieID + 1 == closestMoviesID[i]) {
                if (weights[movieID] < minWeights[i]) {
                    minWeights[i] = weights[movieID];
                    isMovieInClosest = true; // Update flag for movie found
                }
                break; // Break after updating
            }
        }

        if (isMovieInTab || isMovieInClosest) {
            continue;
        }

        // Otherwise, if the weight is lower than the max weight in the array we update the array
        int idMax = max(minWeights, n);
        if (weights[movieID] < minWeights[idMax]) {
            minWeights[idMax] = weights[movieID];
            closestMoviesID[idMax] = movieID + 1;
        }
    }

    free(ponderation);
    free(weights);
    free(minWeights);
    
    return closestMoviesID;
}

void serializegraph(float** graph, char* path){
    FILE* file = fopen(path, "wb");
    if (file != NULL) {
        for (int i = 0; i < NBMOVIES; i++) {
            for(int j = 0; j < NBMOVIES; j++){
                fwrite(&(graph[i][j]), sizeof(float), 1, file);
            }
            if(i % 10 == 0){
                updateProgressBar((int) (100 * (float) i / NBMOVIES));
            }
        }
        fclose(file);
    } else {
        printf("Failed to open the file for writing.\n");
    }
}

float** deserializegraph(char* path){
    FILE* file = fopen(path, "rb");
    if (file != NULL) {
        float** graph = malloc(NBMOVIES * sizeof(float*));
        for(int i = 0; i < NBMOVIES; i++){
            graph[i] = malloc(NBMOVIES * sizeof(float));
            for(int j = 0; j < NBMOVIES; j++){
                fread(&(graph[i][j]), sizeof(float), 1, file);
            }
            if(i % 10 == 0){
                updateProgressBar((int) (100 * (float) i / NBMOVIES));
            }
        }
        fclose(file);
        return graph;
    } else {
        printf("Failed to open %s. It might not exist (normal if you haven't downloaded or created the bin files using -o yet\n", path);
        return NULL;
    }
}