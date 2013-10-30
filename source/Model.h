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
#include "Solver.h"
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

	arma::vec qCurr;
	arma::vec qdCurr;
	arma::vec qddCurr;
	arma::vec phiCurr;
	arma::mat phi_qCurr;
	arma::vec nu;
	arma::vec gamma;

	std::vector<arma::vec> q_list;
	std::vector<arma::vec> qd_list;
	std::vector<arma::vec> qdd_list;


	std::vector<arma::vec> phi_list; //List of Vectors of kinematic and driver constraints at each time
	std::vector<arma::mat> phi_q_list; //List of the Jacobian at each time
	std::vector<arma::vec> nu_list; //List of RHS of the velocity linear system at each time
	std::vector<arma::vec> gamma_list; //List of RHS of the acceleration linear system at each time

public:
	vec gravity;
	double t;

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

	double getOutputSteps(){return this->outputSteps;}
	double getStepSize(){return this->stepSize;}
	const std::vector<arma::vec>& getQList(){return q_list;}
	const std::vector<arma::vec>& getQdList(){return qd_list;}
	const std::vector<arma::vec>& getQddList(){return qdd_list;}



	/**
	 * Solvers
	 */
	void solve();

};
#endif /* MODEL_H_ */
