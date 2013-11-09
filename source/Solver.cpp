/*
 * Solver.cpp
 *
 *  Created on: Oct 15, 2013
 *      Author: pipe
 */
#include "Solver.h"
#include "constraints/c_constraint.h"
#include "Function.h"

Solver::Solver(){
	std::cout << "creating a solver" <<std::endl;

}
arma::vec Solver::getPhi(std::vector<Body>* bodies,std::vector<c_constraint*> *constraints, double t){
	int flags = 1;
	arma::vec phi = arma::zeros(bodies->size()*3);
	int counter = 0;

	for(std::vector<int>::size_type i = 0; i < constraints->size();i++){
		string type = constraints->at(i)->type;

		if(type == "AbsoluteX"){
			c_absX* absX =  static_cast<c_absX*>(constraints->at(i));
			int bodyID = (int) absX->bodyID1;
			Body body = bodies->at(bodyID-1);
			double xPhi = getAbsX(absX,body,t,flags);
			phi(counter) = xPhi;
			counter++;

		}
		else if(type == "AbsoluteY"){
			c_absY* absY =  static_cast<c_absY*>(constraints->at(i));
			int bodyID = (int) absY->bodyID1;
			Body body = bodies->at(bodyID-1);
			double yPhi = getAbsY(absY,body,t,flags);
			phi(counter) = yPhi;
			counter++;
		}
		else if(type == "AbsoluteAngle"){
			c_absAngle* absAngle =  static_cast<c_absAngle*>(constraints->at(i));
			int bodyID = (int) absAngle->bodyID1;
			Body body = bodies->at(bodyID-1);
			double anglePhi = getAbsAngle(absAngle,body,t,flags);
			phi(counter) = anglePhi;
			counter++;

		}
		else if(type == "AbsoluteDistance"){

		}
		else if(type == "RevoluteJoint"){
			c_revJoint* revJoint =  static_cast<c_revJoint*>(constraints->at(i));
			int bodyID1 = (int) revJoint->bodyID1;
			int bodyID2 = (int) revJoint->bodyID2;

			Body body1 = bodies->at(bodyID1-1);
			Body body2 = bodies->at(bodyID2-1);

			arma::vec revJointPhi = getRevJoint(revJoint,body1,body2,t,flags);
			phi(counter) = revJointPhi(0);
			phi(counter) = revJointPhi(1);
			counter = counter+2;

		}
	}

	//std::cout << phi << std::endl;
	return phi;
}
arma::mat Solver::getJacobian(std::vector<Body>* bodies,std::vector<c_constraint*> *constraints, double t){

	arma::mat phi_q = arma::zeros(bodies->size()*3,bodies->size()*3);
	int counter = 0;

	for(std::vector<int>::size_type i = 0; i < constraints->size();i++){
			string type = constraints->at(i)->type;

			if(type == "AbsoluteX"){
				c_absX* absX =  static_cast<c_absX*>(constraints->at(i));
				int bodyID = (int) absX->bodyID1;
				Body body = bodies->at(bodyID-1);
				arma::rowvec xPhi_q = getAbsX_jac(absX,body,t);
				int col = body.start;
				int row = counter;
				phi_q(row,col) = xPhi_q(0);
				phi_q(row,col+1) = xPhi_q(1);
				phi_q(row,col+2) = xPhi_q(2);
				counter++;

			}
			else if(type == "AbsoluteY"){
				c_absY* absY =  static_cast<c_absY*>(constraints->at(i));
				int bodyID = (int) absY->bodyID1;
				Body body = bodies->at(bodyID-1);
				arma::rowvec yPhi_q = getAbsY_jac(absY,body,t);
				int col = body.start;
				int row = counter;
				phi_q(row,col) = yPhi_q(0);
				phi_q(row,col+1) = yPhi_q(1);
				phi_q(row,col+2) = yPhi_q(2);
				counter++;

			}
			else if(type == "AbsoluteAngle"){
				c_absAngle* absAngle =  static_cast<c_absAngle*>(constraints->at(i));
				int bodyID = (int) absAngle->bodyID1;
				Body body = bodies->at(bodyID-1);
				arma::rowvec anglePhi_q = getAbsAngle_jac(absAngle,body,t);
				int col = body.start;
				int row = counter;
				phi_q(row,col) = anglePhi_q(0);
				phi_q(row,col+1) = anglePhi_q(1);
				phi_q(row,col+2) = anglePhi_q(2);
				counter++;
			}
			else if(type == "AbsoluteDistance"){

			}
			else if(type == "RevoluteJoint"){
				c_revJoint* revJoint =  static_cast<c_revJoint*>(constraints->at(i));
				int bodyID1 = (int) revJoint->bodyID1;
				int bodyID2 = (int) revJoint->bodyID2;

				Body body1 = bodies->at(bodyID1-1);
				Body body2 = bodies->at(bodyID2-1);
				arma::mat revJointPhi_q = getRevJoint_jac(revJoint,body1,body2,t);

				int colFor1 = body1.start;
				int colFor2 = body2.start;
				int rowFor1 = counter;
				counter++;
				int rowFor2 = counter;
				counter++;

				phi_q(rowFor1,colFor1) = revJointPhi_q(0,0);
				phi_q(rowFor1,colFor1+1) = revJointPhi_q(0,1);
				phi_q(rowFor1,colFor1+2) = revJointPhi_q(0,2);

				phi_q(rowFor1,colFor2) = revJointPhi_q(0,3);
				phi_q(rowFor1,colFor2+1) = revJointPhi_q(0,4);
				phi_q(rowFor1,colFor2+2) = revJointPhi_q(0,5);

				phi_q(rowFor2,colFor1) = revJointPhi_q(1,0);
				phi_q(rowFor2,colFor1+1) = revJointPhi_q(1,1);
				phi_q(rowFor2,colFor1+2) = revJointPhi_q(1,2);

				phi_q(rowFor2,colFor2) = revJointPhi_q(1,3);
				phi_q(rowFor2,colFor2+1) = revJointPhi_q(1,4);
				phi_q(rowFor2,colFor2+2) = revJointPhi_q(1,5);

				//phi_q(rowFor1,colFor1) = revJointPhi_q(0,0);
				//phi_q(rowFor1,colFor1+1) = revJointPhi_q(1);
				//phi_q(rowFor1,colFor1+2) = revJointPhi_q(2);
			}
		}

	return phi_q;

}
arma::vec Solver::getNu(std::vector<Body>* bodies,std::vector<c_constraint*> *constraints, double t){
	int flags = 2;
	arma::vec nu = arma::zeros(bodies->size()*3);
	int counter = 0;

	for(std::vector<int>::size_type i = 0; i < constraints->size();i++){
		string type = constraints->at(i)->type;

		if(type == "AbsoluteX"){
			c_absX* absX =  static_cast<c_absX*>(constraints->at(i));
			int bodyID = (int) absX->bodyID1;
			Body body = bodies->at(bodyID-1);
			double xNu = getAbsX(absX,body,t,flags);
			nu(counter) =xNu;
			counter++;


		}
		else if(type == "AbsoluteY"){
			c_absY* absY =  static_cast<c_absY*>(constraints->at(i));
			int bodyID = (int) absY->bodyID1;
			Body body = bodies->at(bodyID-1);
			double yNu = getAbsY(absY,body,t,flags);
            nu(counter) = yNu;
			counter++;

		}
		else if(type == "AbsoluteAngle"){
			c_absAngle* absAngle =  static_cast<c_absAngle*>(constraints->at(i));
			int bodyID = (int) absAngle->bodyID1;
			Body body = bodies->at(bodyID-1);
			double angleNu = getAbsAngle(absAngle,body,t,flags);
            nu(counter) = angleNu;
			counter++;


		}
		else if(type == "AbsoluteDistance"){

		}
		else if(type == "RevoluteJoint"){
			c_revJoint* revJoint =  static_cast<c_revJoint*>(constraints->at(i));
			int bodyID1 = (int) revJoint->bodyID1;
			int bodyID2 = (int) revJoint->bodyID2;

			Body body1 = bodies->at(bodyID1-1);
			Body body2 = bodies->at(bodyID2-1);

			arma::vec revJointNu = getRevJoint(revJoint,body1,body2,t,flags);
            nu(counter) = revJointNu(0);
			counter++;

            nu(counter) = revJointNu(1);
			counter++;

		}
	}

	return nu;

}
arma::vec Solver::getGamma(std::vector<Body>* bodies,std::vector<c_constraint*> *constraints, double t){
	int flags = 3;
	arma::vec gamma = arma::zeros(bodies->size()*3);
	int counter = 0;
	for(std::vector<int>::size_type i = 0; i < constraints->size();i++){
		string type = constraints->at(i)->type;

		if(type == "AbsoluteX"){
			c_absX* absX =  static_cast<c_absX*>(constraints->at(i));
			int bodyID = (int) absX->bodyID1;
			Body body = bodies->at(bodyID-1);
			double xGamma = getAbsX(absX,body,t,flags);
			gamma(counter) = xGamma;
			counter++;

		}
		else if(type == "AbsoluteY"){
			c_absY* absY =  static_cast<c_absY*>(constraints->at(i));
			int bodyID = (int) absY->bodyID1;
			Body body = bodies->at(bodyID-1);
			double yGamma = getAbsY(absY,body,t,flags);
			gamma(counter) = yGamma;
			counter++;

		}
		else if(type == "AbsoluteAngle"){
			c_absAngle* absAngle =  static_cast<c_absAngle*>(constraints->at(i));
			int bodyID = (int) absAngle->bodyID1;
			Body body = bodies->at(bodyID-1);
			double angleGamma = getAbsAngle(absAngle,body,t,flags);
			gamma(counter) = angleGamma;
			counter++;

		}
		else if(type == "AbsoluteDistance"){

		}
		else if(type == "RevoluteJoint"){
			c_revJoint* revJoint =  static_cast<c_revJoint*>(constraints->at(i));
			int bodyID1 = (int) revJoint->bodyID1;
			int bodyID2 = (int) revJoint->bodyID2;

			Body body1 = bodies->at(bodyID1-1);
			Body body2 = bodies->at(bodyID2-1);

			arma::vec revJointGamma = getRevJoint(revJoint,body1,body2,t,flags);
			gamma(counter) = revJointGamma(0);
			counter++;
			gamma(counter) = revJointGamma(1);
			counter++;
		}
	}
	return gamma;

}

