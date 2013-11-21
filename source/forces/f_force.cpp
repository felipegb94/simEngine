/*
 * f_force.cpp
 *
 *  Created on: Nov 11, 2013
 *      Author: pipe
 */

#include "f_force.h"


f_force::f_force(const rapidjson::Value& d)
:	name(std::string(d["name"].GetString())),
 	type(std::string(d["type"].GetString())),
 	id(d["id"].GetDouble()),
 	bodyID(d["body1"].GetDouble())
{
	forces.zeros(3);
}
