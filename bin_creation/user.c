#include "user.h"

user* createUsersTable(char* folderpath){
    int i = 0;
    while(i<NBFILMS){
        char filename[100];
        sprintf(filename, "%s/mv_%07d.txt", folderpath, i);
        FILE* file = fopen(filename, "r");
        char tmp = fgetc(file);
        tmp = fgetc(file);
        tmp = fgetc(file);


        //to be coded
        fclose(file);
    }
}

// Function to serialize the user array into a binary file
void serializeUsers(user* users, int numUsers, const char* filename) {
    FILE* file = fopen(filename, "wb"); // Open the file in write mode (binary)
    if (file != NULL) {
        fwrite(&numUsers, sizeof(int), 1, file); // Write number of users first
        fwrite(users, sizeof(user), numUsers, file); // Write users data
        fclose(file); // Close the file
    } else {
        printf("Failed to open the file for writing.\n");
    }
}

// Function to deserialize the user array from a binary file
user* deserializeUsers(const char* filename, int* numUsers) {
    FILE* file = fopen(filename, "rb"); // Open the file in read mode (binary)
    user* users = NULL;

    if (file != NULL) {
        fread(numUsers, sizeof(int), 1, file); // Read number of users first
        users = (user*)malloc(sizeof(user) * (*numUsers)); // Allocate memory for users
        fread(users, sizeof(user), *numUsers, file); // Read users data
        fclose(file); // Close the file
    } else {
        printf("Failed to open the file for reading.\n");
    }

    return users;
}