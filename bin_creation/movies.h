#ifndef MOVIES_H
#define MOVIES_H

#include "../all.h"
#include "../util/maxadvices.h"
#include "../util/progressbar.h"

/**
 * @file movies.h
 * @brief Header file for movies.c.
 */

/**
 * @brief Given the ID of a movie, the path of movie_titles.txt, and the training_set folder,
 * returns a pointer to a movie structure containing the movie's information and ratings.
 *
 * @param id The ID of the movie.
 * @param movietitlespath The path of movie_titles.txt.
 * @param trainingsetpath The path of the training_set folder.
 * @return A pointer to the initialized movie structure.
 */
movie* initMovie(int id, char* movietitlespath, char* trainingsetpath);

/**
 * @brief Given the path of movie_titles.txt and the training_set folder, creates the full table of movies thanks to initMovie
 * and returns a pointer to it. This function may take around 1 minute 30 seconds to execute.
 *
 * @param movietitlespath The path of movie_titles.txt.
 * @param trainingsetpath The path of the training_set folder.
 * @return A pointer to the created full table of movies.
 */
movie* createMovieTable(char* movietitlespath, char* trainingsetpath);

/**
 * @brief Serializes the movie array into a binary file in the given filename.
 *
 * @param movies The array of movies to serialize.
 * @param numMovies The number of movies in the array.
 * @param filename The filename for the output binary file.
 */
void serializeMovies(movie* movies, int numMovies, const char* filename);

/**
 * @brief Deserializes the movie array from a binary file using the given filename.
 *
 * @param filename The filename of the input binary file.
 * @return A pointer to the deserialized movie array.
 */
movie* deserializeMovies(const char* filename);

/**
 * @brief Frees the memory allocated for a movie array, including its ratings.
 *
 * @param movies The array of movies to free.
 * @param numMovies The number of movies in the array.
 */
void freeMovies(movie* movies, int numMovies);

/**
 * @brief Get the average rating of a movie by its ID.
 *
 * @param id The ID of the movie.
 * @param movies The movie table.
 * @return The average rating of the movie.
 */
float averageRating(int id, movie* movies);

/**
 * @brief Print the statistics of a movie by its ID.
 *
 * @param id The ID of the movie.
 * @param movies The movie table.
 */
void print_movie_stats(int id, movie* movies);

#endif /* MOVIES_H */
