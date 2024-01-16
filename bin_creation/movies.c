#include "movies.h"
#include "../all.h"


/**
 * @file movies.c
 * @brief Functions to parse the movie database and create the movie table.
 */

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
    int file_id, file_release_date;
    char file_title[300];
    int curline = 0;
    while (fgets(line, sizeof(line), file)) {
        curline++;
        if(curline == id){ //Because movie with id n is on line n
            sscanf(line, "%d,%d,%299[^\n]", &file_id, &file_release_date, file_title);
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

    //allocate memory for ratings array
    m->ratings = (rating*)malloc(sizeof(rating) * m->nb_ratings);
    if (m->ratings == NULL) {
        printf("Memory allocation failed for ratings\n");
        free(m);
        return NULL;
    }

    FILE* fileRating = fopen(filepath, "r");
    if (fileRating == NULL) {
        printf("Can't open file.\n");
        free(m->ratings);
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
        movie* m = initMovie(i+1, movietitlespath, trainingsetpath); // i+1 because movie ids start at 1
        movies[i] = *m;
        free(m);
        if(i % 5 == 0){
            updateProgressBar(i * 100 / NBMOVIES);
        }
    }

    return movies;
}

void serializeMovies(movie* movies, int numMovies, const char* filename) {
    FILE* file = fopen(filename, "wb");
    if (file != NULL) {
        fwrite(&numMovies, sizeof(int), 1, file); // Write number of movies first

        for (int i = 0; i < numMovies; ++i) {
            int titleLength = strlen(movies[i].title) + 1; // Include null terminator

            // Calculate buffer size needed for each movie
            size_t bufferSize = sizeof(int) * 4 + titleLength; // id, release_date, nb_ratings, titleLength, and title
            bufferSize += sizeof(rating) * movies[i].nb_ratings; // for each rating

            // Allocate buffer
            char* buffer = (char*)malloc(bufferSize);
            if (buffer == NULL) {
                printf("Memory allocation failed.\n");
                fclose(file);
                return;
            }

            // Copy movie data to buffer
            char* ptr = buffer;
            memcpy(ptr, &(movies[i].id), sizeof(int));
            ptr += sizeof(int);
            memcpy(ptr, &(movies[i].release_date), sizeof(int));
            ptr += sizeof(int);
            memcpy(ptr, &titleLength, sizeof(int));
            ptr += sizeof(int);
            memcpy(ptr, movies[i].title, titleLength); // Include null terminator
            ptr += titleLength;
            memcpy(ptr, &(movies[i].nb_ratings), sizeof(int));
            ptr += sizeof(int);

            // Copy ratings data to buffer
            if (movies[i].nb_ratings > 0 && movies[i].ratings != NULL) {
                memcpy(ptr, movies[i].ratings, sizeof(rating) * movies[i].nb_ratings);
            }

            // Write buffer to file
            fwrite(buffer, bufferSize, 1, file);

            // Free the buffer
            free(buffer);

            // Progress update
            if(i % 5 == 0) {
                updateProgressBar(i * 100 / numMovies);
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
        if (movies == NULL) {
            fclose(file);
            printf("Memory allocation failed.\n");
            return NULL;
        }

        for (int i = 0; i < numMovies; ++i) {
            fread(&(movies[i].id), sizeof(int), 1, file);
            fread(&(movies[i].release_date), sizeof(int), 1, file);

            // Read title length and allocate memory to read the title
            int titleLength;
            fread(&titleLength, sizeof(int), 1, file);
            char *buffer = (char*)malloc(titleLength + 1); // +1 for null terminator
            if (buffer == NULL) {
                fclose(file);
                for (int j = 0; j < i; ++j) {
                    free(movies[j].ratings);
                }
                free(movies);
                printf("Memory allocation failed for title.\n");
                return NULL;
            }

            fread(buffer, sizeof(char), titleLength, file);
            buffer[titleLength] = '\0'; // Null-terminate the string
            strncpy(movies[i].title, buffer, 300); // Copy to movie title
            free(buffer); // Free temporary buffer

            // Deserialize ratings
            fread(&(movies[i].nb_ratings), sizeof(int), 1, file);
            movies[i].ratings = (rating*)malloc(movies[i].nb_ratings * sizeof(rating));
            if (movies[i].ratings == NULL && movies[i].nb_ratings > 0) {
                fclose(file);
                for (int j = 0; j <= i; ++j) {
                    free(movies[j].ratings);
                }
                free(movies);
                printf("Memory allocation failed for ratings.\n");
                return NULL;
            }

            if (movies[i].nb_ratings > 0) {
                fread(movies[i].ratings, sizeof(rating), movies[i].nb_ratings, file);
            }

            if(i % 5 == 0){
                updateProgressBar(i * 100 / numMovies);
            }
        }

        fclose(file);
        return movies;
    } else {
        printf("Failed to open %s. It might not exist or could not be opened.\n", filename);
        exit(1);
    }
}

void freeMovies(movie* movies, int numMovies) {
    for (int i = 0; i < numMovies; i++) {
        free(movies[i].ratings);
    }
    free(movies);
}

float averageRating(int id, movie* movies){
    if(id < 1 || id > NBMOVIES){
        printf("Invalid movie id\n");
        return -1;
    }
    float sum = 0;
    for(int i = 0; i < movies[id - 1].nb_ratings; i++){
        sum += movies[id - 1].ratings[i].star;
    }
    return sum / movies[id - 1].nb_ratings;
}

void print_movie_stats(int id, movie* movies){
    if(id < 1 || id > NBMOVIES){
        printf("Invalid movie id\n");
        return;
    }

    
    printf("Information about movie ");
    printf("\033[1;37m");
    printf("%s", movies[id - 1].title);
    printf("\033[1;22m");
    printf(" (id : %d) :\n\n", id);

    printf("📅 Release date : %d\n", movies[id - 1].release_date);
    printf("⭐ Average rating : %f\n", averageRating(id, movies));
    printf("🔢 Number of ratings : %d\n", movies[id - 1].nb_ratings);
}