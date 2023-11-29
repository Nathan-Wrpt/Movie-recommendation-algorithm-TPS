#include "movies.h"

//Function to count the lines of a file
int countLines(const char* filename){
    FILE* file = fopen(filename, "r");
    int count = 0;

    if (file != NULL) {
        char buffer[1024]; // Buffer to store the line

        while (fgets(buffer, sizeof(buffer), file) != NULL) {
            count++; // Increment the counter for each line
        }

        fclose(file);
    } else {
        printf("Impossible d'ouvrir le fichier %s\n", filename);
    }

    return count;
}


movie* initMovie(int id, char* movietitlespath, char* trainingsetpath){
    if(id < 1 || id > NBMOVIES){
        printf("Invalid movie id\n");
        return NULL;
    }

    movie* m = (movie*)malloc(sizeof(movie));
    
    if (m == NULL) {
        printf("Memory allocation failed\n");
        return NULL;
    }

    FILE* file = fopen(movietitlespath, "r");
    if (file == NULL) {
        printf("Can't open file\n");
        free(m);
        return NULL;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        int file_id, file_release_date;
        char file_title[300];

        // Read the line and store the data into the variables
        sscanf(line, "%d,%d,%299[^\n]", &file_id, &file_release_date, file_title);

        if (file_id == id) {
            m->id = file_id;
            m->release_date = file_release_date;
            strncpy(m->title, file_title, sizeof(m->title) - 1);
            m->title[sizeof(m->title) - 1] = '\0';
            break;
        }
    }
    fclose(file);

    char filepath[100];
    sprintf(filepath, "%s/mv_%07d.txt", trainingsetpath, id);
    m->nb_ratings = countLines(filepath) - 1;

    //allocate memory for ratings tab
    m->ratings = (rating*)malloc(sizeof(rating) * m->nb_ratings);

    FILE* fileRating = fopen(filepath, "r");
    if (fileRating == NULL) {
        printf("Can't open file.\n");
        free(m);
        return NULL;
    }

    int i = 0;
    fgets(line, sizeof(line), fileRating); // Skip the first line
    while (fgets(line, sizeof(line), fileRating) && i < m->nb_ratings) {
        sscanf(line, "%d,%d,%d-%d-%d", 
            &m->ratings[i].id_user, 
            &m->ratings[i].star, 
            &m->ratings[i].year, 
            &m->ratings[i].month, 
            &m->ratings[i].day
        );

        m->ratings[i].id_film = id;
        i++;
    }
    fclose(fileRating);

    return m;
}

movie* createMovieTable(char* movietitlespath, char* trainingsetpath){
    movie* movies = (movie*)malloc(sizeof(movie) * NBMOVIES);

    if (movies == NULL) {
        printf("Memory allocation failed\n");
        return NULL;
    }

    for (int i = 0; i < NBMOVIES; i++) {
        movies[i] = *initMovie(i+1, movietitlespath, trainingsetpath); // i+1 because movie ids start at 1
    }

    return movies;
}

void serializeMovies(movie* movies, int numMovies, const char* filename) {
    FILE* file = fopen(filename, "wb");
    if (file != NULL) {
        fwrite(&numMovies, sizeof(int), 1, file); // Write number of movies first

        for (int i = 0; i < numMovies; ++i) {
            fwrite(&(movies[i].id), sizeof(int), 1, file);
            fwrite(&(movies[i].release_date), sizeof(int), 1, file);

            // Write title length and title content separately
            int titleLength = strlen(movies[i].title);
            fwrite(&titleLength, sizeof(int), 1, file);
            fwrite(movies[i].title, sizeof(char), titleLength, file);

            fwrite(&(movies[i].nb_ratings), sizeof(int), 1, file);

            // Serialize each rating within the movie
            for (int j = 0; j < movies[i].nb_ratings; ++j) {
                fwrite(&(movies[i].ratings[j].id_user), sizeof(int), 1, file);
                fwrite(&(movies[i].ratings[j].id_film), sizeof(int), 1, file);
                fwrite(&(movies[i].ratings[j].year), sizeof(int), 1, file);
                fwrite(&(movies[i].ratings[j].day), sizeof(int), 1, file);
                fwrite(&(movies[i].ratings[j].month), sizeof(int), 1, file);
                fwrite(&(movies[i].ratings[j].star), sizeof(int), 1, file);
            }
        }
        fclose(file);
    } else {
        printf("Failed to open the file for writing.\n");
    }
}