double Solver::getAbsX(c_absX* absX, Body body,double t, int flags){
	double xComponent;
	double x = body.getQ()(0);
	double anglePhi = body.getQ()(2);
	double anglePhi_d = body.getQd()(2);
	double cosine = cos(anglePhi);
	double sine = sin(anglePhi);

	if(flags == 1){
		xComponent = x + (absX->sP1(0)*cosine) - (absX->sP1(1)*sine) - (absX->c_function.eval(t));
	}
	else if(flags == 2){
		xComponent = absX->c_dFunction.eval(t);
	}
	else if(flags == 3){

		xComponent = (((absX->sP1(0)*cosine) - absX->sP1(1)*sine)*pow(anglePhi_d,2)) + absX->c_ddFunction.eval(t);
	}



	return xComponent;
}
double Solver::getAbsY(c_absY* absY, Body body,double t,int flags){

	double yComponent;
	double y = body.getQ()(1);
	double anglePhi = body.getQ()(2);
	double anglePhi_d = body.getQd()(2); //6.28
	double cosine = cos(anglePhi);
	double sine = sin(anglePhi);

	if(flags == 1){
		yComponent = y + (absY->sP1(0)*sine) + (absY->sP1(1)*cosine) - (absY->c_function.eval(t));

	}
	else if(flags == 2){
		yComponent = absY->c_dFunction.eval(t);

	}
	else if(flags == 3){

		yComponent = (((absY->sP1(0)*sine) + absY->sP1(1)*cosine)*pow(anglePhi_d,2)) + absY->c_ddFunction.eval(t);
	}


	return yComponent;

}
double Solver::getAbsAngle(c_absAngle* absAngle, Body body,double t,int flags){

	double angleComponent;
	double anglePhi = body.getQ()(2);

	if(flags == 1){
		angleComponent = anglePhi - absAngle->c_function.eval(t) ;
	}
	else if(flags == 2){
		angleComponent = absAngle->c_dFunction.eval(t);

	}
	else if(flags == 3){

		angleComponent = absAngle->c_ddFunction.eval(t);
	}



	return angleComponent;

}

