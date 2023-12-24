#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include "bin_creation/movies.h"
#include "bin_creation/user.h"
#include "algo/graphcreation.h"

//Compte le nombre de films likés après l'option -r, séparés par des ','
int num_movies_liked(char* moviesLiked){
    char* copy = malloc(strlen(moviesLiked) + 1);
    strcpy(copy, moviesLiked);
    int num = 0;
    char* token = strtok(copy, ",");
    while(token != NULL){
        num++;
        token = strtok(NULL, ",");
    }
    free(copy);
    return num;
}

//Parse les films likés après l'option -r et les stocke dans le tableau pris en paramètre (moviesLikedParsed)
void movies_liked_parsing(char* moviesLiked, int** moviesLikedParsed, int numMoviesLiked){
    char* copy = malloc(strlen(moviesLiked) + 1);
    strcpy(copy, moviesLiked);
    char* token = strtok(copy, ",");
    int i = 0;
    while(token != NULL && i < numMoviesLiked){
        (*moviesLikedParsed)[i] = atoi(token);
        token = strtok(NULL, ",");
        i++;
    }
    free(copy);
}

void displayWelcomeMessage() {
    char text[] = "Welcome to the movie recommendation system by :";
    int i;

    for (i = 0; text[i] != '\0'; i++) {
        if (i % 3 == 0) {
            printf("\033[0;31m"); // Red text
        } else if (i % 3 == 1) {
            printf("\033[0;32m"); // Green text
        } else {
            printf("\033[0;33m"); // Magenta text
        }
        printf("%c", text[i]);
        fflush(stdout);
        usleep(50000); // Pause for 50000 microseconds (0.05 seconds)
    }

    printf("\033[0;32m"); // Reset text color to default
}

void displayNWEC() {
    printf("\033[0;33m");
    printf("\n\n");
    printf("  N   N    W   W  -  EEEEE  CCCC\n");
    fflush(stdout);
    usleep(200000); // Pause for 500000 microseconds (0.5 seconds)

    printf("  NN  N    W   W  -  E      C   \n");
    fflush(stdout);
    usleep(200000);

    printf("  N N N    W   W  -  EEEE   C   \n");
    fflush(stdout);
    usleep(200000);

    printf("  N  NN    W W W  -  E      C   \n");
    fflush(stdout);
    usleep(200000);

    printf("  N   N    WW WW  -  EEEEE  CCCC\n");
    fflush(stdout);
    usleep(200000);
    printf("\033[0;32m");
}



