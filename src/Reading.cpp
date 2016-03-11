/*
 * Reading.cpp
 *
 *  Created on: 11/03/2016
 *      Author: claus
 */

#include "Reading.hpp"

Reading::Reading(std::string k, std::string v) : _measuredValue(k, v) {}

Reading::~Reading() {}

std::pair<std::string, std::string> Reading::measuredValue() {
    return _measuredValue;
}
