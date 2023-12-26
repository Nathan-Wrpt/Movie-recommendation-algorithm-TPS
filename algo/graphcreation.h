#ifndef GRAPHCREATION_H
#define GRAPHCREATION_H
#include "../all.h"

float** initGraph(int nbMovies);
void freeGraph(float** graph, int nbMovies);
float updateWeight(int rating1, int rating2, float weights[5][5]);
void updateGraphUser(user user, float** graph, int limitDate, float weights[5][5]);
bool isUserIgnored(int* ignoredUsers, int nbIgnoredUsers, int userId);
int findIdUser(user* users, int nbUsers, int idUser);
void updateGraph(float** graph, user* users, int nbUsers, int* ignoredUsers, int nbIgnoredUsers, int* privilegedUser, int nbPrivilegedUser, int minRatings, int limitDate, float weights[5][5]);
int* getNClosestMovies(int* moviesID, int numFilmsID, float** graph, int n);
int* getNClosestMovies2(int* moviesIDs, int numFilmsID, float** graph, int n);
void updateProgressBar(int percentage);

#endif // GRAPHCREATION_H