arma::vec Solver::getRevJoint(c_revJoint* revoluteJoint, Body body1, Body body2,double t, int flags){

	arma::vec revoluteComponents(2);

	double x1 = body1.getQ()(0);
	double y1= body1.getQ()(1);
	double anglePhi1= body1.getQ()(2);
	arma::vec sP1 = revoluteJoint->sP1;
	double cosine1 = cos(anglePhi1);
	double sine1 = sin(anglePhi1);

	double x2= body2.getQ()(0);
	double y2= body2.getQ()(1);
	double anglePhi2= body2.getQ()(2);
	arma::vec sP2 = revoluteJoint->sP2;
	double cosine2 = cos(anglePhi2);
	double sine2 = sin(anglePhi2);

	if(flags == 1){
		revoluteComponents(0) = (x1 + (sP1(0)*cosine1) - (sP1(1)*sine1)) -
				(x2 + (sP2(0)*cosine2) - (sP2(1)*sine2));
		revoluteComponents(1)= (y1 + (sP1(0)*sine1) + (sP1(1)*cosine1)) -
				(y2 + (sP2(0)*sine2) - (sP2(1)*cosine2));
	}
	else if(flags == 2){
		revoluteComponents(0) = 0;
		revoluteComponents(1) = 0;
	}
	else if(flags == 3){
		revoluteComponents(0) = 0;
		revoluteComponents(1) = 0;
	}

	return revoluteComponents;
}

