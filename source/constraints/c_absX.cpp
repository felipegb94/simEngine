/*
 * c_absX.cpp
 *
 *  Created on: Sep 26, 2013
 *      Author: pipe
 */
#include "c_constraint.h"
#include <iostream>
c_absX::c_absX(const rapidjson::Value& d)
:	c_constraint(d),
	bodyID1(d["body1"].GetDouble())
{
	c_constraint::numBodies = 1;
	const rapidjson::Value& b = d["sP1"];
	std::vector<double> sP1Temp;
	for(rapidjson::SizeType i = 0; i < b.Size(); i++){
		sP1Temp.push_back(b[i].GetDouble());
	}
	sP1 = sP1Temp;
}

void c_absX::print(){

	c_constraint::print();


	std::cout << "Constraint BodyID1: ";
	std::cout << bodyID1<<std::endl;
	std::cout << "Constraint sP1 = [";

	for(std::vector<int>::size_type i = 0; i != sP1.size(); i++){
		std::cout << sP1[i];
		std::cout << ",";
	}
	std::cout<<"]"<<std::endl;



}


