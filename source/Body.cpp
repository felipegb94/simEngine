/*
 * Body.cpp
 *
 *  Created on: Sep 23, 2013
 *      Author: pipe
 */
#include <vector>
#include <string>
#include <iostream>

#include "../includes/Body.h"
#include "../includes/rapidjson/document.h"


Body::Body(const rapidjson::Value& d){
	setBody(d);
	//cout<<"print";
}


void Body::setBody(const rapidjson::Value& d){

	name = string(d["name"].GetString());
	id = d["id"].GetDouble();
	jbar = d["jbar"].GetDouble();
	mass = d["mass"].GetDouble();
	//std::cout << d["q0"].GetType()<<std::endl;
	//std::cout << d["q0"].GetArray()<<std::endl;
	const rapidjson::Value& b = d["q0"];
	const rapidjson::Value& b2 = d["qd0"];

	for(rapidjson::SizeType i = 0; i < b.Size(); i++){
		q0.push_back(b[i].GetDouble());
		qd0.push_back(b2[i].GetDouble());
	}


}
void Body::print(){
	std::cout << "Printing body." <<std::endl;
	std::cout << "Body ID = ";
	std::cout << id << std::endl;
	std::cout << "Body name = ";
	std::cout << name << std::endl;
	std::cout << "Body mass = ";
	std::cout << mass << std::endl;
	std::cout << "Body jbar = ";
	std::cout << jbar << std::endl;
	std::cout << "Body q0 = [";

	for(std::vector<int>::size_type i = 0; i != q0.size(); i++){
		std::cout << q0[i];
		std::cout << ",";
	}
	std::cout<<"]"<<endl;
	std::cout << "Body qd0 = [";
	for(std::vector<int>::size_type i = 0; i != qd0.size(); i++){
			std::cout << qd0[i];
			std::cout << ",";
		}
	std::cout << "]" <<endl;


}
