/*
 * c_constraint.cpp
 *
 *  Created on: Sep 26, 2013
 *      Author: pipe
 */
#include "../includes/c_constraint.h"
#include <iostream>
#include "../includes/rapidjson/document.h"
#include "../includes/Function.h"
#include <string>


c_constraint::c_constraint(const rapidjson::Value& d)
:	name(std::string(d["name"].GetString())),
 	type(std::string(d["type"].GetString())),
 	id(d["id"].GetDouble())
{
	c_function.setFunction(std::string(d["fun"].GetString()));
}
void c_constraint::setConstraint(const rapidjson::Value& d){



}







