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

	const rapidjson::Value& b = d["Pground"];

	for(rapidjson::SizeType i = 0; i < b.Size(); i++){
			pGround.push_back(b[i].GetDouble());
		}

	const rapidjson::Value& b2 = d["sP1"];
	for(rapidjson::SizeType i = 0; i < b2.Size(); i++){
		sP1.push_back(b2[i].GetDouble());
	}
}
void c_absDist::setConstraint(const rapidjson::Value& d){


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

	std::cout << "Constraint pGround = [";

	for(std::vector<int>::size_type i = 0; i != pGround.size(); i++){
		std::cout << pGround[i];
		std::cout << ",";
	}
	std::cout<<"]"<<std::endl;

}

