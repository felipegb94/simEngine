/*
 * f_rsda.cpp
 *
 *  Created on: Dec 16, 2013
 *      Author: pipe
 */

#include "f_force.h"
f_rsda::f_rsda(const rapidjson::Value& d)
:	f_force(d),
	bodyID1(d["body1"].GetDouble()),
	bodyID2(d["body2"].GetDouble()),
	c(d["c"].GetDouble()),
	k(d["k"].GetDouble()),
	theta0(d["theta0"].GetDouble())
{
	anglePhiIJ = 0;
	anglePhidIJ = 0;
	h.setFunction(std::string(d["h"].GetString()));

	//torqueFun.setFunction(std::string(d["fun"].GetString()));
}

void f_rsda::print(){

	f_force::print();

	std::cout << "Force RSDA body id: ";
	std::cout << bodyID1<< "and "<< bodyID2 << std::endl;

}
void f_rsda::updateForce(double time,double anglePhi){
	forces(0) = 0;
	forces(1) = 0;
	forces(2) = 0;
}
void f_rsda::updateForce(double time, double phiI, double phidI,double phiJ ,double phidJ){
	//Ground will be body 2
	anglePhiIJ = phiJ - phiI;
	anglePhidIJ = phidJ - phidI;

	forces(0) = 0;
	forces(1) = 0;
	forces(2) = k*(anglePhiIJ - theta0) + c*anglePhidIJ + h.eval(time);

}
arma::vec f_rsda::getForce(double time){
	return forces;
}
