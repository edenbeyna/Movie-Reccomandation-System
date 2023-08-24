
#include "RecommenderSystem.h"
#include <map>

using std::vector;
using std::map;
using std::pair;
using std::string;


bool RecommenderSystem::map_comp(const sp_movie& m1,const sp_movie& m2){
  return *m1 < *m2;
}

sp_movie RecommenderSystem::add_movie(const std::string &name, int year, const
std::vector<double> &features)
{
  sp_movie new_movie = std::make_shared<Movie> (name, year);
  _movie_features[new_movie] = features;
  return new_movie;

}

void RecommenderSystem::normalize (rank_map &user_ratings_copy)
{
  double sum = 0.0;
  int divisor = 0;
  for (auto const &value: user_ratings_copy)
  {
    if (value.second != 0)
    {
      sum += value.second;
      divisor++;
    }
  }
  sum = sum / divisor;
  for (auto &value: user_ratings_copy)
  {
    if (value.second!= 0)
    {
      value.second -= sum;
    }
  }
}

vector<double> RecommenderSystem::preference(rank_map &normalised_vec, int
size, movie_map &movies)
{
  vector<double> preference_vec (size, 0.0);
  for (auto &movie: movies)
  {
    double value = normalised_vec[movie.first];
    if (value!=0)
    {
      for (int i = 0; i < size; i++)
      {
        double c = value * movie.second[i];
        preference_vec[i] += c;
      }
    }
  }
  return preference_vec;
}

double RecommenderSystem::inner_product (int size, vector<double>
    &vec1,
                                         vector<double> &vec2)
{
  double result = 0;
  for (int i = 0; i < size; ++i)
  {
    double c = vec1[i] * vec2[i];
    result += c;
  }
  return result;
}

double RecommenderSystem::similarity(vector<double> &movie_features,
                                     vector<double> &prefVec)
{
  double norm1 = sqrt(inner_product(movie_features.size(),movie_features,
      movie_features));
  double norm2 = sqrt(inner_product(movie_features.size(), prefVec, prefVec));
  return inner_product (movie_features.size(),movie_features, prefVec) /
  (norm1 * norm2);
}


sp_movie RecommenderSystem::chosen_movie(movie_map &movies,rank_map
&user_rating, vector<double> &preference)
{
  sp_movie chosen_movie;
  double max = SIMILAR_MIN;
  for (auto &movie: user_rating)
  {
    if (movie.second == 0)
    {
      double res = similarity (movies[movie.first], preference);
      if (max < res)
      {
        max = res;
        chosen_movie = movie.first;
      }
    }
  }
  return chosen_movie;
}

sp_movie RecommenderSystem::recommend_by_content (const RSUser &user)
{
  rank_map rating_copy = user.get_ranks ();
  normalize (rating_copy);
  auto preference_vec = preference (rating_copy, _movie_features.begin()
  ->second.size(), _movie_features);
  return chosen_movie (_movie_features, rating_copy, preference_vec);
}

bool RecommenderSystem::sort_map(const pair<double, sp_movie> &a,const
pair<double, sp_movie> &b)
{
  return a.first >= b.first;
}

vector<pair<double,sp_movie>> RecommenderSystem::create_similarity_map
(movie_map &movies, const RSUser &user, const sp_movie &movie, int k){
  rank_map user_rankings = user.get_ranks();
  vector<pair<double,sp_movie>> all_similar_vec;
  for(auto &elem: user_rankings){
    if(elem.second != 0){
      double similarity_rate = similarity (movies[elem.first],

                  movies[movie]);
      std::pair<double, sp_movie> pair;
      pair = std::make_pair(similarity_rate, elem.first);
      all_similar_vec.push_back(pair);
    }
  }
  std::sort(all_similar_vec.begin(), all_similar_vec.end(), sort_map);
  auto start = all_similar_vec.begin() + 0;
  auto end = all_similar_vec.begin() + k ;
  vector<pair<double, sp_movie>> result(k );
  copy(start, end, result.begin());
  return result;
}

double RecommenderSystem::predict_movie_score(const RSUser &user, const
sp_movie &movie, int k){
  double prediction = 0.0;
  double divisor = 0.0;
  rank_map user_rankings = user.get_ranks();
  vector<pair<double, sp_movie>> result = create_similarity_map
      (_movie_features, user,movie, k);
  for(auto & it : result) {
    prediction += it.first * (user_rankings[it.second]);
    divisor += it.first;
  }
  prediction = prediction / divisor;
  return prediction;
}

sp_movie RecommenderSystem::recommend_by_cf(const RSUser& user, int k){
  double max = 0.0;
  sp_movie chosen_movie;
  for(auto &elem: user.get_ranks()){
    if(elem.second == 0){
      double prediction = predict_movie_score (user, elem.first, k);
      if(prediction >= max){
        max = prediction;
        chosen_movie = elem.first;
      }
    }
  }
  return chosen_movie;
}

sp_movie RecommenderSystem::get_movie (const std::string &name, int year){
  auto curr = std::make_shared<Movie>(name, year);
  auto result = _movie_features.find(curr);
  if(_movie_features.find(curr) == _movie_features.end()){
    return nullptr;
  }
  return (*result).first;
}

std::ostream & operator<<(std::ostream &output, const
RecommenderSystem &recommender_system){
  if(!output){
    return output;
  }
  for (const auto& movies: recommender_system._movie_features){
    output << *movies.first;
  }
  return output;
}

