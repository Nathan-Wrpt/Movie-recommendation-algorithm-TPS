#ifndef MAXA_H
#define MAXA_H

#include "../all.h"

//Counts the number of lines in a file
int countLines(const char* filename);

//Returns the max numbers of ratings a user has given
int getMaxAdvices(char* folderpath);

#endif // MAXA_H