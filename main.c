#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>




int num_bad_reviewers(char* bad_reviewers){ //compte le nombre de mauvais reviewers après l'options -b
        int num = 0;
        char* token = strtok(bad_reviewers, ",");
        while(token != NULL){
                num++;
                token = strtok(NULL, ",");
        }
        return num;
}

char* bad_reviewers_parsing(char* bad_reviewers){ //parse les mauvais reviewers après l'options -b et les stocke dans un tableau
        int num = num_bad_reviewers(bad_reviewers);
        char** bad_reviewers_parsed = (char**)malloc(num*sizeof(char*));
        int i = 0;
        while(i < num){
                bad_reviewers_parsed[i] = (char*)malloc(1000*sizeof(char));
                i++;
        }
        char* token = strtok(bad_reviewers, ",");
        i = 0;
        while(token != NULL){
                strcpy(bad_reviewers_parsed[i], token);
                token = strtok(NULL, ",");
                i++;
        }
}

void parse_args(int argc, char* argv[], bool foption, char* folderpath, bool loption, int num, bool soption, char* film_id, bool coption, char* clients, bool boption, char* bad_reviewers, bool eoption, int minmoviesreviewed, bool toption){
        int i = 0;
        if(argv[1] == "-h"){
            print_usage();
            exit(0);
        }
        while(i < argc){
                if(strcmp(argv[i], "-f") == 0){
                        foption = true;
                        folderpath = argv[i+1];
                        i++;
                }
                else if(strcmp(argv[i], "-l") == 0){
                        loption = true;
                        num = atoi(argv[i+1]);
                        i++;
                }
                else if(strcmp(argv[i], "-s") == 0){
                        soption = true;
                        film_id = argv[i+1];
                        i++;
                }
                else if(strcmp(argv[i], "-c") == 0){
                        coption = true;
                        clients = argv[i+1];
                        i++;
                }
                else if(strcmp(argv[i], "-b") == 0){
                        boption = true;
                }
                else if(strcmp(argv[i], "-e") == 0){
                        eoption = true;
                        minmoviesreviewed = atoi(argv[i+1]);
                        i++;
                }
                else if(strcmp(argv[i], "-t") == 0){
                        toption = true;
                }
                i++;
        }
}

int main(int argc, char* argv[]){
    //option -f
    bool foption = false;
    char* folderpath = (char*)malloc(1000*sizeof(char));
    folderpath = "";

    //option -l
    bool loption = false;
    int num = 0;

    //option -s
    bool soption = false;
    char* film_id = (char*)malloc(1000*sizeof(char));
    film_id = "";

    //option -c
    bool coption = false;
    char* clients = (char*)malloc(1000*sizeof(char));

    //option -b
    bool boption = false;
    char* bad_reviewers = (char*)malloc(10000*sizeof(char));
    char** bad_reviewers_parsed = (char**)malloc(1000*sizeof(char*));
    bad_reviewers = "";
    //option -e
    bool eoption = false;
    int minmoviesreviewed = 0;

    //option -t
    bool toption = false;

    parse_args(argc, argv, foption, folderpath, loption, num, soption, film_id, coption, clients, boption, bad_reviewers, eoption, minmoviesreviewed, toption);
    if(boption == true){
        char** bad_reviewers_parsed = bad_reviewers_parsing(bad_reviewers);
    }
    return 0;
}

