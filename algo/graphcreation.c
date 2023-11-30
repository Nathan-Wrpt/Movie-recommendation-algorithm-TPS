#include "graphcreation.h"
#include <time.h>

#define MAXRATED 50

float** initGraph(int nbMovies, int initWeight){
    float** graph = malloc(nbMovies*sizeof(float*));
    for(int i = 0; i < nbMovies; i++){
        graph[i] = malloc(nbMovies*sizeof(float));
        for(int j = 0; j < nbMovies; j++){
            graph[i][j] = initWeight;
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
        return 0.0; //Default value
    }
}

void updateGraphuser(user user, float** graph, int dateLimit){
    for(int i = 0; i < user.nb_ratings && i < MAXRATED; i++){
        for(int j = i + 1; j < user.nb_ratings && j < MAXRATED; j++){
            if(user.ratings[i].year < dateLimit && user.ratings[j].year < dateLimit){
                float updatedweight = updateWeight(user.ratings[i].star, user.ratings[j].star);
                graph[user.ratings[i].id_film - 1][user.ratings[j].id_film - 1] += updatedweight;
                graph[user.ratings[j].id_film - 1][user.ratings[i].id_film - 1] += updatedweight;
            }
        }
    }
}

bool userinarray(user* users, int nbUsers, user user){
    if(users == NULL){
        return false;
    }
    for(int i = 0; i < nbUsers; i++){
        if(users[i].id == user.id){
            return true;
        }
    }
    return false;
}

void updateGraph(float** graph, user* users, int nbUsers, user* toBeIgnoredUsers, int nbIgnoredUsers, int dateLimit, int id_privilegeduser1, int id_privilegeduser2, int min_ratings_to_be_considered_a_good_reviewer){
    if(id_privilegeduser1 != -1 && id_privilegeduser2 != -1){
        int i = 0;
        while(i < nbUsers && users[i].id != id_privilegeduser1){
            i++;
        }
        int j = 0;
        while(j < nbUsers && users[j].id != id_privilegeduser2){
            j++;
        }
        updateGraphuser(users[i], graph, dateLimit);
        updateGraphuser(users[j], graph, dateLimit);
    }
    for(int i = 0; i < nbUsers; i++){
        if(userinarray(toBeIgnoredUsers, nbIgnoredUsers, users[i])){
            continue;
        }else{
            if(users[i].nb_ratings >= min_ratings_to_be_considered_a_good_reviewer){
                updateGraphuser(users[i], graph, dateLimit);
            }
        }
    }
}

//Given a film id, returns an array of the n ids of the n most close movies to the given one in the graph
int* getNClosestMovies(int filmid, float** graph, int n){
    //TO BE CODED
    return NULL;
}


// Function to deserialize the users.bin (here to test)
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

int meanreviewsperuser(user* users, int nbUsers){
    int sum = 0;
    for(int i = 0; i < nbUsers; i++){
        sum += users[i].nb_ratings;
    }
    return sum/nbUsers;
}

void serializeGraph(const char* filename, float** graph, int nbMovies) {
    FILE* file = fopen(filename, "wb"); // Open the file in binary write mode

    if (file != NULL) {
        // Write the entire matrix data into the file
        fwrite(*graph, sizeof(float), nbMovies * nbMovies, file);

        fclose(file); // Close the file after writing
    } else {
        printf("Unable to open the file for writing.\n");
    }
}

float** deserializeGraph(const char* filename, int nbMovies) {
    FILE* file = fopen(filename, "rb"); // Open the file in binary read mode

    if (file != NULL) {
        // Allocate memory for the matrix
        float** graph = (float**)malloc(nbMovies * sizeof(float*));
        for (int i = 0; i < nbMovies; i++) {
            graph[i] = (float*)malloc(nbMovies * sizeof(float));
        }

        // Read the entire matrix data from the file
        fread(*graph, sizeof(float), nbMovies * nbMovies, file);

        fclose(file); // Close the file after reading

        return graph;
    } else {
        printf("Unable to open the file for reading.\n");
        return NULL;
    }
}

//Returns the index of the minimum value of the array
int min(float* array, int size){
    int min = 0;
    for(int i = 1; i < size; i++){
        if(array[i] < array[min]){
            min = i;
        }
    }
    return min;
}


int main(){
    int nbUsers;
    user* users = deserializeUsers("../bin_creation/users.bin", &nbUsers);

    printf("Number of users: %d\n", nbUsers);
    fflush(stdout);

    float** graph = initGraph(NBMOVIES, NB_MAX_REVIEWS);

    printf("Graph initialized\n");
    fflush(stdout);

    clock_t begin = clock();
    int nbUsersTest = NBUSERS - 5;
    updateGraph(graph, users, nbUsersTest, NULL, 0, 2010, -1, -1, 0);
    clock_t end = clock();

    printf("Graph updated\n");
    // printf("distance ADC - STW1 : %f\n", graph[16264][9885]);
    // printf("distance ADC - STW6 : %f\n", graph[16264][9627]);
    // printf("distance ADC - STW2 : %f\n", graph[16264][8686]);
    // printf("distance ADC - STW5 : %f\n", graph[16264][5581]);

    // serializeGraph("graph.bin", graph, NBMOVIES);
    // printf("Graph serialized\n");


    freeGraph(graph, NBMOVIES);
    freeUsers(users, nbUsers);

    printf("Graph freed\n");
    fflush(stdout);

    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("The graph took %f seconds to be updated with %d users.\n", time_spent, nbUsersTest);

    return 0;
    
}