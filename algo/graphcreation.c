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

// int* getNClosestMovies(int* moviesID, int numFilmsID, float** graph, int n){
//     int* closestMoviesID = malloc(n * sizeof(int));
//     int* minWeights = malloc(n * sizeof(int));
//     for(int i = 0; i < n; i++) minWeights[i] = 1000;

//     for (int nbMovieInTab = 0; nbMovieInTab < numFilmsID; nbMovieInTab++) {
//         printf("Studying movie with id %d\n", moviesID[nbMovieInTab]);
//         for(int movieID = 0; movieID < NBMOVIES; movieID++) {

//             // a film can't be added in closestMoviesID if it is in moviesID given
//             // in arg 
//             bool flagContinue = false;
//             for (int i = 0; i < numFilmsID; i++) {
//                 if (movieID + 1 == moviesID[nbMovieInTab]) {
//                     printf("%d trouve et ne peux pas être ajouté\n", movieID + 1);
//                     flagContinue = true;
//                 }
//             }
//             if (flagContinue) continue;

//             // verifying if the distance is one of the n min -> added one time (break)
//             for (int i = 0; i < n; i++) {
//                 if (graph[moviesID[nbMovieInTab] - 1][movieID] < minWeights[i]) {
//                     minWeights[i] = graph[moviesID[nbMovieInTab] - 1][movieID];
//                     closestMoviesID[i] = movieID + 1;
//                     break;
//                 }
//             }
//         }
//     }

//     free(minWeights);
//     return closestMoviesID;
// }

int* getNClosestMovies(int* moviesID, int numFilmsID, float** graph, int n){
    int* closestMoviesID = malloc(n * sizeof(int));
    float* minWeights = malloc(n * sizeof(float));
    int* whoHasFound = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) minWeights[i] = 1000;

    for (int nbMovieInTab = 0; nbMovieInTab < numFilmsID; nbMovieInTab++) {
        for(int movieID = 0; movieID < NBMOVIES; movieID++) {

            bool flagContinue = false;
            for (int i = 0; i < numFilmsID; i++) {
                if (movieID + 1 == moviesID[i]) {
                    flagContinue = true;
                }
            }
            if (flagContinue) continue;

            // Verify if the distance is one of the n min -> added one time (break)
            // for (int i = 0; i < n; i++) {
            //     if (graph[moviesID[nbMovieInTab] - 1][movieID] < minWeights[i]) {
            //         minWeights[i] = graph[moviesID[nbMovieInTab] - 1][movieID];
            //         closestMoviesID[i] = movieID + 1;
            //         whoHasFound[i] = moviesID[nbMovieInTab];
            //         break;
            //     }
            // }

            int idMax = max(minWeights, n);
            if (graph[moviesID[nbMovieInTab] - 1][movieID] < minWeights[idMax]) {
                minWeights[idMax] = graph[moviesID[nbMovieInTab] - 1][movieID];
                closestMoviesID[idMax] = movieID + 1;
                whoHasFound[idMax] = moviesID[nbMovieInTab];
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

        int tmp3 = whoHasFound[i];
        whoHasFound[i] = whoHasFound[min];
        whoHasFound[min] = tmp3;
    }

    for (int i = 0; i < n; i++) {
        printf("%d has found %d (%f)\n", whoHasFound[i], closestMoviesID[i], minWeights[i]);
    }

    free(whoHasFound);
    free(minWeights);
    return closestMoviesID;
}


int main(){
    int nbUsers;
    user* users = deserializeUsers("../bin_creation/users.bin", &nbUsers);

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

    printf("Graph updated\n");
    printf("Le film recommandé si vous aimez le film 5926 FIGHT CLUB est le film %d (%f)\n", min(graph[5925], NBMOVIES) + 1, graph[5925][min(graph[5925], NBMOVIES)]);
    printf("Le film recommandé si vous aimez le film 16265 STW4 est le film %d (%f)\n", min(graph[16264], NBMOVIES) + 1, graph[16264][min(graph[16264], NBMOVIES)]);
    printf("Le film recommandé si vous aimez le film 9886 STW1 est le film %d (%f)\n", min(graph[9885], NBMOVIES) + 1, graph[9885][min(graph[9885], NBMOVIES)]);
    printf("Le film recommandé si vous aimez le film 9628 STW6 est le film %d (%f)\n", min(graph[9627], NBMOVIES) + 1, graph[9627][min(graph[9627], NBMOVIES)]);
    printf("Le film recommandé si vous aimez le film 8687 STW2 est le film %d (%f)\n", min(graph[8686], NBMOVIES) + 1, graph[8686][min(graph[8686], NBMOVIES)]);
    printf("Le film recommandé si vous aimez le film 5582 STW5 est le film %d (%f)\n", min(graph[5581], NBMOVIES) + 1, graph[5581][min(graph[5581], NBMOVIES)]);

    int n = 10;
    int moviesID[6] = {5926, 6265, 9886, 9628, 8687, 5582};
    int* recommendedMovies = getNClosestMovies(moviesID, 6, graph, n);
    printf("===========================================================\n");
    printf("Les titres des %d films recommandés si vous aimez les films", n);
    for (int i = 0; i < 6; i++) {
        printf(" %d", moviesID[i]);
    }
    printf(" sont :\n");
    for(int i = 0; i < n; i++){
        printf("Film %d/%d recommandé : %d\n", i+1, n, recommendedMovies[i]);
    }

    free(recommendedMovies);

    freeGraph(graph, NBMOVIES);
    freeUsers(users, nbUsers);

    printf("Graph freed\n");
    fflush(stdout);

    return 0;
    
}