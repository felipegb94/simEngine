/*
 * c_absDist.cpp
 *
 *  Created on: Sep 26, 2013
 *      Author: pipe
 */

#include "c_constraint.h"
#include <iostream>
c_absDist::c_absDist(const rapidjson::Value& d)
:	c_constraint(d),
 	bodyID1(d["body1"].GetDouble())
{


	const rapidjson::Value& b2 = d["sP1"];
	std::vector<double> sP1Temp;

	for(rapidjson::SizeType i = 0; i < b2.Size(); i++){
		sP1Temp.push_back(b2[i].GetDouble());
	}
	sP1 = sP1Temp;
	std::cout << sP1 << std::cout;
}

void c_absDist::print(){
	c_constraint::print();
	std::cout << "Constraint BodyID1:";
	std::cout << bodyID1<<std::endl;
	std::cout << "Constraint sP1 = [";
	for(std::vector<int>::size_type i = 0; i != sP1.size(); i++){
		std::cout << sP1[i];
		std::cout << ",";
	}
	std::cout<<"]"<<std::endl;


}

