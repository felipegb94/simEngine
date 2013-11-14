/*
 * f_pointForce.cpp
 *
 *  Created on: Nov 11, 2013
 *      Author: pipe
 */

#include "f_force.h"

f_pointForce::f_pointForce(const rapidjson::Value& d)
:	f_force(d),
	bodyID1(d["body1"].GetDouble()),
	frame(std::string(d["frame"].GetString()))
{
	const rapidjson::Value& b = d["sP1"];
	std::vector<double> sP1Temp;
	for(rapidjson::SizeType i = 0; i < b.Size(); i++){
		sP1Temp.push_back(b[i].GetDouble());
	}
	sP1 = sP1Temp;

	const rapidjson::Value& fun = d["fun"];
	rapidjson::SizeType index = 0;
	xComponentFun.setFunction(std::string(fun[index].GetString()));
	index = 1;
	yComponentFun.setFunction(std::string(fun[index].GetString()));
}

void f_pointForce::toGRF(double anglePhi){

	std::stringstream s1;
	std::stringstream s2;


	double cosine = cos(anglePhi);
	s1 << cosine;
	std::string cosineStr = s1.str();
	double sine = sin(anglePhi);
	s2 << sine;
	std::string sineStr = s2.str();
	std::string oldXFun = xComponentFun.getString();
	std::string oldYFun = yComponentFun.getString();

	std::string newXFun = "("+oldXFun + "*"+cosineStr+") - " + "("+oldYFun + "*"+sineStr+")";
	std::cout << newXFun <<std::endl;
	xComponentFun.setFunction(newXFun);

	std::string newYFun = "("+oldXFun + "*"+sineStr+") + " + "("+oldYFun + "*"+cosineStr+")";
	std::cout << newYFun <<std::endl;
	yComponentFun.setFunction(newYFun);

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
