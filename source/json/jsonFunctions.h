/*
 * jsonFunctions.h
 *
 *  Created on: Nov 6, 2013
 *      Author: pipe
 */

#ifndef JSONFUNCTIONS_H_
#define JSONFUNCTIONS_H_
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"
#include <iostream>
#include <string>
#include <stdlib.h>

#include <fstream>
using namespace rapidjson;
class jsonFunctions
{
public:
	jsonFunctions(){};
	void  save(rapidjson::Document& doc);


};


#endif /* JSONFUNCTIONS_H_ */
