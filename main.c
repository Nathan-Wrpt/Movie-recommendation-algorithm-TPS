#include "all.h"
#include "bin_creation/user.h"
#include "bin_creation/movies.h"
#include "algo/graphcreation.h"
#include "util/maxadvices.h"
#include "util/progressbar.h"
#include "util/getmovietitle.h"

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
    printf("-o : Creates or create again all the .bin files (movies.bin, users.bin, graph.bin)\n");
    printf("-a <1 or 2>: Choose the algorithm you want to use (1 or 2)\n");
    printf("-z <num>: Specify the number of ratings considered for each user (by default 30)\n");
}

//fonction qui à partir d'un chemin vers un fichier txt, écrit le contenu de string dans ce fichier
void write_txt(char* string, char* path){
    FILE* file = fopen(path, "w");
    if(file == NULL){
        printf("Error opening output file\n");
        exit(1);
    }
    fprintf(file, "%s", string);
    fclose(file);
}

int* movieslikedfromtxt(char* path){
    FILE* file = fopen(path, "r");
    if(file == NULL){
        printf("Error opening movies liked file\n");
        exit(1);
    }
    int* moviesLikedParsed = malloc(10000 * sizeof(int));
    int i = 0;
    char* line = NULL;
    size_t len = 0;
    ssize_t read;
    while((read = getline(&line, &len, file)) != -1){
        moviesLikedParsed[i] = atoi(line);
        i++;
    }
    fclose(file);
    free(line);
    if(i == 0){
        printf("⚠️ The text file meant to contain the movies you like is empty ⚠️\n");
        printf("⛔ Program will be stopped ⛔\n");
        exit(1);
    }
    return moviesLikedParsed;
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

    //-------------- ARGUMENTS PARSING --------------
    extern char* optarg;
    int opt;

    int minmoviesreviewed = 0;
    int numMoviesLiked = 0;
    char* moviesLiked = NULL;
    int* moviesLikedParsed = NULL;
    int numMoviesRecommended = 10;
    char* folderpath = NULL;
    int dateLimit = 2006;
    int film_id = -1;
    char* clients = NULL;
    int numClients = 0;
    int* clientsParsed = NULL;
    char* bad_reviewers = NULL;
    int numBadReviewers = 0;
    int* badReviewersParsed = NULL;
    bool toption = false;
    bool ooption = false;
    int algochosen = 1;
    int ratingsConsidered = 30;

    while ((opt = getopt(argc, argv, "r:n:f:l:s:c:b:e:thoa:z:")) != -1) {
        switch (opt) {
            case 'h':
                print_usage();
                exit(0);
                break;
            case 'r':
                if(optarg[0] == '-' || optarg[0] == '\0'){
                    printf("⚠️ You didn't provide any movie you like ⚠️\n");
                    printf("⛔ Program will be stopped ⛔\n");
                    exit(1);
                }
                if(strstr(optarg, ".txt") != NULL){
                    moviesLikedParsed = movieslikedfromtxt(optarg);
                    numMoviesLiked = countLines(optarg);
                }else{
                    moviesLiked = optarg;
                    numMoviesLiked = num_movies_liked(moviesLiked);
                    moviesLikedParsed = malloc(numMoviesLiked * sizeof(int));
                    movies_liked_parsing(moviesLiked, &moviesLikedParsed, numMoviesLiked);
                }
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
            case 'o':
                ooption = true;
                break;
            case 'a':
                algochosen = atoi(optarg);
                break;
            case 'z':
                ratingsConsidered = atoi(optarg);
                break;
            default:
                fprintf(stderr, "Usage: %s -r <MovieYouLikeid1,MovieYouLikeid2,...>(or the path of a .txt) -n <numberOfMoviesYouWannaGetRecommended-f <folderpath> -l <num> -s <film_id> -c <client1,client2...> -b <bad_reviewer1,bad_reviewer2,...> -e <minmoviesreviewed> -t\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    //If the option -z is used but other options are not
    if(ratingsConsidered != 30 && dateLimit >= 2006 && clients == NULL && bad_reviewers == NULL && minmoviesreviewed == 0){
        printf("\033[1;37m"); // White Bold
        printf("⚠️ You are changing the number of ratings considered but you are not using options that require the graph to be created again ⚠️\n");
        printf("Are you sure you want to do that ? (y/n)\n");
        char* response = malloc(10*sizeof(char));
        scanf("%s", response);
        if(strcmp(response, "n") == 0){
            ratingsConsidered = 30;
        }
    }

    if(film_id == -1 && ooption == false){

        printf("\033[1;37m"); // White Bold
        printf("\n ──────────── 🔩 OPTIONS ──────────── \n\n");
        
        printf("\033[1;37m"); printf("💕 Movies Liked ");
        printf("\033[1;22m"); printf("(Option -r): %s\n", moviesLiked);
        
        printf("\033[1;37m"); printf("🔢 Number of Movies Recommended ");
        printf("\033[1;22m"); printf("(Option -n): %d\n", numMoviesRecommended);

        printf("\033[1;37m"); printf("📂 Folder Path ");
        printf("\033[1;22m"); printf("(Option -f): %s\n", folderpath);

        printf("\033[1;37m"); printf("📅 Limit Date ");
        printf("\033[1;22m"); printf("(Option -l): %d (ending of the database mid 2005)\n", dateLimit);

        printf("\033[1;37m"); printf("👥 Clients Considered ");
        printf("\033[1;22m"); printf("(Option -c): ");
        if (numClients > 0){
            for(int i = 0; i < numClients - 1; i++){
                printf("%d,", clientsParsed[i]);
            }
            printf("%d\n", clientsParsed[numClients - 1]);

        } else {
            printf("ALL\n");
        }

        printf("\033[1;37m"); printf("⭐ CLients Min Review Number ");
        printf("\033[1;22m"); printf("(Option -e): %d\n", minmoviesreviewed);


        printf("\033[1;37m"); printf("🚫 Reviewers Black-listed ");
        printf("\033[1;22m"); printf("(Option -b): ");
        if (numBadReviewers > 0){
            for(int i = 0; i < numBadReviewers - 1; i++){
                printf("%d,", badReviewersParsed[i]);
            }
            printf("%d\n", badReviewersParsed[numBadReviewers - 1]);

        } else {
            printf("NONE\n");
        }

        // printf("---- (option -s) : \n Film ID: %d\n", film_id);

        printf("\033[1;37m"); printf("⏰ Time of the Process Indicated ");
        printf("\033[1;22m"); printf("(Option -t): ");
        if (toption){
            printf("YES\n");
        } else {
            printf("NO\n");
        }
        printf("\033[1;37m"); printf("📁 Create .bin Files ");
        printf("\033[1;22m"); printf("(Option -o): ");
        if (ooption){
            printf("YES\n");
        } else {
            printf("NO\n");
        }
        printf("\033[1;37m"); printf("🎲 Algorithm Chosen ");
        printf("\033[1;22m"); printf("(Option -a): ");
        printf("%d\n", algochosen);
        printf("\033[1;37m"); // White Bold
    }


    //---------------------END OF ARGUMENTS PARSING---------------------

    printf("\033[1;37m"); // White Bold
    printf("\n ──────────── ⏳ PROCESS ──────────── \n\n");
    printf("\033[1;22m"); // Normal

    

    //Matrix representing the function to update weights between 2 movies based on the stars a same user gave to both movies
    float weights[5][5] = {
        {-0.25, -0.17,  0.0,   0.46,  1.0},
        {-0.17, -0.62, -0.46,  0.0,   0.5},
        { 0.0,  -0.46, -1.0,  -0.5,   0.0},
        { 0.46,  0.0,  -0.5,   1.0,  -0.5},
        { 1.0,   0.5,   0.0,  -0.5,  -1.0}
    };
    clock_t begin = clock();

    //If the user wants to create or recreate the .bin files
    if(ooption){
        printf("\033[1;33m");
        printf("Creating movies table.\n");
        clock_t createmoviestime = clock();
        movie* movies = createMovieTable("util/movie_titles.txt", "training_set");
        clock_t createmoviestimeend = clock();
        float createmoviestimespent = (float)(createmoviestimeend - createmoviestime) / CLOCKS_PER_SEC;
        printf("\033[1;32m");
        printf("Done. (%fs)                                      \n", createmoviestimespent);
        printf("\033[1;33m");
        printf("Serializing movies.\n");
        clock_t serializemoviestime = clock();
        serializeMovies(movies, NBMOVIES, "bin_creation/movies.bin");
        clock_t serializemoviestimeend = clock();
        float serializemoviestimespent = (float)(serializemoviestimeend - serializemoviestime) / CLOCKS_PER_SEC;
        printf("\033[1;32m");
        printf("Done. (%fs)                                      \n", serializemoviestimespent);
        printf("\033[1;33m");
        printf("Creating users table.\n");
        clock_t createuserstime = clock();
        user* users = createUsersTable(movies);
        clock_t createuserstimeend = clock();
        float createuserstimespent = (float)(createuserstimeend - createuserstime) / CLOCKS_PER_SEC;
        printf("\033[1;32m");
        printf("Done. (%fs)                                      \n", createuserstimespent);
        printf("\033[1;33m");
        printf("Serializing users.\n");
        clock_t serializeuserstime = clock();
        serializeUsers(users, NBUSERS, "bin_creation/users.bin");
        clock_t serializeuserstimeend = clock();
        float serializeuserstimespent = (float)(serializeuserstimeend - serializeuserstime) / CLOCKS_PER_SEC;
        printf("\033[1;32m");
        printf("Done. (%fs)                                      \n", serializeuserstimespent);
        printf("\033[1;33m");
        printf("Creating graph.bin.\n");
        clock_t creategraphtime = clock();
        float** graph = initGraph(NBMOVIES);
        updateGraph(graph, users, NBUSERS, NULL, 0, NULL, 0, 0, 2006, weights, ratingsConsidered);
        clock_t creategraphtimeend = clock();
        float creategraphtimespent = (float)(creategraphtimeend - creategraphtime) / CLOCKS_PER_SEC;
        printf("\033[1;32m");
        printf("Done. (%fs)                                      \n", creategraphtimespent);
        printf("\033[1;33m");
        printf("Serializing graph.\n");
        clock_t serializegraphtime = clock();
        serializegraph(graph, "algo/graphtest.bin");
        clock_t serializegraphtimeend = clock();
        float serializegraphtimespent = (float)(serializegraphtimeend - serializegraphtime) / CLOCKS_PER_SEC;
        printf("\033[1;32m");
        printf("Done. (%fs)                                      \n", serializegraphtimespent);
        freeGraph(graph, NBMOVIES);
        // freeMovies(movies, NBMOVIES);
        freeUsers(users, NBUSERS);
        exit(0);
    }

    //If the user wants to get statistics about a specific movie
    if(film_id != -1){

        printf("\033[1;33m");
        printf("Deserializing movies.\n");
        movie* movies = deserializeMovies("bin_creation/movies.bin");
        printf("\033[1;32m");
        printf("Done.                                      \n");

        printf("\033[1;37m"); // White Bold
        printf("\n ────────── 📃 INFORMATIONS ───────── \n\n");
        printf("\033[1;22m"); // Normal

        print_movie_stats(film_id, movies);
        freeMovies(movies, NBMOVIES);

        printf("\033[1;37m"); // White Bold
        printf("\n ──────────────────────────────────── \n\n");

        exit(0);
    }

    int nbUsers;
    user* users = NULL;
    float **graph = NULL;
    if(dateLimit >= 2006 && clients == NULL && bad_reviewers == NULL && minmoviesreviewed == 0 && ratingsConsidered == 30){
        printf("\033[1;33m");
        printf("Deserializing graph.\n");
        clock_t deserializetime = clock();
        graph = deserializegraph("algo/graph200.bin");
        clock_t deserializetimeend = clock();
        float deserializetimespent = (float)(deserializetimeend - deserializetime) / CLOCKS_PER_SEC;
        printf("\033[1;32m");
        printf("Done. (%fs)                                      \n", deserializetimespent);
    }else{
        printf("\033[1;33m");
        printf("Deserializing users.\n");
        clock_t deserializetime = clock();
        users = deserializeUsers("bin_creation/users.bin", &nbUsers);
        clock_t deserializetimeend = clock();
        float deserializetimespent = (float)(deserializetimeend - deserializetime) / CLOCKS_PER_SEC;
        printf("\033[1;32m");
        printf("Done. (%fs)                                      \n", deserializetimespent);
        printf("\033[1;33m");
        printf("Creating the graph.\n");
        clock_t graphtime = clock();
        graph = initGraph(NBMOVIES);
        clock_t graphtimeend = clock();
        float graphtimespent = (float)(graphtimeend - graphtime) / CLOCKS_PER_SEC;
        printf("\033[1;32m");
        printf("Done. (%fs)                                      \n", graphtimespent);

        printf("\033[1;33m");
        printf("Updating the graph.\n");
        clock_t updatetime = clock();
        updateGraph(graph, users, NBUSERS, badReviewersParsed, numBadReviewers, clientsParsed, numClients, minmoviesreviewed, dateLimit, weights, ratingsConsidered);
        clock_t updatetimeend = clock();
        float updatetimespent = (float)(updatetimeend - updatetime) / CLOCKS_PER_SEC;
        printf("\033[1;32m");
        printf("Done. (%fs)                                      \n", updatetimespent);
    }

    int* recommendedMovies = NULL;
    switch(algochosen){
        case 1:
            recommendedMovies = getNClosestMovies(moviesLikedParsed, numMoviesLiked, graph, numMoviesRecommended);
            break;
        case 2:
            recommendedMovies = getNClosestMovies2(moviesLikedParsed, numMoviesLiked, graph, numMoviesRecommended);
            break;
        default:
            printf("Error: algo chosen must be 1 or 2\n");
            exit(1);
    }
    
    clock_t end = clock();

    //Calculate the execution time of the program
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    if(toption){
        printf("\033[1;37m"); // White Bold
        printf("\nExecution time of the program was : %f seconds.\n", time_spent);
    }

    printf("\033[1;37m"); // White Bold
    printf("\n ──────────── 📊 RESULTS ──────────── \n\n");
    printf("\033[1;22m"); // Normal

    //Print the movies recommended
    printf("📝 Based on the movies you liked:\n\n");
    for(int i = 0; i < numMoviesLiked; i++){
        printf("▸ ");
        printf("\033[1;37m");
        char* title = getMovieTitle(moviesLikedParsed[i], "util/movie_titles.txt");
        printf("%s ", title);
        free(title);
        printf("\033[1;22m");
        printf("(id : %d)\n", moviesLikedParsed[i]);
    }

    
    printf("\n🎯 We recommend you the following movies:\n\n");
    for(int i = 0; i < numMoviesRecommended; i++){
        printf("▸ ");
        printf("\033[1;37m");
        char* title = getMovieTitle(recommendedMovies[i], "util/movie_titles.txt");
        printf("%s ", title);
        free(title);
        printf("\033[1;22m");
        printf("(id : %d)\n", recommendedMovies[i]);
    }

    
    printf("\033[1;37m"); // White Bold
    printf("\n ──────────────────────────────────── \n\n");

    //If the user wants to save the results in a txt file
    if(folderpath != NULL){
        char* string = malloc(10000 * sizeof(char));
        string[0] = '\0';
        strcat(string, "📝 Based on the movies you liked:\n\n");
        for(int i = 0; i < numMoviesLiked; i++){
            strcat(string, "▸ ");
            char* title = getMovieTitle(moviesLikedParsed[i], "util/movie_titles.txt");
            strcat(string, title);
            free(title);
            strcat(string, "(id ");
            char* id = malloc(10 * sizeof(char));
            sprintf(id, "%d", moviesLikedParsed[i]);
            strcat(string, id);
            strcat(string, ")\n");
            free(id);
        }
        strcat(string, "\n🎯 We recommend you the following movies:\n\n");
        for(int i = 0; i < numMoviesRecommended; i++){
            strcat(string, "▸ ");
            char* title = getMovieTitle(recommendedMovies[i], "util/movie_titles.txt");
            strcat(string, title);
            free(title);
            strcat(string, "(id ");
            char* id = malloc(10 * sizeof(char));
            sprintf(id, "%d", recommendedMovies[i]);
            strcat(string, id);
            strcat(string, ")\n");
            free(id);
        }
        write_txt(string, folderpath);
        free(string);
        printf("💾 Results saved in %s\n", folderpath);
    }

    // ---------------------Free the memory---------------------
    freeGraph(graph, NBMOVIES);
    free(recommendedMovies);
    free(moviesLikedParsed);
    free(clientsParsed);
    free(badReviewersParsed);
    if(dateLimit >= 2006 && clients == NULL && bad_reviewers == NULL && minmoviesreviewed == 0){
        free(users);
    }else{
        freeUsers(users, nbUsers);
    }
    return 0;

}

