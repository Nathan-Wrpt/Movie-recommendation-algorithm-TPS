# 💡 Movie recommendation algorithm by WURPILLOT Nathan and COUBRET Erwan

## ❓ Why ?

This project was made for the C programming course at the Telecom Physique Strasbourg engineering school. The goal was to create a movie recommendation algorithm based on the Netflix Prize dataset.

## ✨ How to use ?

### 1 ─ Download dependencies and Install

- If gcc is not installed on your computer, run these commands :
```bash
$ sudo apt update
$ sudo apt install build-essential
```
- Go to [Netflix Prize dataset](https://academictorrents.com/details/9b13183dc4d60676b773c9e2cd6de5e5542cee9a) and download ` nf_prize_dataset.tar.gz`

- Extract the download file, and then extract the `training_set.tar` in it

- Fork this project and pull it locally with git

### 2 ─ Use Makefile

Compile the project using the following command :

```bash
$ make
```

### 3 ─ Build Binary Files

Exectute the following command to create the binary files created in order to save data with precise structure that are necessary to create the graph of recommandaions :

```bash
$ ./main -o 
```

The program will then ask you to precise the path of the training_set folder. 

This command will create :

- `bin_creation/movies.bin`

- `bin_creation/user.bin`

[Details about those binary files](#details-about-the-binary-files)


### 4 - You are now able to use the recommendation algorithm :

```bash
$ ./main -r <id1,id2,...>
```

If you want to use the additional options provided by the program, you can use ./main -h to see all the options available and their usage.

Go get your recommendations !

:)

PS : We highly recommend you to download the graphWHOLEBDD.bin file to obtain the best results in a tiny amount of time.

Here's why :
<br />

| Ratings Considered | Time to update graph |
| --- | --- | 
| `5` | 0.669s 
| `10` | 3.050s 
| `20` | 13.053s 
| `30` | 31.887s
| `50` | 78.202s
| `100` | 253.987s
| `150` | 469.266s
| `ALL` | 4410.273s

<br />

> [Click here to see what the ratings considered mean](#ratings-considered-explanation)

graphWHOLEBDD.bin is the serialized version of the update graph with all the ratings considered. It means that to get a recommendation, it just takes 6.658463s (time to deserialize the graph) instead of ~4410s

## 🎥 Video Tutorial

Here is a demonstration of the program (with the graphWHOLEBDD.bin file downloaded)

![Alt Text](util/PROJECTDEMO.gif)

## 🌳 Structure of the project

```bash
< Project >
     | 
     |-- bin_creation/                              
     |    |-- movies.c            # Parses the dataset/creates the movie table
     |    |-- movies.bin          # Serialized movie table         
     |    |-- user.c              # Creates the user table      
     |    |-- user.bin            # Serialized user table       
     | 
     |-- util/
     |    |-- getmovietitle.c     # Functions to get the title of a movie
     |    |-- progressbar.c       # Progress bar implementation
     |    |-- maxadvices.c        # Function to count the Lines of a file and get
     |                            # the max amount of ratings a movie has
     |
     |-- algo                             
     |    |-- graphcreation.c     # All the functions to create/update the graph          
     |    |-- graphWHOLEBDD.bin   # The updated graph of the whole dataset 
     | 
     |-- all.h                    # Contains all the structures/global variables     
     |                        
     |-- ************************************************************************
```



## 🚀 Good to know

When you use the -g option to create your graph.bin after using ./main -r "likedmovies" -z "number" -g, the next time you will call the function with the same number in the z option, the process time will only be 6 seconds, as the program detects the graph.bin was created for this amount of ratings considered.

Great amount of time saved, isn't it ?

## 🛠️ How ?

The dataset contains 2 main parts, a text file named movie_titles.txt containing the movie id, the release year and the title of the movie, and a folder named training_set containing 17,770 text files, each one containing the movie id, the user id, the rating and the date of the rating.

Link to the dataset : https://academictorrents.com/details/9b13183dc4d60676b773c9e2cd6de5e5542cee9a

To take advantage of the dataset, we decided to create a movie structure containing the movie id, the release year, the title, the number of ratings and an array of all the ratings (which is a structure too). Moreover, we created a user structure containing the user id, the number of ratings and an array of all the ratings.
We then created a function to read the movie_titles.txt file and the corresponding text file in the training_set folder, and then store all the movies in an array of movie structures.
We then made a function to create a user table containing all the users and their ratings, based on the movie table we created right before.

Having all the data we needed, we then thought about the algorithm we would use to recommend movies to a user. We decided to create a graph where each node is a movie and each edge is a similarity between two movies. The similarity is calculated by comparing the ratings of the two movies by the same user. The more the ratings are close, the more the similarity is high.
We then created a function to create the graph with a matrix of size 17770x17770, and then we created a function to calculate the similarity between two movies iterating over all the users and their ratings with a function telling how much the distance between 2 movies should be adjusted depending on the rating of a same user on both movies.

With the graph updated, we could easily recommend movies to a user by looking at the movies he liked. Indeed, if the user tells us he likes a movie, we can look at the graph and find the nearest movies to the one he liked, and then recommend him the movies. 

The problem is : if he tells us he liked several movies, how to deal with the graph ? 
We made 2 different algorithm to do so :
- The first one creating an array of size 17770 and for each movie, we add the distance of all the movies he liked. We then find the nearest movies in this new array and recommend them to the user.

- The second one creating an array of size 17770 and for each movie, we add the distance of all the movies he liked, but this time ponderating the distance by the celebrity of the movie. This algorithm seems to be more efficient since the first one could take too much into consideration the famous movies and not enough the less famous ones in the list of liked movies.


#### Ratings considered explanation :

We encountered a problem while trying to recommend movies : it takes 4400s to calculate the graph and the similarity between all the movies, which is way too much. 
The solution we found was to create a file containing the graph and the similarity between all the movies, and then read it when we need it.
But this solution is not perfect since we need to recreate the graph if the user uses options such as -b, -c, -l, -e... Because to update the graph, we need to know beforehand if the user wants to exclude reviewers, change the minimum amount of reviews a user has to have to be considered, etc... 
So we decided to use this file taking in account the whole dataset only when no such options are used.

To avoid waiting 4400s every time we use an option, we created a new option : -z. This option limitates the number of ratings per user taken into account to create the graph. For example, for a reviewer having 252 ratings, if the user uses -z 50, only the 50 first ratings will be used to adjust the distances between the movies.
By default, the number of ratings taken into account is 30, taking an average time of 31s to update the whole graph, and giving pretty decent results.

## 🤔 If you are curious...
If further information is needed about the functions used to end up on this result, details about them are located in the header files of the project.

## Details about the binary files

- `bin_creation/movies.bin` : a table of movies, where every user has stocked its reviews. 

```c
//Structure of a rating
typedef struct rating_temp{
     int id_user;
     int id_film;
     int year;
     int day;
     int month;
     int star;
} rating;

// Structure of a movie
typedef struct movie_temp{
    int id;
    int release_date;
    char title[300];
    int nb_ratings;
    rating* ratings;
} movie;
```

- `bin_creation/user.bin` : a table of users, where every user has his reviews stored. 

```c
//Structure of a rating
typedef struct rating_temp{
     int id_user;
     int id_film;
     int year;
     int day;
     int month;
     int star;
} rating;

//Structure of a user
typedef struct user_temp{
    int id;
    int nb_ratings;
    rating* ratings;
} user;
```

---




