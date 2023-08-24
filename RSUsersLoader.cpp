#include <vector>
#include <sstream>
#include <iostream>
#include <fstream>
#include "RSUsersLoader.h"
#include "RecommenderSystem.h"


using std::string;

std::vector<RSUser> RSUsersLoader::create_users_from_file (const
                                                           std::string &users_file_path, std::unique_ptr<RecommenderSystem> rs)
noexcept (false)
{
  std::vector<RSUser> users;
  std::vector<sp_movie> movies_years;
  rec_system shared = std::move (rs);
  std::ifstream users_stream;
  users_stream.open (users_file_path);
  if (!users_stream.is_open ())
  {
    throw std::runtime_error (Error1);
  }
  string line, first_line, movie_and_year;
  std::getline (users_stream, first_line);
  std::istringstream iss1 (first_line);
  while (iss1 >> movie_and_year)
  {
    int int_year;
    size_t where_dash = movie_and_year.find (DASH);
    string name = movie_and_year.substr (0, where_dash);
    string year = movie_and_year.substr (where_dash+1);
    std::istringstream iss3(year);
    iss3 >> int_year;
    movies_years.push_back(shared->get_movie (name, int_year));
  }
  while (getline (users_stream, line))
  {
    rank_map user_ranking(0, sp_movie_hash, sp_movie_equal);
    string user_name;
    string rate;
    std::istringstream iss2 (line);
    iss2 >> user_name;
    int ind = 0;
    while (iss2 >> rate)
    {
      if(rate == "NA"){
        user_ranking[movies_years[ind]] = 0;
        ind++;
      }
      else{
        double int_rate = std::stod (rate);
        if(int_rate < MIN|| int_rate > MAX){
          throw std::runtime_error( Error2);
        }
        user_ranking[movies_years[ind]] = int_rate;
        ind++;


      }

    }
    RSUser new_user(user_name, user_ranking, shared);
    users.push_back (new_user);
  }
  users_stream.close ();
  return users;
}



