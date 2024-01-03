# Movie recommendation algorithm by WURPILLOT Nathan and COUBRET Erwan

## Why ?

This project was made for the C programming course at the Telecom Physique Strasbourg engineering school. The goal was to create a movie recommendation algorithm based on the Netflix Prize dataset.

## How ?

The dataset contains 2 main parts, a text file named movie_titles.txt containing the movie id, the release year and the title of the movie, and a folder named training_set containing 17770 text files, each one containing the movie id, the user id, the rating and the date of the rating.

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

We encountered a problem while trying to recommend movies : it takes 4400s to calculate the graph and the similarity between all the movies, which is way too much. 
The solution we found was to create a file containing the graph and the similarity between all the movies, and then read it when we need it.
But this solution is not perfect since we need to recreate the graph if the user uses options such as -b, -c, -l, -e... Because to update the graph, we need to know beforehand if the user wants to exclude reviewers, change the minimum amount of reviews a user has to have to be considered, etc... 
So we decided to use this file taking in account the whole dataset only when no such options are used.
To avoid waiting 4400s every time we use an option, we created a new option : -z. This option limitates the number of ratings per user taken into account to create the graph. For example, for a reviewer having 252 ratings, if the user uses -z 50, only the 50 first ratings will be used to adjust the distances between the movies.
By default, the number of ratings taken into account is 30, taking an average time of 31s to update the whole graph, and giving pretty decent results.

For your information, here are the average times to update the graph depending on the number of ratings taken into account (on our PC):
- 5 ratings : (0.669877s)
- 10 ratings : (3.050473s)
- 20 ratings : (13.053415s)
- 30 ratings : (31.887728s)
- 50 ratings : (78.202530s)
- 100 ratings : (253.987503s)
- 150 ratings : (469.266174s)

- EVERY rating : (4410.273438s)

- Simply deserializing a graph created before : (6.658463s)

## How to use ?

To use the program, you need to compile it with the makefile, and then run it with the following command :
./main -o to create all the .bin files needed to run the program (you need to do this only once). We highly recommend you to download the graphWHOLEBDD.bin file to obtain the best results in your recommendations.
You can then use ./main -h to see all the options available and their usage.

Go get your recommendations !

:)

## Structure of the project

The project is divided in 3 main folders :
- bin_creation : contains the files to translate the .txt files into movies array and users array, and then serialize it into binary files (movies.bin containing the movies array, users.bin the users array)
- util : contains all the files than can be useful, such as getmovietitle that helps us to get the title of a movie given its id and the .txt file, or the progressbar that prevents YOU from being bored when using the program(Yes, us too, we spent hours in front of the terminal waiting for the program to finish)
- algo : contains all the files needed to create the graph and the similarity between the movies, and then recommend movies to the user.


## Good to know

When you use the -g option to create your graph.bin after using ./main -r "likedmovies" -z "number" -g, the next time you will call the function with the same number in the z option, the process time will only be 6 seconds, as the program detects the graph.bin was created for this amount of ratings considered.

Great amount of time saved, isn't it ?

## If you are curious...
If further information is needed about the functions used to end up on this result, details about them are located in the header files of the project.

## Demonstration

Here is a demonstration of the program, with the graphWHOLEBDD.bin file downloaded from the releases section of the project.

GIF :
Display of the gif located in the util folder named PROJECTDEMO.gif :

![Alt Text](util/PROJECTDEMO.gif)








