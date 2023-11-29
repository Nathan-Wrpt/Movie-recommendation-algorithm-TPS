#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct rating_temp{
     int id_film;
     int date;
     int rating;
} rating;

typedef struct user_temp{
    int id;
    int nb_ratings;
    rating* ratings;
} user;


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

int main() {
    // Création de données de test (simulation de plusieurs utilisateurs avec des évaluations)
    user users[3];

    // Simulation des évaluations pour le premier utilisateur
    rating ratings_user1[2];
    ratings_user1[0].id_film = 1;
    ratings_user1[0].date = 20230101;
    ratings_user1[0].rating = 4;

    ratings_user1[1].id_film = 2;
    ratings_user1[1].date = 20230102;
    ratings_user1[1].rating = 5;

    users[0].id = 1;
    users[0].nb_ratings = 2;
    users[0].ratings = ratings_user1;

    // Simulation des évaluations pour le deuxième utilisateur
    rating ratings_user2[3];
    ratings_user2[0].id_film = 3;
    ratings_user2[0].date = 20230103;
    ratings_user2[0].rating = 3;

    ratings_user2[1].id_film = 4;
    ratings_user2[1].date = 20230104;
    ratings_user2[1].rating = 2;

    ratings_user2[2].id_film = 5;
    ratings_user2[2].date = 20230105;
    ratings_user2[2].rating = 1;

    users[1].id = 2;
    users[1].nb_ratings = 3;
    users[1].ratings = ratings_user2;

    // Simulation des évaluations pour le troisième utilisateur
    rating ratings_user3[1];
    ratings_user3[0].id_film = 6;
    ratings_user3[0].date = 20230106;
    ratings_user3[0].rating = 5;

    users[2].id = 3;
    users[2].nb_ratings = 1;
    users[2].ratings = ratings_user3;

    // Sérialisation des données d'utilisateur dans un fichier binaire
    serializeUsers(users, 3, "users_data.bin");

    // Désérialisation des données d'utilisateur depuis le fichier binaire
    int numUsers;
    user* deserializedUsers = deserializeUsers("users_data.bin", &numUsers);

    // Utilisation des données désérialisées (affichage pour cet exemple)
    for (int i = 0; i < numUsers; ++i) {
        printf("User ID: %d\n", deserializedUsers[i].id);
        printf("Number of ratings: %d\n", deserializedUsers[i].nb_ratings);
        printf("Ratings:\n");
        for (int j = 0; j < deserializedUsers[i].nb_ratings; ++j) {
            printf("  Film ID: %d, Date: %d, Rating: %d\n",
                   deserializedUsers[i].ratings[j].id_film,
                   deserializedUsers[i].ratings[j].date,
                   deserializedUsers[i].ratings[j].rating);
        }
    }

    // Nettoyage : libération de la mémoire allouée pour les données désérialisées
    for (int i = 0; i < numUsers; ++i) {
        free(deserializedUsers[i].ratings);
    }
    free(deserializedUsers);

    return 0;
}
