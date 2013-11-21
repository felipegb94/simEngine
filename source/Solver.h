/*
 * Solver.h
 *
 *  Created on: Oct 15, 2013
 *      Author: pipe
 */

#ifndef SOLVER_H_
#define SOLVER_H_

#include <armadillo>
#include <iostream>
#include "c_constraint.h"
#include "Body.h"
#include "Function.h"

class Solver{
public:
	Solver();
	arma::vec getPhi(std::vector<Body>* bodies,std::vector<c_constraint*> *constraints, double t,int numConstraints);
	arma::mat getJacobian(std::vector<Body>* bodies,std::vector<c_constraint*> *constraints, double t,int numConstraints);
	arma::vec getNu(std::vector<Body>* bodies,std::vector<c_constraint*> *constraints, double t,int numConstraints);
	arma::vec getGamma(std::vector<Body>* bodies,std::vector<c_constraint*> *constraints, double t,int numConstraints);

private:

	double getAbsX(c_absX* absX, Body body,double t,int flags);
	double getAbsY(c_absY* absY, Body body,double t,int flags);
	double getAbsAngle(c_absAngle* absAngle, Body body,double t,int flags);
	arma::vec getRevJoint(c_revJoint* revoluteJoint, Body body1, Body body2,double t, int flags);
	arma::rowvec getAbsX_jac(c_absX* absX, Body body,double t);
	arma::rowvec getAbsY_jac(c_absY* absY, Body body,double t);
	arma::rowvec getAbsAngle_jac(c_absAngle* absAngle, Body body,double t);
	arma::mat getRevJoint_jac(c_revJoint* revoluteJoint, Body body1, Body body2,double t);
};

#endif /* SOLVER_H_ */
