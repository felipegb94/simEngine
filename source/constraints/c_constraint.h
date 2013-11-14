/*
 * c_constraint.h
 *
 *  Created on: Sep 26, 2013
 *      Author: pipe
 */

#ifndef C_CONSTRAINT_H_
#define C_CONSTRAINT_H_


#include <string>
#include "Function.h"
#include "rapidjson/document.h"
#include <iostream>
#include <armadillo>
typedef rapidjson::GenericDocument<rapidjson::UTF8<> > MyJsonDocument;

class c_constraint{

public:
	std::string name;
	std::string type;
	double id;
	Function c_function;
	Function c_dFunction;
	Function c_ddFunction;



	c_constraint(const rapidjson::Value& d);

	virtual ~c_constraint(){};

	virtual void print() = 0 ;

};

inline void c_constraint::print(){
	std::cout << "Virtual print" << std::endl;
	std::cout << "Constraint ID = ";
	std::cout << id << std::endl;
	std::cout << "Constraint type = ";
	std::cout << type << std::endl;
	std::cout << "Constraint name = ";
	std::cout << name << std::endl;

	std::cout << "Constraint Function = ";
	c_function.print() ;
	std::cout << "Constraint Function Derivative = ";
	c_dFunction.print() ;
	std::cout << "Constraint Function Second Derivative = ";
	c_ddFunction.print() ;
}


class c_absX:public c_constraint{
public:
	arma::vec sP1;
	double bodyID1;

	c_absX(const rapidjson::Value& d);
	~c_absX(){}
	void setConstraint(const rapidjson::Value& d);
	virtual void print();

};

class c_absY:public c_constraint{
public:
	arma::vec sP1;
	double bodyID1;


	c_absY(const rapidjson::Value& d);
	~c_absY(){}

	void setConstraint(const rapidjson::Value& d);
	virtual void print();

};

class c_absAngle:public c_constraint{
public:

	c_absAngle(const rapidjson::Value& d);
	~c_absAngle(){}
	double bodyID1;
	virtual void print();

};

class c_absDist:public c_constraint{
public:
	arma::vec sP1;
	arma::vec pGround;
	double bodyID1;


	c_absDist(const rapidjson::Value& d);
	~c_absDist(){}

	void setConstraint(const rapidjson::Value& d);
	virtual void print();


};
class c_relX:public c_constraint{
public:
	arma::vec sP1;
	arma::vec sP2;
	double bodyID1;
	double bodyID2;


	c_relX(const rapidjson::Value& d);
	~c_relX(){}

	void setConstraint(const rapidjson::Value& d);
	virtual void print();

};

class c_relY:public c_constraint{
public:
	arma::vec sP1;
	arma::vec sP2;
	double bodyID1;
	double bodyID2;


	c_relY(const rapidjson::Value& d);
	~c_relY(){}

	void setConstraint(const rapidjson::Value& d);
	virtual void print();

};
class c_relDist:public c_constraint{
public:
	arma::vec sP1;
	arma::vec sP2;
	double bodyID1;
	double bodyID2;


	c_relDist(const rapidjson::Value& d);
	~c_relDist(){}

	void setConstraint(const rapidjson::Value& d);
	virtual void print();

};
class c_revJoint:public c_constraint{
public:
	arma::vec sP1;
	arma::vec sP2;
	double bodyID1;
	double bodyID2;


	c_revJoint(const rapidjson::Value& d);
	~c_revJoint(){}

	void setConstraint(const rapidjson::Value& d);
	virtual void print();

};
class c_transJoint:public c_constraint{
public:
	arma::vec sP1;
	arma::vec sP2;
	double bodyID1;
	double bodyID2;
	arma::vec vP1;
	arma::vec vP2;



	c_transJoint(const rapidjson::Value& d);
	~c_transJoint(){}

	void setConstraint(const rapidjson::Value& d);
	virtual void print();

};



#endif /* C_CONSTRAINT_H_ */
