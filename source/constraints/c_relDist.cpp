/*
 * c_relDist.cpp
 *
 *  Created on: Oct 1, 2013
 *      Author: pipe
 */


#include "c_constraint.h"
#include <iostream>

c_relDist::c_relDist(const rapidjson::Value& d)
:	c_constraint(d),
 	bodyID1(d["body1"].GetDouble()),
 	bodyID2(d["body2"].GetDouble())
{

	const rapidjson::Value& b = d["sP1"];
	for(rapidjson::SizeType i = 0; i < b.Size(); i++){
		sP1.push_back(b[i].GetDouble());
	}
	const rapidjson::Value& b2 = d["sP2"];
	for(rapidjson::SizeType i = 0; i < b2.Size(); i++){
		sP2.push_back(b2[i].GetDouble());
	}
}
void c_relDist::setConstraint(const rapidjson::Value& d){

}
void c_relDist::print(){
	c_constraint::print();
	std::cout << "Constraint BodyID1:";
	std::cout << bodyID1<<std::endl;
	std::cout << "Constraint sP1 = [";
	for(std::vector<int>::size_type i = 0; i != sP1.size(); i++){
		std::cout << sP1[i];
		std::cout << ",";
	}
	std::cout<<"]"<<std::endl;

	std::cout << "Constraint BodyID2:";
	std::cout << bodyID2<<std::endl;
	std::cout << "Constraint sP2 = [";
	for(std::vector<int>::size_type i = 0; i != sP2.size(); i++){
		std::cout << sP2[i];
		std::cout << ",";
	}
	std::cout<<"]"<<std::endl;



}
