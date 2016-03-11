/*
 * Reading.hpp
 *
 *  Created on: 11/03/2016
 *      Author: claus
 */

#ifndef READING_HPP_
#define READING_HPP_

#include <string>
#include <utility>

class Reading {
private:
    std::pair<std::string, std::string> _measuredValue {};

public:
    Reading(std::string, std::string);
    std::pair<std::string, std::string> measuredValue();
    virtual ~Reading();
};

#endif /* READING_HPP_ */
