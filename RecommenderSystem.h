//
// Created on 2/20/2022.
//

#ifndef SCHOOL_SOLUTION_RECOMMENDERSYSTEM_H
#define SCHOOL_SOLUTION_RECOMMENDERSYSTEM_H
#include <map>
#include "RSUser.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include <cmath>
#include <utility>
#include <algorithm>
#define SIMILAR_MIN -2

using std::string;
using std::vector;
using std::pair;

typedef bool (*map_comp) (const sp_movie &m1, const sp_movie &m2);
typedef std::map<sp_movie, std::vector<double>, map_comp> movie_map;

class RecommenderSystem
{
 public:
  /** default constructor*/

  explicit RecommenderSystem () : _movie_features (map_comp)
  {}
  /**
   * adds a new movie to the system
   * @param name name of movie
   * @param year year it was made
   * @param features features for movie
   * @return shared pointer for movie in system
   */
  sp_movie
  add_movie (const std::string &name, int year, const std::vector<double> &features);

  /**
   * a function that calculates the movie with highest score based on movie features
   * @param ranks user ranking to use for algorithm
   * @return shared pointer to movie in system
   */
  sp_movie recommend_by_content (const RSUser &user);

  /**
   * a function that calculates the movie with highest predicted score based on ranking of other movies
   * @param ranks user ranking to use for algorithm
   * @param k
   * @return shared pointer to movie in system
   */
  sp_movie recommend_by_cf (const RSUser &user, int k);

  /**
   * Predict a user rating for a movie given argument using item cf procedure with k most similar movies.
   * @param user_rankings: ranking to use
   * @param movie: movie to predict
   * @param k:
   * @return score based on algorithm as described in pdf
   */
  double predict_movie_score (const RSUser &user, const sp_movie &movie,
                              int k);

  /**
   * gets a shared pointer to movie in system
   * @param name name of movie
   * @param year year movie was made
   * @return shared pointer to movie in system
   */
  sp_movie get_movie (const std::string &name, int year);
  /**
   * operator<< for movie
   * @param output ostream to output info with
   * @param recommender_system databae of all movies to print from
   */

  friend std::ostream &operator<< (std::ostream &output, const
  RecommenderSystem &recommender_system);
 private:
  static bool map_comp (const sp_movie &m1, const sp_movie &m2);
  movie_map _movie_features;
  ////////////////////////////////////////////////////////////////////////////
  /**
   * A function that normalises a copy of user ranks by calculating its rates
   * average and subtracting it from each of the vector values
   * @param user_ratings_copy copy of users rank map to normalize
   */
  static void normalize (rank_map &user_ratings_copy);
  /**
   * function to create preference vector from the normalized user ranks
   * @param normalised_vec normalized user rank copy
   * @param size number of features
   * @param movies movie database
   * @return preference vector
   */
  static vector<double> preference (rank_map &normalised_vec, int
  size, movie_map &movies);
  /**
	 * This function accepts two vector as input and calculates their
     * inner product
	 * @param vec1: first vector
	 * @param vec2: second vector
	 * @return the inner product of the two vectors
	 */
  static double inner_product (int size, vector<double>
  &vec1, vector<double> &vec2);
/**
	 * The function calculates the similarity of the preference vector
	 * with the features vector
	 * @param movie_features: movies databae
	 * @param preference: user's preference vector.
	 * @return similarity rate
	 */
  static double similarity (std::vector<double> &movie_features,
                            std::vector<double> &preference);
  /**
	 * This function return the best movie for that user to watch
	 * @param movies: movies database
	 * @param preference: user's preferences vector.
	 * @return the reccomended movie for that user to watch.
	 */
  static sp_movie chosen_movie (movie_map &movies, rank_map
  &user_rating, vector<double> &preference);
  /**
	 * this function creates  a similarity map
	 * @param movies: movies database
	 * @param user: the user
     * @param movie: unseen movie
     * @param k: number of most similar movies to return
	 * @return the k most similiar movies.
	 */
  static std::vector<std::pair<double, sp_movie>> create_similarity_map
      (movie_map &movies, const RSUser &user, const sp_movie &movie, int k);
  static bool sort_map (const std::pair<double, sp_movie> &a, const
  std::pair<double, sp_movie> &b);

};

#endif //SCHOOL_SOLUTION_RECOMMENDERSYSTEM_H
