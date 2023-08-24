#include "RecommenderSystemLoader.h"
 #include <string>
#include <fstream>

std::unique_ptr<RecommenderSystem> RecommenderSystemLoader::create_rs_from_movies_file(const
std::string&movies_file_path)noexcept(false){
   std::unique_ptr<RecommenderSystem> recommender_system =
       std::make_unique<RecommenderSystem>();
  vector<double> features;
  std::ifstream movies_stream;
  movies_stream.open(movies_file_path);
  if (!movies_stream.is_open())
  {
    throw std::runtime_error(Error1);
  }
  string line;
  while (std::getline(movies_stream, line))
  {
    std::istringstream iss(line);
    string title;
    iss >> title;
    int feature;
    while (iss >> feature)
    {
      if(feature < MIN || feature > MAX){
        throw std::runtime_error(Error2);
      }
      features.push_back(feature);
    }
    int int_year;
    size_t where_dash = title.find (DASH);
    string name = title.substr (0, where_dash);
    string year = title.substr (where_dash+1);
    std::istringstream iss3(year);
    iss3 >> int_year;
    sp_movie movie = recommender_system->add_movie (name, int_year, features);
    features.clear();
  }
  movies_stream.close();
  return recommender_system;
}



