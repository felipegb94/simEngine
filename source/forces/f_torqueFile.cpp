/*
 * f_torqueFile.cpp
 *
 *  Created on: Dec 12, 2013
 *      Author: pipe
 */

#include "f_force.h"
#include <fstream>
#include <sstream>
#include <string>
f_torqueFile::f_torqueFile(const rapidjson::Value& d)
:	f_force(d),
	bodyID1(d["body1"].GetDouble())
{

	std::string timestr;
	std::string torquestr;
	torqueFileName=std::string(d["filename"].GetString())+".res";
	std::ifstream torqueFile(torqueFileName.c_str());
	std::string line;
	if (torqueFile.is_open())
	  {
	    std::getline(torqueFile,timestr);
	    std::getline(torqueFile,torquestr);
	    torqueFile.close();
	  }
	  else std::cout << "Unable to open file";
	std::stringstream ssin1(timestr);
	double tmp = 0;
    while (ssin1.good()){
        ssin1 >> tmp;
        time.push_back(tmp);
    }
	std::stringstream ssin2(torquestr);
    while (ssin2.good()){
        ssin2 >> tmp;
        torqueVal.push_back(tmp);
    }


}

void f_torqueFile::print(){

	f_force::print();

	std::cout << "Force Torque body id: ";
	std::cout << bodyID1<<std::endl;

	std::cout << "Torque file =  " << torqueFileName << std::endl;
}

void f_torqueFile::updateForce(double time,double anglePhi){
	forces(0) = 0;
	forces(1) = 0;
	int index = (int)(time/0.01);
	forces(2) = torqueVal.at(index);
	std::cout << "torqueFile =" << index << std::endl;
}
void f_torqueFile::updateForce(double time, double x, double xDot){

}

arma::vec f_torqueFile::getForce(double time){
	return forces;
}
