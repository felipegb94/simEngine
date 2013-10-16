/*
 * c_transJoint.cpp
 *
 *  Created on: Oct 1, 2013
 *      Author: pipe
 */


#include "c_constraint.h"
#include <iostream>

c_transJoint::c_transJoint(const rapidjson::Value& d)
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

	const rapidjson::Value& b3 = d["vP1"];
	std::vector<double> vP1Temp;

	for(rapidjson::SizeType i = 0; i < b3.Size(); i++){
		vP1Temp.push_back(b3[i].GetDouble());
	}
	vP1 = vP1Temp;

	const rapidjson::Value& b4 = d["vP2"];
	std::vector<double> vP2Temp;

	for(rapidjson::SizeType i = 0; i < b4.Size(); i++){
		vP2Temp.push_back(b4[i].GetDouble());
	}
	vP2 = vP2Temp;
}
void c_transJoint::setConstraint(const rapidjson::Value& d){

}
void c_transJoint::print(){
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