arma::rowvec Solver::getAbsX_jac(c_absX* absX, Body body,double t){

	arma::rowvec xComponents(3);
	double anglePhi = body.getQ()(2);

	xComponents(0) = 1;
	xComponents(1) = 0;
	xComponents(2) = (-1)*(absX->sP1(0)*sin(anglePhi)) - (absX->sP1(1)*cos(anglePhi));

	return xComponents;
}
arma::rowvec Solver::getAbsY_jac(c_absY* absY, Body body,double t){
	arma::rowvec yComponents(3);
	double anglePhi = body.getQ()(2);

	yComponents(0) = 0;
	yComponents(1) = 1;
	yComponents(2) = (absY->sP1(0)*cos(anglePhi)) - (absY->sP1(1)*sin(anglePhi));

	return yComponents;
}
arma::rowvec Solver::getAbsAngle_jac(c_absAngle* absAngle, Body body,double t){
	arma::rowvec angleComponents(3);
	angleComponents(0) = 0;
	angleComponents(1) = 0;
	angleComponents(2) = 1;


	return angleComponents;
}
arma::mat Solver::getRevJoint_jac(c_revJoint* revoluteJoint, Body body1, Body body2,double t){
	arma::mat revJoint(2,6);
	double anglePhi1= body1.getQ()(2);
	double anglePhi2= body2.getQ()(2);
	double cosine1 = cos(anglePhi1);
	double sine1 = sin(anglePhi1);
	double cosine2 = cos(anglePhi1);
	double sine2 = sin(anglePhi1);

	revJoint(0,0) = 1;
	revJoint(0,1) = 0;
	revJoint(0,2) = (-1)*(revoluteJoint->sP1(0)*sine1) - (revoluteJoint->sP1(1)*cosine1);
	revJoint(0,3) = -1;
	revJoint(0,4) = 0;
	revJoint(0,5) = (revoluteJoint->sP2(0)*sine2) + (revoluteJoint->sP2(1)*cosine2);

	revJoint(1,0) = 0;
	revJoint(1,1) = 1;
	revJoint(1,2) = (revoluteJoint->sP1(0)*cosine1) - (revoluteJoint->sP1(1)*sine1);
	revJoint(1,3) = 0;
	revJoint(1,4) = -1;
	revJoint(1,5) = (-1)*(revoluteJoint->sP2(0)*cosine2) + (revoluteJoint->sP2(1)*sine2);

	return revJoint;
}
