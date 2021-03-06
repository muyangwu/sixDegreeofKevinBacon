/*
 * ActorGraph.cpp
 * Author: Ruoxin Huang, Muyang Wu
 * Date:  3/2/2017
 *
 * This is the implementation file of ActorGraph class.
 */

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include <utility>
#include "ActorGraph.h"
#include "UnionFind.hpp"
#define MAX 2147483647
using namespace std;
/*
 *  Constructor of ActorGraph object
 *
 *  no parameter
 *
 *  no return.
 */
ActorGraph::ActorGraph(void) {
  curr_movie_year = 2015;
}

//destructor of actor graph
ActorGraph::~ActorGraph()
{
  for(unordered_map<string,ActorNode*>::iterator it = actors.begin()
	; it != actors.end(); ++it)
    delete (it->second);

  for(unordered_map<string,Movies*>::iterator it = movies.begin()
	; it != movies.end(); ++it)
    delete (it->second);
}

/*
 * Load the graph from a tab-delimited file of actor->movie relationships. Then
 * load them into two unordered_map variables.
 *
 * in_filename - input filename
 * use_weighted_edges - if true, compute edge weights as 1 + (2015 - movie_year), otherwise all edge weights will be 1
 *
 * return true if file was loaded sucessfully, false otherwise
 */
bool ActorGraph::loadFromFile(const char* in_filename, bool use_weighted_edges) {
    // Initialize the file stream
    ifstream infile(in_filename);

    bool have_header = false;

    // keep reading lines until the end of file is reached
    while (infile) {
        string s;

        // get the next line
        if (!getline( infile, s )) break;

        if (!have_header) {
            // skip the header
            have_header = true;
            continue;
        }

        istringstream ss( s );
        vector <string> record;

        while (ss) {
            string next;

            // get the next string before hitting a tab character and put it in 'next'
            if (!getline( ss, next, '\t' )) break;

            record.push_back( next );
        }

        if (record.size() != 3) {
            // we should have exactly 3 columns
            continue;
        }

        string actor_name(record[0]);
        string movie_title(record[1]);
        int movie_year = stoi(record[2]);
        // insert actors and movies into respective map
        if(actors.find(actor_name) == actors.end() && movies.find(movie_title) == movies.end()){
          // no actor or movie exists, add new actor and movie
          ActorNode* a = new ActorNode(actor_name);
          Movies* m = new Movies(movie_title,movie_year);
          a->addMovie(m);
          actors.emplace(actor_name, a);
          m->addActor(a);
          movies.emplace(movie_title, m);
        }
        else if(actors.find(actor_name) != actors.end() && movies.find(movie_title) != movies.end()){

          //both actor and movie exists, add respective pointers to them
          (actors.find(actor_name))->second->addMovie((movies.find(movie_title))->second);
          (movies.find(movie_title))->second->addActor((actors.find(actor_name))->second);

        }
        else if(actors.find(actor_name) != actors.end() && movies.find(movie_title) == movies.end()){
          //actors exists, create new movie and add actor's pointer
          Movies * m = new Movies(movie_title,movie_year);

          m->addActor((actors.find(actor_name))->second);

          movies.emplace(movie_title, m);

          //add new movie pointer to actor
          (actors.find(actor_name))->second->addMovie(m);


        }
        else if(actors.find(actor_name) == actors.end() && movies.find(movie_title) != movies.end()){
          // actor does not exist, create new actor and add respective movie pointer to it

          ActorNode * a = new ActorNode(actor_name);
          a->addMovie((movies.find(movie_title))->second);
          actors.emplace(actor_name, a);
          // add new actor pointer to movies
          (movies.find(movie_title))->second->addActor(a);
        }
        // we have an actor/movie relationship, now what?
    }

    if (!infile.eof()) {
        cerr << "Failed to read " << in_filename << "!\n";
        return false;
    }
    infile.close();

    return true;
}
/*
 * Load the graph from a tab-delimited file of actor->movie relationships. And * try to find the oldest movie year.
 *
 * in_filename - input filename
 *
 * return true if file was loaded sucessfully, false otherwise
 */
