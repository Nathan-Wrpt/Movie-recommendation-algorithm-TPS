#include "getmovietitle.h"

/**
 * @file getmovietitle.c
 * @brief Function to retrieve movie titles by ID.
 */

char* getMovieTitle(int id, char* movietitlespath){
    if(id < 1 || id > NBMOVIES){
        printf("Invalid movie id\n");
        return NULL;
    }

    FILE* file = fopen(movietitlespath, "r");
    
    if (file == NULL) {
        printf("Can't open file\n");
        return NULL;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        int file_id;
        char file_title[300];
        int date;

        // Read the line and store the data into the variables
        sscanf(line, "%d, %d, %299[^\n]", &file_id, &date, file_title);
        if (file_id == id) { // If the ID matches, return the title
            return strdup(file_title);
        }
    }
    fclose(file);
    return NULL;
}