#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


//option -f
bool foption = false;
char* folderpath = malloc(1000*sizeof(char));
folderpath = "";

//option -l
bool loption = false;
int num = 0;

//option -s
bool soption = false;
char* film_id = malloc(1000*sizeof(char));
film_id = "";

//option -c
bool coption = false;
char* client1 = malloc(1000*sizeof(char));
client1 = "";
char* client2 = malloc(1000*sizeof(char));
client2 = "";

//option -b
bool boption = false;
char* bad_reviewers = malloc(1000*sizeof(char));
bad_reviewers = "";
//option -e
bool eoption = false;
int minmoviesreviewed = 0;

//option -t
bool toption = false;

void print_usage(){
    printf("Usage: ./main -f <folderpath> -l <num> -s <film_id> -c <client1,client2> -b <bad_reviewers -e <minmoviesreviewed> -t\n");
}

void parse_args(int argc, char* argv[]){
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
                        client1 = argv[i+1];
                        client2 = argv[i+2];
                        i+=2;
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
        parse_args(argc, argv);
}