bool ActorGraph::getOldestYear(const char* in_filename) {
    // Initialize the file stream
    ifstream infile(in_filename);

    bool have_header = false;
    // keep reading lines until the end of file is reached
    while (infile) {
        string s;

        // get the next line
        if (!getline( infile, s )) break;

        if (!have_header) {
            // skip the header
            have_header = true;
            continue;
        }

        istringstream ss( s );
        vector <string> record;

        while (ss) {
            string next;

            // get the next string before hitting a tab character and put it in 'next'
            if (!getline( ss, next, '\t' )) break;

            record.push_back( next );
        }

        if (record.size() != 3) {
            // we should have exactly 3 columns
            continue;
        }

        string actor_name(record[0]);
        string movie_title(record[1]);
        int movie_year = stoi(record[2]);
        if(movie_year < curr_movie_year)
          curr_movie_year = movie_year;

      }
    if (!infile.eof()) {
        cerr << "Failed to read " << in_filename << "!\n";
        return false;
    }
    infile.close();

    return true;
}
/*
 * Build the graph according to year parameter, only the same year will be
 * added to map.
 *
 * in_filename - input filename
 * year - at which year this function should load into
 * no return
 */
void ActorGraph::buildGraphByYear(const char* in_filename, int year){
  // Initialize the file stream
  ifstream infile(in_filename);

  bool have_header = false;

  // keep reading lines until the end of file is reached
  while (infile) {
      string s;

      // get the next line
      if (!getline( infile, s )) break;

      if (!have_header) {
          // skip the header
          have_header = true;
          continue;
      }

      istringstream ss( s );
      vector <string> record;

      while (ss) {
          string next;

          // get the next string before hitting a tab character and put it in 'next'
          if (!getline( ss, next, '\t' )) break;

          record.push_back( next );
      }

      if (record.size() != 3) {
          // we should have exactly 3 columns
          continue;
      }

      string actor_name(record[0]);
      string movie_title(record[1]);
      int movie_year = stoi(record[2]);
      // insert actors and movies into respective map
      if(movie_year == year){
	if(actors.find(actor_name) == actors.end() && movies.find(movie_title) == movies.end()){
	  // no actor or movie exists, add new actor and movie
	  ActorNode* a = new ActorNode(actor_name);
	  Movies* m = new Movies(movie_title,movie_year);
	  a->addMovie(m);
	  actors.emplace(actor_name, a);
	  m->addActor(a);
	  movies.emplace(movie_title, m);
	}
	else if(actors.find(actor_name) != actors.end() && movies.find(movie_title) != movies.end()){

	  //both actor and movie exists, add respective pointers to them
	  (actors.find(actor_name))->second->addMovie((movies.find(movie_title))->second);
	  (movies.find(movie_title))->second->addActor((actors.find(actor_name))->second);

	}
	else if(actors.find(actor_name) != actors.end() && movies.find(movie_title) == movies.end()){
	  //actors exists, create new movie and add actor's pointer
	  Movies * m = new Movies(movie_title,movie_year);

	  m->addActor((actors.find(actor_name))->second);

	  movies.emplace(movie_title, m);

	  //add new movie pointer to actor
	  (actors.find(actor_name))->second->addMovie(m);


	}
	else if(actors.find(actor_name) == actors.end() && movies.find(movie_title) != movies.end()){
        // actor does not exist, create new actor and add respective movie pointer to it

	  ActorNode * a = new ActorNode(actor_name);
	  a->addMovie((movies.find(movie_title))->second);
	  actors.emplace(actor_name, a);
	  // add new actor pointer to movies
	  (movies.find(movie_title))->second->addActor(a);
	}
    }
  }

  if (!infile.eof()) {
    cerr << "Failed to read " << in_filename << "!\n";
  }
  infile.close();

}
/*
 * Build the graph according to year parameter, only the same year will be
 * added to map.
 *
 * in_filename - input filename
 *
 * no return
 */
