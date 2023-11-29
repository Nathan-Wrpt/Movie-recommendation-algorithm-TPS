#ifndef MOVIES_H
#define MOVIES_H

#include "user.h"

#define NBMOVIES 17770

// Structure of a movie (Not sure for the moment, prolly gon be modified)
typedef struct movie_temp{
    int id;
    int release_date;
    char title[300];
    int nb_ratings;
    rating* ratings;
} movie;

#endif