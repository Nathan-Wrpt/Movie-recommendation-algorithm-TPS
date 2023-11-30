#include "../bin_creation/user.h"
#include "../bin_creation/movies.h"
#define NB_MAX_REVIEWS 232943


//Creates a graph with nbUsers nodes and initWeight as weight for each edge(the matrix is symetric)
int** initGraph(int nbUsers, int initWeight);

//Function to free the graph
void freeGraph(int** graph, int nbUsers);

//Given 2 ratings from a single user, tells if the 2 movies should be nearer or not and updates the graph accordingly
float updateWeight(int rating1, int rating2);

//Given a user and the graph, updates the distances knowing the ratings of the user (using the function updateWeight)
void updateGraphuser(user user, int** graph);

//iterates the previous function for all users. After calling this function, the graph is supposed to be finished
void updateGraph(int** graph, user* users, int nbUsers);





