#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void print_usage(){
        printf("Usage: ./main -f <folderpath> -l <num> -s <film_id> -c <client1,client2> -b <bad_reviewer1,bad_reviewer2,...> -e <minmoviesreviewed> -t\n");
        printf("Note : toutes les options sont optionnelles\n");
        printf("Options :\n");
        printf("-f <folderpath> : chemin du dossier où seront sauvegardés les fichiers correspondants aux résultats demandés\n");
        printf("-l <num> : permet de ne pas prendre en compte les fils dont la date est supérieure à <num>\n");
        printf("-s <film_id> : donne des statistiques sur le film d'identifiant <film_id> (nombre de notes, note moyenne ...\n");
        printf("-c <client1,client2> : permet de ne prendre en compte que les notes des clients <client1> et <client2>\n");
        printf("-b <bad_reviewer1,bad_reviewer2,...> : permet de ne pas prendre en compte les notes des reviewers <bad_reviewer1>, <bad_reviewer2> ...\n");
        printf("-e <minmoviesreviewed> : ermettra de ne prendre en compte que les clients d'élite ayant vu un minimum de <minmoviesreviewed> films\n"); 
        printf("-t : précise le temps d'exécution de l'algorithme");
}

//Compte le nombre de mauvais reviewers après l'options -b, séparés par des ','

int num_bad_reviewers(char* bad_reviewers){ 
        int num = 0;
        char* token = strtok(bad_reviewers, ",");
        while(token != NULL){
                num++;
                token = strtok(NULL, ",");
        }
        return num;
}


//Parse les mauvais reviewers après l'options -b et les stocke dans un tableau

char** bad_reviewers_parsing(char* bad_reviewers, char** bad_reviewers_parsed, int numbadreviewers){ 
        char** bad_reviewers_parsed = (char**)malloc(1000*sizeof(char*));
        int i = 0;
        while(i < numbadreviewers){
                bad_reviewers_parsed[i] = (char*)malloc(1000*sizeof(char));
                i++;
        }
        char* token = strtok(bad_reviewers, ",");
        int i = 0;
        while(token != NULL){
                strcpy(bad_reviewers_parsed[i], token);
                token = strtok(NULL, ",");
                i++;
        }
        return bad_reviewers_parsed;
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
    //----------------------------------------------------------------------------------------------
    //-----------------------------------TRAITEMENT DES ARGUMENTS-----------------------------------
    //----------------------------------------------------------------------------------------------
    if(argv[1] == "-h"){
        print_usage();
        exit(0);
    }
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
    char* client1 = (char*)malloc(1000*sizeof(char));
    char* client2 = (char*)malloc(1000*sizeof(char));

    //option -b
    bool boption = false;
    char* bad_reviewers = (char*)malloc(10000*sizeof(char));
    bad_reviewers = "";
    //Quand les arguments auront été traités, on stockera dans le tableau bad_reviewers_parsed les mauvais reviewers, et dans numbadreviewers le nombre de mauvais reviewers
    //c'est fait en-dessous mais je le mets ici pour qu'on sache dans quelle variables c'est stocké

    //option -e
    bool eoption = false;
    int minmoviesreviewed = 0;

    //option -t
    bool toption = false;

    //On parse les arguments de la ligne de commande et stocke les valeurs dans les variables définies ci-dessus
    parse_args(argc, argv, foption, folderpath, loption, num, soption, film_id, coption, clients, boption, bad_reviewers, eoption, minmoviesreviewed, toption);

    //Si l'option -b est activée, on parse les mauvais reviewers et on les stocke dans un tableau
    if(boption == true){
        int numbadreviewers = num_bad_reviewers(bad_reviewers);
        char** bad_reviewers_parsed = bad_reviewers_parsing(bad_reviewers, bad_reviewers_parsed, numbadreviewers);
    }
    //Si l'option -c est activée, on parse les deux clients et on les stocke dans deux variables
    if(coption == true){
        char* token = strtok(clients, ",");
        strcpy(client1, token);
        token = strtok(NULL, ",");
        strcpy(client2, token);
    }
    //-----------------------------------------------------------------------------------------------------
    //-----------------------------------FIN DE TRAITEMENT DES ARGUMENTS-----------------------------------
    //-----------------------------------------------------------------------------------------------------


    //free de toutes les variables allouées
    free(folderpath);
    free(film_id);
    free(clients);
    free(client1);
    free(client2);
    free(bad_reviewers);
    free(bad_reviewers_parsed);
    return 0;
}

