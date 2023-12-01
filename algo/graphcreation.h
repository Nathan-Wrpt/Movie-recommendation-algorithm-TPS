#ifndef GRAPHCREATION_H
#define GRAPHCREATION_H

#include "../bin_creation/user.h"
#include "../bin_creation/movies.h"
#include <stdbool.h>
#include <time.h>

#define INIT_WEIGHT 0
#define RATINGS_CONSIDERED 20

float** initGraph(int nbMovies);
void freeGraph(float** graph, int nbMovies);
float updateWeight(int rating1, int rating2);
void updateGraphUser(user user, float** graph, int limitDate);
bool isUserIgnored(int* ignoredUsers, int nbIgnoredUsers, int userId);
int findIdUser(user* users, int nbUsers, int idUser);
void updateGraph(float** graph, user* users, int nbUsers, int* ignoredUsers, int nbIgnoredUsers, int* privilegedUser, int nbPrivilegedUser, int minRatings, int limitDate);
int* getNClosestMovies(int* moviesID, int numFilmsID, float** graph, int n);

#endif // GRAPHCREATION_H