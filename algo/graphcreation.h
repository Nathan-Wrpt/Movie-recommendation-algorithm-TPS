#include "../bin_creation/user.h"
#include "../bin_creation/movies.h"
#define NB_MAX_REVIEWS 232943
#include <stdbool.h>


//Creates a graph with nbUsers nodes and initWeight as weight for each edge(the matrix is symetric)
float** initGraph(int nbUsers, int initWeight);

//Function to free the graph
void freeGraph(float** graph, int nbUsers);

//Given 2 ratings from a single user, tells if the 2 movies should be nearer or not and updates the graph accordingly
float updateWeight(int rating1, int rating2);

//Given a user and the graph, updates the distances knowing the ratings of the user (using the function updateWeight)
void updateGraphuser(user user, float** graph, int dateLimit);

//Given a user and an array of users, tells if the user is in the array
bool userinarray(user* users, int nbUsers, user user);

//iterates the previous function for all users. After calling this function, the graph is supposed to be finished
void updateGraph(float** graph, user* users, int nbUsers, user* toBeIgnoredUsers, int nbIgnoredUsers, int dateLimit, int id_privilegeduser1, int id_privilegeduser2, int min_ratings_to_be_considered_a_good_reviewer);





