/*
 * jsonParser.h
 *
 *  Created on: Sep 23, 2013
 *      Author: pipe
 */

#ifndef JSONPARSER_H_
#define JSONPARSER_H_
#include "rapidjson/document.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

rapidjson::Document parseJSON(std::string filename);

class jsonParser
{
public:

};

#endif /* JSONPARSER_H_ */