int main(){

    float weights[5][5] = {
        {-0.25, -0.17,  0.0,   0.46,  1.0},
        {-0.17, -0.62, -0.46,  0.0,   0.5},
        { 0.0,  -0.46, -1.0,  -0.5,   0.0},
        { 0.46,  0.0,  -0.5,   1.0,  -0.5},
        { 1.0,   0.5,   0.0,  -0.5,  -1.0}
    };
    displayWelcomeMessage();
    displayNWEC();
    printf("Do you want to get some statistics about a specific movie ? (y/n)\n");
    char* response = malloc(10*sizeof(char));
    scanf("%s", response);
    if(strcmp(response, "y") == 0){
        printf("Please enter the ID of the movie you want to get statistics about.\n");
        int film_id;
        scanf("%d", &film_id);
        //TODO
    }
    printf("Please enter all the IDs of your favourite movies (separated by a comma, without spaces) to get a list of recommended movies.\n");
    char* moviesLiked = malloc(10000*sizeof(char));
    scanf("%s", moviesLiked);
    int numMoviesLiked = num_movies_liked(moviesLiked);
    int* moviesLikedParsed = malloc(numMoviesLiked * sizeof(int));
    movies_liked_parsing(moviesLiked, &moviesLikedParsed, numMoviesLiked);
    printf("Please enter the number of movies you want to be recommended.\n");
    int numMoviesRecommended;
    scanf("%d", &numMoviesRecommended);
    printf("Do you want to set a custom path to the folder where the files corresponding to the requested results will be saved ? (y/n)\n");
    scanf("%s", response);
    char* folderpath = malloc(1000*sizeof(char));
    if(strcmp(response, "y") == 0){
        printf("Please enter the path to the folder where the files corresponding to the requested results will be saved.\n");
        scanf("%s", folderpath);
    }
    else{
        folderpath = NULL;
    }
    printf("Do you want to ignore reviews if they were done after a certain year ? (y/n)\n");
    scanf("%s", response);
    int dateLimit;
    if(strcmp(response, "y") == 0){
        printf("Please enter the year after which you want to ignore reviews.\n");
        scanf("%d", &dateLimit);
    }
    else{
        dateLimit = 2023;
    }
    printf("Do you want to ignore some users' reviews ? (y/n)\n");
    scanf("%s", response);
    char* clients = malloc(10000*sizeof(char));
    int numClients = 0;
    int* clientsParsed = malloc(numClients * sizeof(int));
    if(strcmp(response, "y") == 0){
        printf("Please enter the IDs of the users you want to ignore (separated by a comma, without spaces).\n");
        scanf("%s", clients);
        int numClients = num_movies_liked(clients);
        movies_liked_parsing(clients, &clientsParsed, numClients);
    }
    else{
        clients = NULL;
    }
    printf("On the contrary, do you want to take into account only some users' reviews ? (y/n)\n");
    scanf("%s", response);
    char* privilegedclients = malloc(10000*sizeof(char));
    int numPrivilegedClients = 0;
    int* privilegedClientsParsed = malloc(numPrivilegedClients * sizeof(int));
    if(strcmp(response, "y") == 0){
        printf("Please enter the IDs of the users you want to take into account (separated by a comma, without spaces).\n");
        scanf("%s", privilegedclients);
        int numPrivilegedClients = num_movies_liked(privilegedclients);
        movies_liked_parsing(privilegedclients, &privilegedClientsParsed, numPrivilegedClients);
    }
    else{
        privilegedclients = NULL;
    }
    printf("Do you want to take in account only users who have reviewed at least a certain number of movies ? (y/n)\n");
    scanf("%s", response);
    int minmoviesreviewed;
    if(strcmp(response, "y") == 0){
        printf("Please enter the minimum number of movies a user must have reviewed to be taken into account.\n");
        scanf("%d", &minmoviesreviewed);
    }
    else{
        minmoviesreviewed = 0;
    }
    printf("Finally, do you want to get the execution time of the algorithm ? (y/n)\n");
    scanf("%s", response);
    bool time;
    if(strcmp(response, "y") == 0){
        time = true;
    }
    else{
        time = false;
    }
    printf("Please wait while we are processing your request...\n");
    clock_t start = clock();
    // movie* movies = createMovieTable("./data/movies.csv", "./data/training_set/");
    // user* users = createUserTable("./data/users.csv", "./data/training_set/");
    // float** graph = initGraph(NBMOVIES);
    // updateGraph(graph, users, NBUSERS, clientsParsed, numClients, privilegedClientsParsed, numPrivilegedClients, minmoviesreviewed, dateLimit, weights);
    // int* closestMoviesID = getNClosestMovies2(moviesLikedParsed, numMoviesLiked, graph, numMoviesRecommended);
    clock_t end = clock();
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    if(time){
        printf("Execution time of the program was : %f secondes.\n", time_spent);
    }
    // printf("Based on the movies you liked (");
    // for(int i = 0; i < numMoviesLiked; i++){
    //     printf("%s", movies[moviesLikedParsed[i]].title);
    //     if(i != numMoviesLiked - 1){
    //         printf(", ");
    //     }
    // }
    // printf("), we recommend you the following movies:\n");
    // for(int i = 0; i < numMoviesRecommended; i++){
    //     printf("-%s\n", movies[closestMoviesID[i]].title);
    // }
}