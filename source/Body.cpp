/*
 * Body.cpp
 *
 *  Created on: Sep 23, 2013
 *      Author: pipe
 */
#include <vector>
#include <string>
#include <iostream>

#include "Body.h"
#include "rapidjson/document.h"


Body::Body(const rapidjson::Value& d){
	setBody(d);
	//cout<<"print";
}


void Body::setBody(const rapidjson::Value& d){

	name = string(d["name"].GetString());
	id = d["id"].GetDouble();
	jbar = d["jbar"].GetDouble();
	mass = d["mass"].GetDouble();

	const rapidjson::Value& b = d["q0"];
	const rapidjson::Value& b2 = d["qd0"];
	vector<double> q0Temp;
	vector<double> qd0Temp;

	for(rapidjson::SizeType i = 0; i < b.Size(); i++){
		q0Temp.push_back(b[i].GetDouble());
		qd0Temp.push_back(b2[i].GetDouble());
	}
	q = q0Temp;
	qd = qd0Temp;
	qdd = arma::zeros(3);
	A.zeros(2,2);
	B.zeros(2,2);
	start = id + 2*(id -1) - 1;


}

arma::mat Body::getA(){
	A(0,0) = cos(q(2));
	A(0,1) = -sin(q(2));
	A(1,0) = sin(q(2));
	A(1,1) = cos(q(2));
	return A;
}
arma::mat Body::getA(double anglePhi){
	B(0,0)=-sin(anglePhi);
	B(0,1)=-cos(anglePhi);
	B(1,0)=cos(anglePhi);
	B(1,1)=-sin(anglePhi);
	return B;
}
arma::mat Body::getB(){
	B(0,0)=-sin(q(2));
	B(0,1)=-cos(q(2));
	B(1,0)=cos(q(2));
	B(1,1) = -sin(q(2));
	return B;
}

arma::mat Body::getB(double anglePhi){
	B(0,0)=-sin(anglePhi);
	B(0,1)=-cos(anglePhi);
	B(1,0)=cos(anglePhi);
	B(1,1)=-sin(anglePhi);
	return B;
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

	for(std::vector<int>::size_type i = 0; i != q.size(); i++){
		std::cout << q[i];
		std::cout << ",";
	}
	std::cout<<"]"<<endl;
	std::cout << "Body qd0 = [";
	for(std::vector<int>::size_type i = 0; i != qd.size(); i++){
			std::cout << qd[i];
			std::cout << ",";
		}
	std::cout << "]" <<endl;


}
