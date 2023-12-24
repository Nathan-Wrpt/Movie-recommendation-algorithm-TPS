#include "maxadvices.h"

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

//Function to return the max numbers of lines in all the files named mv_*.txt
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