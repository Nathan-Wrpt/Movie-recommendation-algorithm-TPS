#ifndef GRAPHCREATION_H
#define GRAPHCREATION_H
#include "../all.h"
#include "../util/progressbar.h"

//initialize a graph as an adjacency matrix of size nbMovies*nbMovies
float** initGraph(int nbMovies);

//free the memory allocated for a graph
void freeGraph(float** graph, int nbMovies);

//returns how much the distance between two movies should be adjusted according to two ratings
float updateWeight(int rating1, int rating2, float weights[5][5]);

//updates the graph for all the ratings of a user
void updateGraphUser(user user, float** graph, int limitDate, float weights[5][5], int ratingsConsidered);

//function to check if a user is considered as a bad reviewer
bool isUserIgnored(int* ignoredUsers, int nbIgnoredUsers, int userId);

//returns the index of a user in the array of users
int findIdUser(user* users, int nbUsers, int idUser);

//updates the graph for all the users (iterates the updateGraphUser function)
void updateGraph(float** graph, user* users, int nbUsers, int* ignoredUsers, int nbIgnoredUsers, int* privilegedUser, int nbPrivilegedUser, int minRatings, int limitDate, float weights[5][5], int ratingsConsidered);

//Given movies liked by the user, returns the n movies that should be recommended acording to the graph 
int* getNClosestMovies(int* moviesID, int numFilmsID, float** graph, int n);

//Same but this version ponderates avery movie so that flagship movies in the movies liked don't have too much influence over the others
int* getNClosestMovies2(int* moviesIDs, int numFilmsID, float** graph, int n);

//Turns the graph into a binary file
void serializegraph(float** graph, char* path);

//turns a bin file into a graph
float** deserializegraph(char* path);

#endif // GRAPHCREATION_H
