#include "graphcreation.h"

float** initGraph(int nbMovies){
    float** graph = malloc(nbMovies * sizeof(float*));
    for(int i = 0; i < nbMovies; i++){
        graph[i] = malloc(nbMovies * sizeof(float));
        for(int j = 0; j < nbMovies; j++){
            graph[i][j] = INIT_WEIGHT;
        }
    }
    return graph;
}

void freeGraph(float** graph, int nbMovies){
    for(int i = 0; i < nbMovies; i++){
        free(graph[i]);
    }
    free(graph);
}

float updateWeight(int rating1, int rating2) {
    // Weight matrix (explanation in the readme)
    float weights[5][5] = {
        {-0.25, -0.17,  0.0,   0.46,  1.0},
        {-0.17, -0.62, -0.46,  0.0,   0.5},
        { 0.0,  -0.46, -1.0,  -0.5,   0.0},
        { 0.46,  0.0,  -0.5,   1.0,  -0.5},
        { 1.0,   0.5,   0.0,  -0.5,  -1.0}
    };

    if (rating1 >= 1 && rating1 <= 5 && rating2 >= 1 && rating2 <= 5) {
        return weights[rating1 - 1][rating2 - 1];
    } else {
        return 0; //Default value
    }
}

void updateGraphUser(user user, float** graph, int limitDate){

    // For each different couple (i, j) of user ratings we update the graph
    for(int i = 0; i < user.nb_ratings && i < RATINGS_CONSIDERED; i++){
        for(int j = i + 1; j < user.nb_ratings && j < RATINGS_CONSIDERED; j++){

            // If the two ratings are before the limit date, we update the graph
            if(user.ratings[i].year < limitDate && user.ratings[j].year < limitDate){
                float updatedweight = updateWeight(user.ratings[i].star, user.ratings[j].star);
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

void updateGraph(float** graph, user* users, int nbUsers, int* ignoredUsers, int nbIgnoredUsers, int* privilegedUser, int nbPrivilegedUser, int minRatings, int limitDate){
    
    // We update the graph only taking in consideration the ratings of the privileged users
    if(privilegedUser != NULL){
        for(int i = 0; i < nbPrivilegedUser; i++){
            int idUser = findIdUser(users, nbUsers, privilegedUser[i]);
            if(idUser != -1){
                updateGraphUser(users[idUser], graph, limitDate);
            }
        }
        return;
    }

    // We update the graph for each user that has enough ratings and is not in the ignoredUsers array
    for(int i = 0; i < nbUsers; i++){
        if(isUserIgnored(ignoredUsers, nbIgnoredUsers, users[i].id)){
            continue;
        }
        
        else {
            if(users[i].nb_ratings >= minRatings){
                updateGraphUser(users[i], graph, limitDate);
            }
        }
    }
}

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
                    if (graph[moviesIDs[nbMovieInTab] - 1][movieID] < minWeights[i]) {
                        minWeights[i] = graph[moviesIDs[nbMovieInTab] - 1][movieID];
                        isMovieInClosest = true;
                    }
                    break;
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
            if (minWeights[j] < minWeights[min]) {
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
    return 1 / maxAbs;
}

// based on nathan algorithm in order to ponderate the influence of
// blockbuster in order to promote other kind of movies
int* getNClosestMovies2(int* moviesIDs, int numFilmsID, float** graph, int n) {

    float* ponderation = malloc(numFilmsID * sizeof(float)); 
    for (int i = 0; i < numFilmsID; i++) {
        ponderation[i] = maxInv(graph[moviesIDs[i] - 1], NBMOVIES);
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
                }
                break;
                isMovieInClosest = true;
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

    return closestMoviesID;
}

movie* deserializeMovies(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (file != NULL) {
        int numMovies;
        fread(&numMovies, sizeof(int), 1, file); // Read the number of movies

        // Allocate memory for movies
        movie* movies = (movie*)malloc(numMovies * sizeof(movie));

        for (int i = 0; i < numMovies; ++i) {
            fread(&(movies[i].id), sizeof(int), 1, file);
            fread(&(movies[i].release_date), sizeof(int), 1, file);

            // Read title length and allocate memory to read the title
            int titleLength;
            fread(&titleLength, sizeof(int), 1, file);
            fread(movies[i].title, sizeof(char), titleLength, file);
            movies[i].title[titleLength] = '\0'; // Null-terminate the string

            fread(&(movies[i].nb_ratings), sizeof(int), 1, file);

            // Allocate memory for ratings
            movies[i].ratings = (rating*)malloc(movies[i].nb_ratings * sizeof(rating));

            // Deserialize each rating within the movie
            for (int j = 0; j < movies[i].nb_ratings; ++j) {
                fread(&(movies[i].ratings[j].id_user), sizeof(int), 1, file);
                fread(&(movies[i].ratings[j].id_film), sizeof(int), 1, file);
                fread(&(movies[i].ratings[j].year), sizeof(int), 1, file);
                fread(&(movies[i].ratings[j].day), sizeof(int), 1, file);
                fread(&(movies[i].ratings[j].month), sizeof(int), 1, file);
                fread(&(movies[i].ratings[j].star), sizeof(int), 1, file);
            }
        }

        fclose(file);
        return movies;
    } else {
        printf("Failed to open the file for reading.\n");
        return NULL;
    }
}

void freeMovies(movie* movies, int numMovies) {
    for (int i = 0; i < numMovies; i++) {
        free(movies[i].ratings);
    }
    free(movies);
}

int main(){
    int nbUsers;
    user* users = deserializeUsers("../bin_creation/users.bin", &nbUsers);
    movie* movies = deserializeMovies("../bin_creation/movies.bin");

    printf("Number of users: %d\n", nbUsers);
    fflush(stdout);

    float** graph = initGraph(NBMOVIES);

    printf("Graph initialized\n");
    fflush(stdout);

    clock_t begin = clock();
    int nbUsersTest = NBUSERS - 5;
    updateGraph(graph, users, nbUsersTest, NULL, 0, NULL, 0, 0, 2010);
    clock_t end = clock();

    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("The graph took %f seconds to be updated with %d users.\n", time_spent, nbUsersTest);

    int n = 10;
    int moviesIDs1[3] = {17482, 11265, 7617};

    // printf("===========================================================\n");
    // for (int i = 0; i < 3; i ++) {
        
    //     printf("Pour le film %s(%d) le film recommandé est :%s (%f)\n", movies[moviesIDs1[i] - 1].title, moviesIDs1[i], movies[min(graph[moviesIDs1[i] - 1], NBMOVIES)].title, graph[moviesIDs1[i] - 1][min(graph[moviesIDs1[i] - 1], NBMOVIES)]);
    // }

    int* recommendedMovies = getNClosestMovies2(moviesIDs1, 3, graph, n);
    printf("===========================================================\n");
    printf("Les titres des %d films recommandés si vous aimez les films :\n", n);
    for (int i = 0; i < 3; i++) {
        printf("- %s(%d)\n", movies[moviesIDs1[i] - 1].title, moviesIDs1[i]);
    }
    printf("sont :\n");
    for(int i = 0; i < n; i++){
        printf("Film %02d/%d recommandé : %s(%d)\n", i+1, n, movies[recommendedMovies[i] - 1].title, recommendedMovies[i]);
    }

    free(recommendedMovies);


    int moviesIDs2[6] = {5926, 16265, 9886, 9628, 8687, 5582};
    int* recommendedMovies2 = getNClosestMovies(moviesIDs2, 6, graph, n);
    printf("===========================================================\n");
    printf("Les titres des %d films recommandés si vous aimez les films :\n", n);
    for (int i = 0; i < 6; i++) {
        printf("- %s(%d)\n", movies[moviesIDs2[i] - 1].title, moviesIDs2[i]);
    }
    printf("sont :\n");
    for(int i = 0; i < n; i++){
        printf("Film %02d/%d recommandé : %s(%d)\n", i+1, n, movies[recommendedMovies2[i] - 1].title, recommendedMovies2[i]);
    }

    free(recommendedMovies2);

    freeMovies(movies, NBMOVIES);
    freeGraph(graph, NBMOVIES);
    freeUsers(users, nbUsers);

    printf("Graph freed\n");
    fflush(stdout);

    return 0;
    
}