movie* deserializeMovies(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (file != NULL) {
        int numMovies;
        fread(&numMovies, sizeof(int), 1, file); // Read the number of movies

        // Allocate memory for movies
        movie* movies = (movie*)malloc(numMovies * sizeof(movie));

        for (int i = 0; i < numMovies; ++i) {
            fread(&(movies[i].id), sizeof(int), 1, file);
            fread(&(movies[i].release_date), sizeof(int), 1, file);

            // Read title length and allocate memory to read the title
            int titleLength;
            fread(&titleLength, sizeof(int), 1, file);
            fread(movies[i].title, sizeof(char), titleLength, file);
            movies[i].title[titleLength] = '\0'; // Null-terminate the string

            fread(&(movies[i].nb_ratings), sizeof(int), 1, file);

            // Allocate memory for ratings
            movies[i].ratings = (rating*)malloc(movies[i].nb_ratings * sizeof(rating));

            // Deserialize each rating within the movie
            for (int j = 0; j < movies[i].nb_ratings; ++j) {
                fread(&(movies[i].ratings[j].id_user), sizeof(int), 1, file);
                fread(&(movies[i].ratings[j].id_film), sizeof(int), 1, file);
                fread(&(movies[i].ratings[j].year), sizeof(int), 1, file);
                fread(&(movies[i].ratings[j].day), sizeof(int), 1, file);
                fread(&(movies[i].ratings[j].month), sizeof(int), 1, file);
                fread(&(movies[i].ratings[j].star), sizeof(int), 1, file);
            }
        }

        fclose(file);
        return movies;
    } else {
        printf("Failed to open the file for reading.\n");
        return NULL;
    }
}

void freeMovies(movie* movies, int numMovies) {
    for (int i = 0; i < numMovies; i++) {
        free(movies[i].ratings);
    }
    free(movies);
}

// int main(){
//     //Tests to see if the creation of the movie table works properly
//     // movie* movies = createMovieTable("../../nf_prize_dataset/download/movie_titles.txt", "../../nf_prize_dataset/download/training_set");
//     // printf("Movie 17770 id: %d\n", movies[17769].id);
//     // printf("Movie 17770 release date: %d\n", movies[17769].release_date);
//     // printf("Movie 17770 title: %s\n", movies[17769].title);
//     // printf("Movie 17770 nb_ratings: %d\n", movies[17769].nb_ratings);
//     // printf("Movie 17770 ratings: \n");
//     // for (int j = 0; j < movies[17769].nb_ratings; j++) {
//     //     printf("Rating %d id_user: %d\n", j, movies[17769].ratings[j].id_user);
//     //     printf("Rating %d id_film: %d\n", j, movies[17769].ratings[j].id_film);
//     //     printf("Rating %d year: %d\n", j, movies[17769].ratings[j].year);
//     //     printf("Rating %d month: %d\n", j, movies[17769].ratings[j].month);
//     //     printf("Rating %d day: %d\n", j, movies[17769].ratings[j].day);
//     //     printf("Rating %d rating: %d\n", j, movies[17769].ratings[j].star);
//     // }
//     // freeMovies(movies, NBMOVIES);

//     //Tests to see if the serialization works properly
//     // movie* movies = createMovieTable("../../nf_prize_dataset/download/movie_titles.txt", "../../nf_prize_dataset/download/training_set");
//     // serializeMovies(movies, NBMOVIES, "movies.bin");
//     // freeMovies(movies, NBMOVIES);

//     //
//     //Tests to see if the deserialization works properly
//     //
//     // movie* movies = deserializeMovies("movies.bin");
//     // printf("Movie 17770 id: %d\n", movies[17769].id);
//     // printf("Movie 17770 release date: %d\n", movies[17769].release_date);
//     // printf("Movie 17770 title: %s\n", movies[17769].title);
//     // printf("Movie 17770 nb_ratings: %d\n", movies[17769].nb_ratings);
//     // printf("Movie 17770 ratings: \n");
//     // for (int j = 0; j < movies[17769].nb_ratings; j++) {
//     //     printf("Rating %d id_user: %d\n", j, movies[17769].ratings[j].id_user);
//     //     printf("Rating %d id_film: %d\n", j, movies[17769].ratings[j].id_film);
//     //     printf("Rating %d year: %d\n", j, movies[17769].ratings[j].year);
//     //     printf("Rating %d month: %d\n", j, movies[17769].ratings[j].month);
//     //     printf("Rating %d day: %d\n", j, movies[17769].ratings[j].day);
//     //     printf("Rating %d rating: %d\n", j, movies[17769].ratings[j].star);
//     // }
//     return 0;
// }
