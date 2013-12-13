/*
 * f_torque.cpp
 *
 *  Created on: Nov 11, 2013
 *      Author: pipe
 */
#include "f_force.h"
f_torque::f_torque(const rapidjson::Value& d)
:	f_force(d),
	bodyID1(d["body1"].GetDouble())
{
	torqueFun.setFunction(std::string(d["fun"].GetString()));
}

void f_torque::print(){

	f_force::print();

	std::cout << "Force Torque body id: ";
	std::cout << bodyID1<<std::endl;

	std::cout << "Torque function =  ";
	torqueFun.print();
}
void f_torque::updateForce(double time,double anglePhi){
	forces(0) = 0;
	forces(1) = 0;
	forces(2) = torqueFun.eval(time);
}
void f_torque::updateForce(double time, double x, double xDot){


}
arma::vec f_torque::getForce(double time){
	return forces;
}
