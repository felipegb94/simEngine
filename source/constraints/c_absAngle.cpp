/*
 * c_absAngle.cpp
 *
 *  Created on: Sep 26, 2013
 *      Author: pipe
 */
#include "../includes/c_constraint.h"
#include <iostream>
c_absAngle::c_absAngle(const rapidjson::Value& d)
:	c_constraint(d),
 	bodyID1(d["body1"].GetDouble())
{

}

void c_absAngle::print(){
	c_constraint::print();
	std::cout << "Constraint bodyID1 = ";
	std::cout << bodyID1 << std::endl;
}
