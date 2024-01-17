#ifndef MAXA_H
#define MAXA_H

#include "../all.h"

/**
 * @file maxadvices.h
 * @brief Header file for maxadvices.c.
 */

/**
 * @brief Counts and returns the number of lines in the specified file.
 *
 * @param filename The path to the file.
 * @return The number of lines in the file.
 */
int countLines(const char* filename);

/**
 * @brief Get the maximum number of ratings a user has given.
 *
 * @param folderpath The path to the training_set.
 * @return The maximum number of ratings a user has given.
 */
int getMaxAdvices(char* folderpath);

/**
 * @brief Same as getMaxAdvices, but using the users table
 *
 * @param users The users table.
 * @param numUsers The number of users in the table.
 * @return The maximum number of ratings given by users.
 */
int maxratings(user* users, int numUsers);

/**
 * @brief Get the average number of ratings given by users.
 *
 * @param users The table of users.
 * @param numUsers The number of users in the table.
 * @return The average number of ratings given by users.
 */
float averageRatings(user* users, int numUsers);


/**
 * @brief Get the size of a file.
 *
 * @param filename The path to the file.
 * @return The size of the file.
 */
long filesize(const char* filename);

#endif // MAXA_H
