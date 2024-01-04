#ifndef MAXA_H
#define MAXA_H

#include "../all.h"

//Counts the number of lines in a file
int countLines(const char* filename);

//Returns the max numbers of ratings a user has given
int getMaxAdvices(char* folderpath);

//Returns the maximum number of ratings a user has given
int maxratings(user* users, int numUsers);

//Returns the average number of ratings the users gave
float averageRatings(user* users, int numUsers);

#endif // MAXA_H