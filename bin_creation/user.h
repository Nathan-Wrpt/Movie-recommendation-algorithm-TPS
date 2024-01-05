#ifndef USER_H
#define USER_H

#include "../all.h"
#include "../util/progressbar.h"

/**
 * @file user.h
 * @brief Header file for user.c.
 */

/**
 * @brief Creates an empty user structure with the provided ID.
 *
 * @param id The ID of the user.
 * @return A pointer to the initialized user structure.
 */
user* initUser(int id);

/**
 * @brief Given the movies table, creates the entire table of users
 * with all their ratings stored.
 *
 * @param moviesTable The movies table.
 * @return A pointer to the created table of users.
 */
user* createUsersTable(movie* moviesTable);

/**
 * @brief Serializes the user array into a binary file with the specified filename.
 *
 * @param users The users table to serialize.
 * @param numUsers The number of users in the table.
 * @param filename The filename for serialization.
 */
void serializeUsers(user* users, int numUsers, const char* filename);

/**
 * @brief Deserializes a binary file into an array of users and retrieves the number of users.
 *
 * @param filename The filename for deserialization.
 * @param[out] numUsers Pointer to store the number of users.
 * @return The deserialized array of users.
 */
user* deserializeUsers(const char* filename, int* numUsers);

/**
 * @brief Frees the memory allocated for the user table.
 *
 * @param users The users table to be freed.
 * @param numUsers The number of users in the table.
 */
void freeUsers(user* users, int numUsers);

#endif // USER_H
