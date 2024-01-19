#ifndef GETMT_H
#define GETMT_H

#include "../globalVarAndStructures.h"

/**
 * @file getmovietitle.h
 * @brief Header file for getmovietitle.c.
 */

/**
 * @brief Retrieves the title of a movie by its ID directly from the movie database.
 *
 * @param id The ID of the movie.
 * @param movietitlespath The path to the movie_titles.txt file.
 * @return The title of the movie.
 */
char* getMovieTitle(int id, char* movietitlespath);

#endif // GETMT_H
