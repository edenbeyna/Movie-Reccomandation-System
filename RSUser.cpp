
#include "RSUser.h"
#include "RecommenderSystem.h"

RSUser::RSUser (const std::string &username, rank_map &user_rankings,
                rec_system &recommendation){
  _username = username;
  _user_rankings = user_rankings;
  _recommendation = recommendation;
}

const std::string& RSUser::get_name() const {
  return _username;
}

rank_map RSUser::get_ranks() const {
  return _user_rankings;
}

void RSUser::add_movie_to_rs(const std::string &name, int year,
                     const std::vector<double> &features,
                     double rate){
  sp_movie added_movie = _recommendation->add_movie (name,year,features);
  _user_rankings[added_movie] = rate;

}

sp_movie RSUser::get_recommendation_by_content() const{
  return _recommendation->recommend_by_content (*this);

}

double RSUser:: get_prediction_score_for_movie(const std::string& name,
                                               int year, int k) const
{
  return _recommendation->predict_movie_score (*this,
                                                   _recommendation->get_movie (name, year)
                                                   ,k);
}

sp_movie RSUser:: get_recommendation_by_cf(int k) const
{
  return _recommendation->recommend_by_cf (*this,k);
}

std::ostream &operator<<(std::ostream &output, const RSUser& user){
  if(!output){
    return output;
  }
  output << "name " << user._username << std::endl;
  output << *user._recommendation;
  return output;
}



// implement your cpp code here
