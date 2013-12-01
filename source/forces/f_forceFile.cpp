/*
 * f_forceFile.cpp
 *
 *  Created on: Nov 30, 2013
 *      Author: pipe
 */

#include "f_force.h"

f_forceFile::f_forceFile(const rapidjson::Value& d)
:	f_force(d),
	frame(std::string(d["frame"].GetString()))
{
	const rapidjson::Value& b = d["sP1"];
	std::vector<double> sP1Temp;
	for(rapidjson::SizeType i = 0; i < b.Size(); i++){
		sP1Temp.push_back(b[i].GetDouble());
	}
	sP1 = sP1Temp;

	function1.setFunction(std::string(d["function1"].GetString()));
	function2.setFunction(std::string(d["function2"].GetString()));
	function3.setFunction(std::string(d["function3"].GetString()));

	if(frame.compare("LRF") == 0){
		cout <<  "Changing to GRF " <<  endl;
		toGRF();
	}

}

void f_forceFile::toGRF(){

/**
	std::string oldXFun = xComponentFun.getString();
	std::string oldYFun = yComponentFun.getString();

	std::string newXFun = "("+oldXFun + "*"+"cos(phi)"+") - " + "("+oldYFun + "*"+"sin(phi)"+")";
	std::cout << newXFun <<std::endl;
	xComponentFun.setFunction(newXFun);

	std::string newYFun = "("+oldXFun + "*"+"sin(phi)"+") + " + "("+oldYFun + "*"+"cos(phi)"+")";
	std::cout << newYFun <<std::endl;
	yComponentFun.setFunction(newYFun);
	*/

}
void f_forceFile::updateForce(double time,double anglePhi){
	forces(0) = 0;
	forces(1) = 0;
	forces(2) = 0;
	//std::cout << "wrong updateFORCE!!!!!"<<std::endl;
}
void f_forceFile::updateForce(double time, double x, double xDot){
	if(xDot <= 0){

		forces(0) = 0;
	}
	else if((x <= 5)){

		forces(0) = function2.evalX(time,x);
	}
	else if(x <= 5.5){
		std::cout <<"x = "<< x << std::endl;
		forces(0) = function3.evalX(time,x);
		std::cout <<"force(0) = "<< forces(0) << std::endl;

	}
	else{
		forces(0) = 0;
	}

	forces(1) = 0;
	forces(2) = 0;

}
arma::vec f_forceFile::getForce(){
	return forces;
}

void f_forceFile::print(){

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
	std::cout << "Force function1 =  ";
	function1.print();
	std::cout << "Force function2 =  ";
	function2.print();
	std::cout << "Force function3 =  ";
	function3.print();


}
