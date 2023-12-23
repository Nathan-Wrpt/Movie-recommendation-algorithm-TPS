#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include "graphcreation.c"

void print_usage(){
    printf("Usage: ./main -r <MovieYouLikeid1,MovieYouLikeid2,...>(or the path of a .txt) -n <numberOfMoviesYouWannaGetRecommended-f <folderpath> -l <num> -s <film_id> -c <client1,client2...> -b <bad_reviewer1,bad_reviewer2,...> -e <minmoviesreviewed> -t\n");
    printf("Note: All options are optional except -r\n");
    printf("Options:\n");
    printf("-r <MovieYouLikeid1,MovieYouLikeid2,...>: Provide a list of movies you like (separated by commas) and get a list of movies you might like\n");
    printf("-n <numberOfMoviesYouWannaGetRecommended>: Specify the number of movies you want to get recommended (by default 10)\n");
    printf("-f <folderpath>: Path to the folder where the files corresponding to the requested results will be saved (by default in the current directory)\n");
    printf("-l <num>: Ignore ratings whose date is greater than <num>\n");
    printf("-s <film_id>: Provide statistics on the film with identifier <film_id> (number of ratings, average rating, etc.)\n");
    printf("-c <client1,client2...>: Consider only ratings from clients <client1> and <client2>\n");
    printf("-b <bad_reviewer1,bad_reviewer2,...>: Exclude ratings from reviewers <bad_reviewer1>, <bad_reviewer2>, etc.\n");
    printf("-e <minmoviesreviewed>: Consider only elite clients who have watched a minimum of <minmoviesreviewed> movies\n"); 
    printf("-t: Specify the execution time of the algorithm\n");
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
    if(argc < 3){
        print_usage();
        exit(0);
    }

    //---------------------------------------------------------------ARGUMENTS PARSING---------------------------------------------------------------
    extern char* optarg;
    int opt;

    int minmoviesreviewed = 0;
    int numMoviesLiked = 0;
    char* moviesLiked = NULL;
    int* moviesLikedParsed = NULL;
    int numMoviesRecommended = 10;
    char* folderpath = NULL;
    int dateLimit = 2023;
    int film_id = -1;
    char* clients = NULL;
    int numClients = 0;
    int* clientsParsed = NULL;
    char* bad_reviewers = NULL;
    int numBadReviewers = 0;
    int* badReviewersParsed = NULL;
    bool toption = false;

    while ((opt = getopt(argc, argv, "r:n:f:l:s:c:b:e:t:h")) != -1) {
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

    // printf("(Option -r) Movies Liked: %s\n", moviesLiked);
    // printf("Number of Movies Liked: %d\n", numMoviesLiked);
    // printf("Movies Liked Parsed: ");
    // for(int i = 0; i < numMoviesLiked; i++){
    //     printf("%d ", moviesLikedParsed[i]);
    // }
    // printf("\n");
    // printf("(Option -n) : \nNumber of Movies Recommended: %d\n", numMoviesRecommended);
    // printf("(option -f) : \nFolder Path: %s\n", folderpath);
    // printf("(option -l) : \nDate limite : %d\n", dateLimit);
    // printf("(option -s) : \n Film ID: %d\n", film_id);
    // printf("(option -c) : \n Clients: %s\n", clients);
    // printf("Number of Clients: %d\n", numClients);
    // printf("Clients Parsed: ");
    // for(int i = 0; i < numClients; i++){
    //     printf("%d ", clientsParsed[i]);
    // }
    // printf("(option -e) : \nMin Movies Reviewed: %d\n", minmoviesreviewed);
    // printf("(option -b) : \nBad Reviewers: %s\n", bad_reviewers);
    // printf("Number of Bad Reviewers: %d\n", numBadReviewers);
    // printf("Bad Reviewers Parsed: ");
    // for(int i = 0; i < numBadReviewers; i++){
    //     printf("%d ", badReviewersParsed[i]);
    // }
    // printf("\n");
    // printf("Toption: %d\n", toption);
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
    int nbUsers;
    user* users = deserializeUsers("../bin_creation/users.bin", &nbUsers);
    clock_t timer1 = clock();
    double time_spent1 = (double)(timer1 - begin) / CLOCKS_PER_SEC;
    movie* movies = deserializeMovies("../bin_creation/movies.bin");
    clock_t timer3 = clock();
    //Creation of the graph
    float** graph = initGraph(NBMOVIES);
    clock_t timer4 = clock();
    updateGraph(graph, users, NBUSERS - 5, badReviewersParsed, numBadReviewers, clientsParsed, numClients, minmoviesreviewed, dateLimit, weights);
    clock_t timer2 = clock();
    //Determining the movies to recommend
    int* recommendedmMovies1 = getNClosestMovies(moviesLikedParsed, numMoviesLiked, graph, numMoviesRecommended);
    int* recommendedMovies2 = getNClosestMovies2(moviesLikedParsed, numMoviesLiked, graph, numMoviesRecommended);
    clock_t end = clock();

    //Calculate the execution time of the program
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    double time_spent2 = (double)(timer2 - timer4) / CLOCKS_PER_SEC;
    double time_spent3 = (double)(timer3 - timer1) / CLOCKS_PER_SEC;
    double time_spent4 = (double)(timer4 - timer3) / CLOCKS_PER_SEC;
    if(toption){
        printf("Total execution time of the program was : %f seconds.\n", time_spent);
        printf("Time passed in the users deserialization process : %f seconds.\n", time_spent1);
        printf("Time passed in the movies deserialization process : %f seconds.\n", time_spent3);
        printf("Time passed in the graph creation process : %f seconds.\n", time_spent4);
        printf("Time passed in the graph update process : %f seconds.\n\n", time_spent2);
    }

    //Print the movies recommended
    printf("Based on the movies you liked (");
    for(int i = 0; i < numMoviesLiked; i++){
        printf("%s", movies[moviesLikedParsed[i] - 1].title);
        if(i != numMoviesLiked - 1){
            printf(", ");
        }
    }
    printf("), \n\nErwan recommends you the following movies:\n");
    for(int i = 0; i < numMoviesRecommended; i++){
        printf("%d-%s\n", i+1, movies[recommendedMovies2[i] - 1].title);
    }
    printf("\nAnd Nathan recommends you the following movies:\n");
    for(int i = 0; i < numMoviesRecommended; i++){
        printf("%d-%s\n", i+1, movies[recommendedmMovies1[i] - 1].title);
    }
    freeGraph(graph, NBMOVIES);
    free(recommendedmMovies1);
    free(recommendedMovies2);
    free(moviesLikedParsed);
    free(clientsParsed);
    free(badReviewersParsed);
    freeUsers(users, NBUSERS);
    freeMovies(movies, NBMOVIES);
    free(folderpath);
    free(moviesLiked);
    free(clients);
    free(bad_reviewers);
    return 0;

}

