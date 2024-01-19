#ifndef GRAPHCREATION_H
#define GRAPHCREATION_H

#include "../globalVarAndStructures.h"
#include "../util/progressbar.h"

/**
 * @file graphcreation.h
 * @brief Header file for graphcreation.c.
 */

/**
 * @brief Initializes a graph as an adjacency matrix of size nbMovies * nbMovies.
 *
 * @param nbMovies The number of movies.
 * @return A pointer to the initialized graph.
 */
float** initGraph(int nbMovies);

/**
 * @brief Frees the memory allocated for the graph.
 *
 * @param graph The graph to be freed.
 * @param nbMovies The number of movies.
 */
void freeGraph(float** graph, int nbMovies);

/**
 * @brief Update the weight between two movies based on 2 ratings from the same user.
 *
 * @param rating1 The rating of the first movie.
 * @param rating2 The rating of the second movie.
 * @param weights The weights matrix.
 * @return The adjusted weight between the two movies.
 */
float updateWeight(int rating1, int rating2, float weights[5][5]);

/**
 * @brief Update the graph for every rating of a user. (Takes in consideration the limit date if -l is specified)
 *
 * @param user The user information.
 * @param graph The graph to be updated.
 * @param limitDate The limit date for ratings.
 * @param weights The weights matrix.
 * @param ratingsConsidered The number of ratings considered.
 */
void updateGraphUser(user user, float** graph, int limitDate, float weights[5][5], int ratingsConsidered);

/**
 * @brief Checks if a user is considered a bad reviewer based on an array of ignored users.
 *
 * @param ignoredUsers The array of ignored user IDs.
 * @param nbIgnoredUsers The number of ignored users. (The size of the array)
 * @param userId The ID of the user to check.
 * @return `true` if the user is ignored, otherwise `false`.
 */
bool isUserIgnored(int* ignoredUsers, int nbIgnoredUsers, int userId);

/**
 * @brief Find the index of a user in the array of users based on the user ID.
 *
 * @param users The array of users.
 * @param nbUsers The number of users.
 * @param idUser The ID of the user to find.
 * @return The index of the user in the array.
 */
int findIdUser(user* users, int nbUsers, int idUser);

/**
 * @brief Updates the graph for all the users by iterating the updateGraphUser function.
 *
 * @param graph The graph to be updated.
 * @param users The users table.
 * @param nbUsers The number of users.
 * @param ignoredUsers The array of ignored user IDs.
 * @param nbIgnoredUsers The number of ignored users. (The size of ignoredUsers)
 * @param privilegedUser The array of privileged users.
 * @param nbPrivilegedUser The number of privileged users. (The size of privilegedUser)
 * @param minRatings The minimum number of ratings.
 * @param limitDate The limit date for ratings.
 * @param weights The weights matrix.
 * @param ratingsConsidered The number of ratings considered.
 */
void updateGraph(float** graph, user* users, int nbUsers, int* ignoredUsers, int nbIgnoredUsers, int* privilegedUser, int nbPrivilegedUser, int minRatings, int limitDate, float weights[5][5], int ratingsConsidered);

/**
 * @brief Get the n IDs of recommended movies based on liked movies.
 *
 * @param moviesID The array of movie IDs liked by the user.
 * @param numFilmsID The number of movie IDs liked.
 * @param graph The graph representing movie relations.
 * @param n The number of recommended movies to retrieve.
 * @return The array of recommended movie IDs.
 */
int* getNClosestMovies(int* moviesID, int numFilmsID, float** graph, int n);

/**
 * @brief Similar to getNClosestMovies but ponderates movies based on their popularity to avoid excessive influence of flagship movies.
 *
 * @param moviesIDs The array of movie IDs liked by the user.
 * @param numFilmsID The number of movie IDs liked.
 * @param graph The graph representing movie relations.
 * @param n The number of recommended movies to retrieve.
 * @return The array of recommended movie IDs.
 */
int* getNClosestMovies2(int* moviesIDs, int numFilmsID, float** graph, int n);

/**
 * @brief Serializes the graph into a binary file at the specified path.
 *
 * @param graph The graph to be serialized.
 * @param path The file path for serialization.
 */
void serializegraph(float** graph, char* path);

/**
 * @brief Deserialize a binary file into a graph.
 *
 * @param path The file path for deserialization.
 * @return The deserialized graph.
 */
float** deserializegraph(char* path);

/**
 * @brief Frees the graph if he has been malloc'd as a whole block (done only when deserializing graph to save time).
 *
 * @param graph The graph to be freed.
 */
void freeGraphBlock(float** graph);

#endif // GRAPHCREATION_H