/*
 * model.h
 *
 *  Created on: Sep 23, 2013
 *      Author: pipe
 */

#ifndef MODEL_H_
#define MODEL_H_
#include <vector>
#include <string>
#include "Body.h"
#include "c_constraint.h"
#include <armadillo>
typedef rapidjson::GenericDocument<rapidjson::UTF8<> > MyJsonDocument;

using namespace arma;

class Model{
private:

	std::string name;
	std::vector<Body> bodies;
	std::vector<c_constraint*> constraints;
	std::string simulationType;
	double tEnd;
	double outputSteps;
	double stepSize;

	arma::vec phi; //Vector of kinematic and driver constraints
	arma::mat phi_q; //Jacobian
	arma::vec nu; //RHS of the velocity linear system
	arma::vec gamma; //RHS of the acceleration linear system

public:
	vec gravity;
	Model(MyJsonDocument& d);
	//input could be a json file?
	const std::vector<Body>& getBodies(){return bodies;}
	const std::vector<c_constraint*>& getConstraints(){return constraints;}
	std::string getName(){return name;}

	/**
	 * Information parsed from acf input file
	 */
	void setSimulationType(std::string type){simulationType = type;}
	void setTEnd(double tend){tEnd = tend;}
	void setOutputSteps(double outputSteps){this->outputSteps = outputSteps;}
	void setStepSize(double stepSize){this->stepSize = stepSize;}

};
#endif /* MODEL_H_ */
