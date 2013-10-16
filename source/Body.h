/*
 * body.h
 *
 *  Created on: Sep 23, 2013
 *      Author: pipe
 */

#ifndef BODY_H_
#define BODY_H_

#include "rapidjson/document.h"
#include <vector>
#include <string>
#include <armadillo>
using namespace std;

typedef rapidjson::GenericDocument<rapidjson::UTF8<> > MyJsonDocument;

class Body
{

private:
	std::string name;
	double id;
	double mass;
	double jbar;
	arma::vec q0;
	arma::vec qd0;

public:
	Body(const rapidjson::Value& d);

	//Getters
	std::string getName(){return name;}
	double getID(){return id;}
	double getMass(){return mass;}
	double getJBar(){return jbar;}
	arma::vec getQ0(){return q0;}
	arma::vec getQd0(){return qd0;}

	//Setters

	void setBody( const rapidjson::Value& d);
	void setName(string name){this->name = name;}
	void setID(double id){this->id = id;}
	void setMass(double mass){this->mass = mass;}
	void setJBar(double jbar){this->jbar = jbar;}
	void setQ0(arma::vec q0){this->q0 = q0;}
	void setQd0(arma::vec qd0){this->qd0 = qd0;}

	//Helper

	void print();


};

#endif /* BODY_H_ */