void ActorGraph::buildExtensionGraph(const char* in_filename){
  // Initialize the file stream
  ifstream infile(in_filename);

  bool have_header = false;
  // keep reading lines until the end of file is reached
  while (infile) {
      string s;

      // get the next line
      if (!getline( infile, s )) break;

      if (!have_header) {
          // skip the header
          have_header = true;
          continue;
      }

      istringstream ss( s );
      vector <string> record;

      while (ss) {
          string next;

          // get the next string before hitting a tab character and put it in 'next'
          if (!getline( ss, next, ' ' )) break;

          record.push_back( next );
      }

      if (record.size() != 2) {
          // we should have exactly 2 columns
          continue;
      }

      string person1(record[0]);
      string person2(record[1]);
      if(actors.find(person1) == actors.end()){
    	  ActorNode* a = new ActorNode(person1);
    	  a->people.emplace(person2);
    	  actors.emplace(person1, a);
    	}
    	else if(actors.find(person1) != actors.end()){

    	  //both actor and movie exists, add respective pointers to them
    	  actors[person1]->people.emplace(person2);

    	}


    }
  if (!infile.eof()) {
      cerr << "Failed to read " << in_filename << "!\n";
  }
  infile.close();

}

/*
 * Do breath-first search to find the shortst path
 *
 * actor1 - ActorNode of first actor
 *
 * no return
 */
void ActorGraph::BFS(ActorNode* actor1) {
  ActorNode* curr;
  ActorNode* currA;
  Movies* currM;
  queue<ActorNode*> queue;
  queue.push(actor1); //push origin actor into queue
  actor1->distance=0; //set distance to 0
  while(!queue.empty()){
    curr = queue.front();
    queue.pop(); //pop element
    //for all movies the actor took part
    for(int i=0; i<curr->movies.size(); i++){
      currM = curr->movies[i];// set pointer to one of his movies
      //for all actors in the movie
      for(int j=0; j<currM->starring.size(); j++){
	currA = currM->starring[j]; //set pointer to one of its actor
	if(currA->distance==MAX){ //if actor is not already searched
	  currA->distance = curr->distance+1; //set distance to previous distance +1
	  currA->prev = curr;// set prev
	  currA->movie = currM->name;
	  currA->year = currM->year;
	  queue.push(currA);//push it to queue
	}
      }
    }
  }
}
/*
 * Do dijkstra's algorithm to find the shortst path
 *
 * actor1 - ActorNode of first actor
 *
 * no return
 */
void ActorGraph::Dijk(ActorNode* actor1) {
  ActorNode* curr;
  ActorNode* currA;
  Movies* currM;
  priority_queue<ActorNode*, vector<ActorNode*>,ActorNodePtrComp> queue;
  int distance;
  queue.push(actor1); //push origin actor into queue
  actor1->distance=0; //set distance to 0
  while(!queue.empty()){
    curr = queue.top();
    queue.pop(); //pop element
    if( (curr->done) == false)
      curr->done=true;
    //for all movies the actor took part
    for(int i=0; i<curr->movies.size(); i++){
      currM = curr->movies[i];// set pointer to one of his movies
      //for all actors in the movie
      for(int j=0; j<currM->starring.size(); j++){
	currA = currM->starring[j]; //set pointer to one of its actor
	distance = curr->distance+1+(2015-currM->year);
	if(distance<currA->distance){ //if actor is not already searched
	  currA->distance = distance; //set distance to previous distance +1
	  currA->prev = curr;// set prev
	  currA->movie = currM->name;
	  currA->year = currM->year;
	  queue.push(currA);//push it to queue
	}
      }
    }
  }
}
/*
 * Clear the graph, distance is set to MAX (which is roughly infinity), prev
 * ActorNode to null and done is false
 *
 * no parameter
 *
 * no return
 */
void ActorGraph::clear(){
  for(auto it = actors.begin(); it != actors.end(); ++it){
    it->second->distance = MAX;
    it->second->prev = NULL;
    it->second->done = false;
  }
}
