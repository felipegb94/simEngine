/*
 * f_force.h
 *
 *  Created on: Nov 11, 2013
 *      Author: pipe
 */


#ifndef F_FORCE_H_
#define F_FORCE_H_

#include <sstream>
#include <string>
#include "Function.h"
#include "rapidjson/document.h"
#include <iostream>
#include <armadillo>

class f_force{

public:
	std::string name;
	std::string type;
	arma::vec forces;
	double id;
	double bodyID;


	f_force(const rapidjson::Value& d);

	virtual ~f_force(){};
	virtual arma::vec getForce() = 0;
	virtual void updateForce(double time, double anglePhi) = 0;
	virtual void updateForce(double time, double x,double xDot) = 0;



	virtual void print() = 0 ;

};

inline void f_force::print(){
	std::cout << "Printing Force Element" << std::endl;
	std::cout << "Force ID = ";
	std::cout << id << std::endl;
	std::cout << "Force type = ";
	std::cout << type << std::endl;
	std::cout << "Constraint name = ";
	std::cout << name << std::endl;
}



class f_pointForce:public f_force{
public:
	arma::vec sP1;
	double bodyID1;
	std::string frame;
	Function xComponentFun;
	Function yComponentFun;

	f_pointForce(const rapidjson::Value& d);
	~f_pointForce(){}
	virtual void print();
	void toGRF();
	virtual void updateForce(double time, double anglePhi);
	virtual void updateForce(double time, double x, double xDot);

	virtual arma::vec getForce();



};

class f_forceFile:public f_force{
public:
	arma::vec sP1;
	double bodyID1;
	std::string frame;
	Function function1;
	Function function2;
	Function function3;

	f_forceFile(const rapidjson::Value& d);
	~f_forceFile(){}
	virtual void print();
	void toGRF();
	virtual void updateForce(double time, double anglePhi);
	virtual void updateForce(double time, double x,double xDot);
	virtual arma::vec getForce();

};

class f_torque:public f_force{
public:
	double bodyID1;
	Function torqueFun;

	f_torque(const rapidjson::Value& d);
	~f_torque(){}
	virtual void print();
	virtual void updateForce(double time, double anglePhi);
	virtual void updateForce(double time, double x, double xDot);

	virtual arma::vec getForce();

};

#endif /* F_FORCE_H_ */
