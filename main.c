#include "all.h"
#include "bin_creation/user.h"
#include "bin_creation/movies.h"
#include "algo/graphcreation.h"
#include "util/maxadvices.h"

void print_usage(){
    printf("Usage: ./main -r <MovieYouLikeid1,MovieYouLikeid2,...>(or the path of a .txt) -n <numberOfMoviesYouWannaGetRecommended-f <folderpath> -l <num> -s <film_id> -c <client1,client2...> -b <bad_reviewer1,bad_reviewer2,...> -e <minmoviesreviewed> -t\n");
    printf("Note: All options are optional\n");
    printf("Options:\n");
    printf("-r <MovieYouLikeid1,MovieYouLikeid2,...>: Provide a list of movies you like (separated by commas) and get a list of movies you might like\n");
    printf("-n <numberOfMoviesYouWannaGetRecommended>: Specify the number of movies you want to get recommended (by default 10)\n");
    printf("-f <.txt path and filename (example : /home/data.txt)>: Path to the txt where the results will be saved if you want them to\n");
    printf("-l <num>: Ignore ratings whose date is greater than <num>\n");
    printf("-s <film_id>: Provide statistics on the film with identifier <film_id> (number of ratings, average rating, etc.)\n");
    printf("-c <client1,client2...>: Consider only ratings from clients <client1> and <client2>\n");
    printf("-b <bad_reviewer1,bad_reviewer2,...>: Exclude ratings from reviewers <bad_reviewer1>, <bad_reviewer2>, etc.\n");
    printf("-e <minmoviesreviewed>: Consider only elite clients who have watched a minimum of <minmoviesreviewed> movies\n"); 
    printf("-t: Specify the execution time of the algorithm\n");
}

//fonction qui à partir d'un chemin vers un fichier txt, écrit le contenu de string dans ce fichier
void write_txt(char* string, char* path){
    FILE* file = fopen(path, "w");
    if(file == NULL){
        printf("Error opening file\n");
        exit(1);
    }
    fprintf(file, "%s", string);
    fclose(file);
}

//Compte le nombre de films likés après l'option -r, séparés par des ','
int num_movies_liked(char* moviesLiked){
    char* copy = malloc(strlen(moviesLiked) + 1);
    strcpy(copy, moviesLiked);
    int num = 0;
    char* token = strtok(copy, ",");
    while(token != NULL){
        num++;
        token = strtok(NULL, ",");
    }
    free(copy);
    return num;
}

//Parse les films likés après l'option -r et les stocke dans le tableau pris en paramètre (moviesLikedParsed)
void movies_liked_parsing(char* moviesLiked, int** moviesLikedParsed, int numMoviesLiked){
    char* copy = malloc(strlen(moviesLiked) + 1);
    strcpy(copy, moviesLiked);
    char* token = strtok(copy, ",");
    int i = 0;
    while(token != NULL && i < numMoviesLiked){
        (*moviesLikedParsed)[i] = atoi(token);
        token = strtok(NULL, ",");
        i++;
    }
    free(copy);
}

