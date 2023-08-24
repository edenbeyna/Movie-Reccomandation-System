//
// Created by 24565 on 2/24/2022.
//

#ifndef EX5_RECOMMENDERSYSTEMLOADER_H
#define EX5_RECOMMENDERSYSTEMLOADER_H

#include "RecommenderSystem.h"
#define Error1 "could not open file"
#define Error2 "invalid values"
#define MIN 1
#define MAX 10
#define DASH '-'


class RecommenderSystemLoader
{

private:


public:
    RecommenderSystemLoader() = delete;
    /**
	 * loads movies by the given format for movies with their feature's score
	 * @param movies_file_path a path to the file of the movies
	 * @return shared pointer to a RecommenderSystem which was created with those movies
	 */
    static std::unique_ptr<RecommenderSystem> create_rs_from_movies_file(const std::string&
    movies_file_path)
    noexcept(false);
};


#endif //EX5_RECOMMENDERSYSTEMLOADER_H
