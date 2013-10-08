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
 	xGround(d["xGround"].GetDouble()),
	bodyID1(d["body1"].GetDouble())

{
	const rapidjson::Value& b = d["sP1"];

	for(rapidjson::SizeType i = 0; i < b.Size(); i++){
		sP1.push_back(b[i].GetDouble());
	}
}
void c_absX::setConstraint(const rapidjson::Value& d){

}
void c_absX::print(){

	c_constraint::print();

	std::cout << "Constraint xGround = ";
	std::cout << xGround << std::endl;
	std::cout << "Constraint BodyID1: ";
	std::cout << bodyID1<<std::endl;
	std::cout << "Constraint sP1 = [";

	for(std::vector<int>::size_type i = 0; i != sP1.size(); i++){
		std::cout << sP1[i];
		std::cout << ",";
	}
	std::cout<<"]"<<std::endl;



}