int main(int argc, char* argv[]){

    //---------------------------------------------------------------ARGUMENTS PARSING---------------------------------------------------------------
    extern char* optarg;
    int opt;

    int minmoviesreviewed = 0;
    int numMoviesLiked = 0;
    char* moviesLiked = NULL;
    int* moviesLikedParsed = NULL;
    int numMoviesRecommended = 10;
    char* folderpath = NULL;
    int dateLimit = 2020;
    int film_id = -1;
    char* clients = NULL;
    int numClients = 0;
    int* clientsParsed = NULL;
    char* bad_reviewers = NULL;
    int numBadReviewers = 0;
    int* badReviewersParsed = NULL;
    bool toption = false;

    while ((opt = getopt(argc, argv, "r:n:f:l:s:c:b:e:th")) != -1) {
        switch (opt) {
            case 'h':
                print_usage();
                exit(0);
                break;
            case 'r':
                moviesLiked = optarg;
                numMoviesLiked = num_movies_liked(moviesLiked);
                moviesLikedParsed = malloc(numMoviesLiked * sizeof(int));
                movies_liked_parsing(moviesLiked, &moviesLikedParsed, numMoviesLiked);
                break;
            case 'n':
                numMoviesRecommended = atoi(optarg);
                break;
            case 'f':
                folderpath = optarg;
                break;
            case 'l':
                dateLimit = atoi(optarg);
                break;
            case 's':
                film_id = atoi(optarg);
                break;
            case 'c':
                clients = optarg;
                numClients = num_movies_liked(clients);
                clientsParsed = malloc(numClients * sizeof(int));
                movies_liked_parsing(clients, &clientsParsed, numClients);
                break;
            case 'b':
                bad_reviewers = optarg;
                numBadReviewers = num_movies_liked(bad_reviewers);
                badReviewersParsed = malloc(numBadReviewers * sizeof(int));
                movies_liked_parsing(bad_reviewers, &badReviewersParsed, numBadReviewers);
                break;
            case 'e':
                minmoviesreviewed = atoi(optarg);
                break;
            case 't':
                toption = true;
                break;
            default:
                fprintf(stderr, "Usage: %s -r <MovieYouLikeid1,MovieYouLikeid2,...>(or the path of a .txt) -n <numberOfMoviesYouWannaGetRecommended-f <folderpath> -l <num> -s <film_id> -c <client1,client2...> -b <bad_reviewer1,bad_reviewer2,...> -e <minmoviesreviewed> -t\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }
    //---------------------------------------------------------------END OF ARGUMENTS PARSING---------------------------------------------------------------

    //Matrix representing the function to update weights between 2 movies based on the stars a same user gave to both movies
    float weights[5][5] = {
        {-0.25, -0.17,  0.0,   0.46,  1.0},
        {-0.17, -0.62, -0.46,  0.0,   0.5},
        { 0.0,  -0.46, -1.0,  -0.5,   0.0},
        { 0.46,  0.0,  -0.5,   1.0,  -0.5},
        { 1.0,   0.5,   0.0,  -0.5,  -1.0}
    };
    clock_t begin = clock();
    //Deserialization of the users and movies
    printf("Deserializing movies...\n");
    movie* movies = deserializeMovies("bin_creation/movies.bin");

    //If the user wants to get statistics about a specific movie
    if(film_id != -1){
        print_movie_stats(film_id, movies);
        exit(0);
    }

    int nbUsers;
    printf("Deserializing users...\n");
    user* users = deserializeUsers("bin_creation/users.bin", &nbUsers);

    //Creation of the graph
    printf("Creating the graph...\n");
    float** graph = initGraph(NBMOVIES);
    printf("Updating the graph...\n");

    updateGraph(graph, users, NBUSERS, badReviewersParsed, numBadReviewers, clientsParsed, numClients, minmoviesreviewed, dateLimit, weights);
    printf("Graph created and updated\n");

    //Determining the movies to recommend
    printf("Determining the movies to recommend...\n");
    printf("Movies liked parsed: ");
    for(int i = 0; i < numMoviesLiked; i++){
        printf("%d ", moviesLikedParsed[i]);
    }
    printf("\n");

    

    printf("Number of movies liked: %d\n", numMoviesLiked);
    printf("Number of movies recommended: %d\n", numMoviesRecommended);
    int* recommendedMovies = getNClosestMovies(moviesLikedParsed, numMoviesLiked, graph, numMoviesRecommended);
    printf("Recommended movies: ");
    for(int i = 0; i < numMoviesRecommended; i++){
        printf("%d ", recommendedMovies[i]);
    }
    
    clock_t end = clock();

    //Calculate the execution time of the program
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    if(toption){
        printf("Execution time of the program was : %f secondes.\n", time_spent);
    }

    printf("\n\n ======================= RESULTS ======================= \n\n");

    //Print the movies recommended
    printf("Based on the movies you liked (");
    for(int i = 0; i < numMoviesLiked; i++){
        printf("%s", movies[moviesLikedParsed[i] - 1].title);
        if(i != numMoviesLiked - 1){
            printf(", ");
        }
    }
    printf("), we recommend you the following movies:\n");
    for(int i = 0; i < numMoviesRecommended; i++){
        printf("-%s\n", movies[recommendedMovies[i] - 1].title);
    }

    if(folderpath != NULL){
        //if folderpath isnt an existing txt file, we create it
        if(strstr(folderpath, ".txt") == NULL){
            char* path = malloc(strlen(folderpath) + 5);
            strcpy(path, folderpath);
            strcat(path, ".txt");
            folderpath = path;
        }
        //we write the results in the file
        FILE* f = fopen(folderpath, "w");
        fprintf(f, "Based on the movies you liked (");
        for(int i = 0; i < numMoviesLiked; i++){
            fprintf(f, "%s", movies[moviesLikedParsed[i] - 1].title);
            if(i != numMoviesLiked - 1){
                fprintf(f, ", ");
            }
        }
        fprintf(f, "), we recommend you the following movies:\n");
        for(int i = 0; i < numMoviesRecommended; i++){
            fprintf(f, "-%s\n", movies[recommendedMovies[i] - 1].title);
        }
        fclose(f);
    }
    freeGraph(graph, NBMOVIES);
    free(recommendedMovies);
    free(moviesLikedParsed);
    free(clientsParsed);
    free(badReviewersParsed);
    freeUsers(users, NBUSERS);
    freeMovies(movies, NBMOVIES);
    return 0;

}

