/*
 * c_relY.cpp
 *
 *  Created on: Oct 1, 2013
 *      Author: pipe
 */

#include "c_constraint.h"
#include <iostream>

c_relY::c_relY(const rapidjson::Value& d)
:	c_constraint(d),
 	bodyID1(d["body1"].GetDouble()),
 	bodyID2(d["body2"].GetDouble())
{

	const rapidjson::Value& b = d["sP1"];
	std::vector<double> sP1Temp;

	for(rapidjson::SizeType i = 0; i < b.Size(); i++){
		sP1Temp.push_back(b[i].GetDouble());
	}
	sP1 = sP1Temp;

	const rapidjson::Value& b2 = d["sP2"];
	std::vector<double> sP2Temp;

	for(rapidjson::SizeType i = 0; i < b2.Size(); i++){
		sP2Temp.push_back(b2[i].GetDouble());
	}
	sP2 = sP2Temp;
}
void c_relY::setConstraint(const rapidjson::Value& d){

}
void c_relY::print(){
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
