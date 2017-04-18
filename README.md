# sixDegreeofKevinBacon

##Overview This repository holds an implementation of the Six Degrees of Kevin Bacon game. The goal is to find the degree of separation between Hollywood actors that act in the same movies.

The Six Degrees of Kevin Bacon game is based off the parlor game, the "six degrees of separation" concept, which claims that any two people on Earth are six or fewer acquaintance-links apart. The same idea can then be morphed into this game wherein movie buffs challenge each other to find the shortest path between any arbritrary actor and the prolific Hollywood actor, Kevin Bacon. It is assumed that any actor involved in the Hollywood film industry can be linked through his or her appearance in their film roles to Kevin Bacon within six steps. The game can be described as a trivia game based on the concept of the small world phenomenon.

##Implementation

###ActorGraph The ActorGraph serves as a container for all of the actors & the movie they've each have had a role in. The implementation of the graph consists of each actor having a respective ActorNodes, which, primarily holds the:

Actor's name
An adjacency list that holds all connections to all of the other actors the primary actor shares through movies.
A list of all the movies he or she has starred in.
In the graph, each actor to other actors via an ActorEdge. The ActorEdge connects two ActorNodes through a common movie they've both starred in. The edge consists of:

A pointer to a movie object
A pointer to the primary actor
A pointer to the secondary actor
Weight integer that is calculated by weight = 1 + (2015 - Y), where Y is the year the movie was released.
Lastly, the last class used by the graph is the Movie class. The Movie class simply serves as an object for all the different movies in the graph & consist of:

Movie name
The release date
And a hash map holding pointers to all of the cast members (actors).
###pathfinder The purpose of this program is to find a path from one actor to another. The program will take different modes, either weighted or unweighted and will export an out file. The exported results will display the entire path consisting of the commonly shared movie & the actor's name. Essentially, this program implements the entire concept of the Six Degrees of Kevin Bacon game.

To run pathfinder: ./pathfinder movie_casts.tsv u test_pairs.tsv out_paths.tsv. Where:

movie_cast.tsv contains the majority of actors/actresses found in IMDB and the movies they've played in.
u refers to the mode, u for unweighted or w for weighted.
test_pairs.tsv is a file containing the various path you want to find from one actor to another.
out_paths.tsv is the output file containing the shortest paths.
####unweighted In the unweighted mode, the program will implement a BFS traversal of the map from the starting point actor. Given that the BFS traversal is an exhaustive, each actor, not just the one we are trying to find, will have the shortest path from the given starting actor. Thus, when finding the shortest path, we take the ending actor and follow the path back to the starting actor. BFS guarantees that this will one of the possible shortest paths.

####weighted The weighted mode will look for shortest path based off the earliest date a movie was release in. Therefore, a Djikstra's algorithm is implemented to find the shortest path to an actor. Again, the Djikstra's algorithm is an exhaustive search & will find the shortest path to all of the other actors, not just the targeted one. Just like the unweighted mode, we will take the ending actor & follow the path back to the starting actor to find one of the possible shortest paths.

###actorconnections This program aims to find the earliest year that two actors become connected through a path of connected actors. This program implements two modes to find the given year. Prior to finding the year, the movies are arranged by year they are released in ascending order.

To run actorsconnections: ./actorconnections movie_cast_file.tsv pair_file.tsv output_file.tsv ufind, where:

movie_cast.tsv contains the majority of actors/actresses found in IMDB and the movies they've played in.
pair_fie.tsv is a file containing the pair of actors which you want to find dates for.
output_file.tsv is the output file containing the actors & the earliest date they became connected.
ufind refers to the mode, ufind for union-find mode or bfs for the BFS mode.
####BFS The BFS mode uses the same approach as the shown previously in the pathfinder program, but differs slightly because instead of adding all the pool of actors at once like before, we only add the actors that starred in a movie on that year. For each iteration of the year, the actors are added to the ActorGraph & a BFS is done in an attempt to find a path between the two given actors. If no connection is found the year will increment & the same process will repeat. This method will guarantee to find the earliest year that two actors have a path connecting them. If at the end of the traversal there is no existing path between the 2 actors, then there is no possible connection among them.

####Union-Find The union-find mode implements a disjoint-set data structure that allows you to keep track of all the connected sets of actors without maintaining the graph structure. Like in the BFS mode, the program will continue to add actors in increasing years to a hash map structure. At the end of each yearly iteration, all sets of actors will connect to one leading set (the highest ranking). With an added path compression implementation, checking if two actors connect is relatively simple. All that is needed is a to check if each actor's highest ranking set & compare if the two are the same. If the same, then a connection exists that year. If not, the year will increment & the process will repeat accordingly. This will result with the earliest year two actors become connected (if a connection exists).

##File structuring

###movie_cast.tsv The provided movie_cast.tsv or any other cast file must maintain the following structure:

Actor/Actress<TAB>Movie<TAB>Year
50 CENT<TAB>BEEF<TAB>2003
50 CENT<TAB>BEFORE I SELF DESTRUCT<TAB>2009
50 CENT<TAB>THE MC: WHY WE DO IT<TAB>2005
50 CENT<TAB>CAUGHT IN THE CROSSFIRE<TAB>2010
50 CENT<TAB>THE FROZEN GROUND<TAB>2013
50 CENT<TAB>BEEF III<TAB>2005
50 CENT<TAB>LAST VEGAS<TAB>2013
50 CENT<TAB>GUN<TAB>2010
...
The <TAB> denotes a single tab character.

###test_pairs.tsv The file containing the testing pairs for both the pathfinder & actorconnections must follow this structure:

Actor1/Actress1 Actor2/Actress2
BACON, KEVIN (I)<TAB>HOUNSOU, DJIMON
BACON, KEVIN (I)<TAB>KIDMAN, NICOLE
BACON, KEVIN (I)<TAB>WILLIS, BRUCE
BACON, KEVIN (I)<TAB>GIAMATTI, PAUL
HOUNSOU, DJIMON<TAB>50 CENT
The <TAB> denotes a single tab character.

###Output results

####pathfinder The output results for pathfinder will follow a structure as follows:

(actor)--[movie#@year]-->(actor)--...
(BACON, KEVIN (I))--[ELEPHANT WHITE#@2011]-->(HOUNSOU, DJIMON)
(BACON, KEVIN (I))--[SUPER#@2010]-->(MCKAY, COLE S.)--[FAR AND AWAY#@1992]-->(KIDMAN, NICOLE)
(BACON, KEVIN (I))--[SUPER#@2010]-->(MORENO, DARCEL WHITE)--[LAY THE FAVORITE#@2012]-->(WILLIS, BRUCE)
(BACON, KEVIN (I))--[A FEW GOOD MEN#@1992]-->(MOORE, DEMI)--[DECONSTRUCTING HARRY#@1997]-->(GIAMATTI, PAUL)
(HOUNSOU, DJIMON)--[IN AMERICA#@2002]-->(MARTINEZ, ADRIAN (I))--[MORNING GLORY#@2010]-->(50 CENT)
####actorconnections The output results for actorconnections will follow a structure as follows:

Actor1<TAB>Actor2<TAB>Year
BACON, KEVIN (I)<TAB>HOUNSOU, DJIMON1992
BACON, KEVIN (I)<TAB>KIDMAN, NICOLE1991
BACON, KEVIN (I)<TAB>WILLIS, BRUCE1990
BACON, KEVIN (I)<TAB>GIAMATTI, PAUL1992
HOUNSOU, DJIMON<TAB>50 CENT2003
The <TAB> denotes a single tab character.
