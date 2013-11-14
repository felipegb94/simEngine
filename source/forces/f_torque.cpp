/*
 * f_torque.cpp
 *
 *  Created on: Nov 11, 2013
 *      Author: pipe
 */
#include "f_force.h"
f_torque::f_torque(const rapidjson::Value& d)
:	f_force(d),
	bodyID1(d["body"].GetDouble())
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
