/*
 * f_pointForce.cpp
 *
 *  Created on: Nov 11, 2013
 *      Author: pipe
 */

#include "f_force.h"

f_pointForce::f_pointForce(const rapidjson::Value& d)
:	f_force(d),
	frame(std::string(d["frame"].GetString()))
{
	const rapidjson::Value& b = d["sP1"];
	std::vector<double> sP1Temp;
	for(rapidjson::SizeType i = 0; i < b.Size(); i++){
		sP1Temp.push_back(b[i].GetDouble());
	}
	sP1 = sP1Temp;

	xComponentFun.setFunction(std::string(d["funX"].GetString()));
	yComponentFun.setFunction(std::string(d["funY"].GetString()));
	if(frame.compare("LRF") == 0){
		cout <<  "Changing to GRF " <<  endl;
		toGRF();
	}

}

void f_pointForce::toGRF(){


	std::string oldXFun = xComponentFun.getString();
	std::string oldYFun = yComponentFun.getString();

	std::string newXFun = "("+oldXFun + "*"+"cos(phi)"+") - " + "("+oldYFun + "*"+"sin(phi)"+")";
	std::cout << newXFun <<std::endl;
	xComponentFun.setFunction(newXFun);

	std::string newYFun = "("+oldXFun + "*"+"sin(phi)"+") + " + "("+oldYFun + "*"+"cos(phi)"+")";
	std::cout << newYFun <<std::endl;
	yComponentFun.setFunction(newYFun);

}
void f_pointForce::updateForce(double time, double anglePhi){
	double sine = sin(anglePhi);
	double cosine = cos(anglePhi);
	forces(0) = xComponentFun.eval(time,anglePhi);
	forces(1) = yComponentFun.eval(time,anglePhi);
	forces(2) = sP1(0)*(forces(0)*(-1*sine) + forces(1)*cosine) - sP1(1)*(forces(0)*cosine + forces(1)*sine);

}
arma::vec f_pointForce::getForce(){
	return forces;
}

void f_pointForce::print(){

	f_force::print();

	std::cout << "Force body id: ";
	std::cout << bodyID1<<std::endl;
	std::cout << "Force sP1 = [";
	for(std::vector<int>::size_type i = 0; i != sP1.size(); i++){
		std::cout << sP1[i];
		std::cout << ",";
	}
	std::cout<<"]"<<std::endl;
	std::cout << "Frame: " << frame << std::endl;
	std::cout << "Force xComponent =  ";
	xComponentFun.print();
	std::cout << "Force yComponent =  ";
	yComponentFun.print();


}
