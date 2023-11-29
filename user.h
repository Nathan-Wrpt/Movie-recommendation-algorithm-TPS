#define NBFILMS 17100

typedef struct rating_temp{
     int id_film;
     int year;
     int day;
     int month;
     int rating;
} rating;

typedef struct user_temp{
    int id;
    int nb_ratings;
    rating* ratings;
} user;

// Function to create a table of all the users having the path of the training_set folder
user* createUsersTable(char* folderpath);

// Function to free the table of users
void freeUsersTable(user* users);

// Function to save the table of users in a file
void saveUsersTable(user* users, char* folderpath);

// Function to load the table of users from a file
user* loadUsersTable(char* filepath);

