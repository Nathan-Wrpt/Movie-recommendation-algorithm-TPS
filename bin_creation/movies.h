#ifndef MOVIES_H
#define MOVIES_H

#include "user.h"

#define NBMOVIES 17770

//This file contains the structure of a movie and the functions to initialize it
//Moreover, the functions to convert the full movie table into a binary file and to convert it back are in this file


// Structure of a movie (Not sure for the moment, prolly gon be modified)
typedef struct movie_temp{
    int id;
    int release_date;
    char title[300];
    int nb_ratings;
    rating* ratings;
} movie;

//Function to count the lines of a file (even tho it's not really a movie function and already created in util/maxadvices.c, but it's easier to put it here for the testing process)
int countLines(const char* filename);

//Given the id of a movie, the path of movie_titles.txt and training_set folder, returns a pointer to a movie structure containing the movie's information and ratings
//It's the most important part of this file.
movie* initMovie(int id, char* movietitlespath, char* trainingsetpath);


//Given the path of movie_titles.txt and training_set folder, it creates the full table of movies and returns a pointer to it
//It's the longest function of the file, takes around 1 minute 30 seconds to execute
movie* createMovieTable(char* movietitlespath, char* trainingsetpath);

// Function to serialize the movie array into a binary file which name is what we give to the function as filename(We didn't know how to do it, so we searched how to do it on the internet and adapted it to our case)
void serializeMovies(movie* movies, int numMovies, const char* filename);

// Function to deserialize the movie array from a binary file
movie* deserializeMovies(const char* filename);


//Function to free the memory allocated for a movie, iterating through the ratings and freeing them too
void freeMovies(movie* movies, int numMovies);

#endif