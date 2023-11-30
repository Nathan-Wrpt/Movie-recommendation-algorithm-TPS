#include "graphcreation.h"

int** initGraph(int nbMovies, int initWeight){
    int** graph = malloc(nbMovies*sizeof(int*));
    for(int i = 0; i < nbMovies; i++){
        graph[i] = malloc(nbMovies*sizeof(int));
        for(int j = 0; j < nbMovies; j++){
            graph[i][j] = initWeight;
        }
    }
    return graph;
}

void freeGraph(int** graph, int nbMovies){
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

void updateGraphuser(user user, int** graph){

}