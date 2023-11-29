#define NBUSER 480189
#define NBMOVIES 17770

// Structure of a movie (Not sure for the moment, prolly gon be modified)
typedef struct movie_temp{
    int id;
    int release_date;
    char title[100];
} movie;



movie* initMovie(int id, char* movietitlespath){
    FILE* file = fopen(movietitlespath, "r");
    movie* m = (movie*)malloc(sizeof(movie));
    m->id = 0;
    m->release_date = 0;
    char title[100] = "";

    return m;
}