#include "maxadvices.h"

/**
 * @file maxadvices.c
 * @brief Functions to count the number of lines in a file and to get the maximum and average number of ratings given by users.
 */

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

int getMaxAdvices(char* folderpath){
    int max = 0;
    int i = 1;
    while(i<NBMOVIES + 1){
        char filename[100];
        sprintf(filename, "%s/mv_%07d.txt", folderpath, i);
        int tmp = countLines(filename);
        if(tmp>max){
            max = tmp;
        }
        i++;
    }
    return (max - 2); //The first line is not a rating
}

int maxratings(user* users, int numUsers) {
    int max = 0;
    for (int i = 0; i < numUsers; i++) {
        if (users[i].nb_ratings > max) {
            max = users[i].nb_ratings;
        }
    }
    return max;
}

float averageRatings(user* users, int numUsers) {
    int sum = 0;
    for (int i = 0; i < numUsers; i++) {
        sum += users[i].nb_ratings;
    }
    return (float)sum / numUsers;
}