#ifndef ALL_H
#define ALL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>

#define NBUSERS 480189
#define IDMAXUSER 2649429
#define NBMOVIES 17770
#define INIT_WEIGHT 0
#define RATINGS_CONSIDERED 30



typedef struct rating_temp{
     int id_user;
     int id_film;
     int year;
     int day;
     int month;
     int star;
} rating;


// Structure of a movie
typedef struct movie_temp{
    int id;
    int release_date;
    char title[300];
    int nb_ratings;
    rating* ratings;
} movie;


typedef struct user_temp{
    int id;
    int nb_ratings;
    rating* ratings;
} user;

#endif // ALL_H

