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
arma::vec Solver::getPhi(std::vector<Body>* bodies,std::vector<c_constraint*> *constraints, double t, int numConstraints){
	int flags = 1;
	arma::vec phi = arma::zeros(numConstraints);
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
			c_absDist* absDist =  static_cast<c_absDist*>(constraints->at(i));
			int bodyID = (int) absDist->bodyID1;
			Body body = bodies->at(bodyID-1);
			double distPhi = getAbsDist(absDist,body,t,flags);
			phi(counter) = distPhi;
			counter++;
		}
		else if(type == "RevoluteJoint"){
			c_revJoint* revJoint =  static_cast<c_revJoint*>(constraints->at(i));
			int bodyID1 = (int) revJoint->bodyID1;
			int bodyID2 = (int) revJoint->bodyID2;

			Body body1 = bodies->at(bodyID1-1);
			Body body2 = bodies->at(bodyID2-1);

			arma::vec revJointPhi = getRevJoint(revJoint,body1,body2,t,flags);
			phi(counter) = revJointPhi(0);
			counter++;
			phi(counter) = revJointPhi(1);
			counter++;
		}
		else if(type == "TranslationalJoint"){
			c_transJoint* transJoint =  static_cast<c_transJoint*>(constraints->at(i));
			int bodyID1 = (int) transJoint->bodyID1;
			int bodyID2 = (int) transJoint->bodyID2;

			Body body1 = bodies->at(bodyID1-1);
			Body body2 = bodies->at(bodyID2-1);

			arma::vec transJointPhi = getTransJoint(transJoint,body1,body2,t,flags);
			phi(counter) = transJointPhi(0);
			counter++;
			phi(counter) = transJointPhi(1);
			counter++;
		}
		else if(type == "RelativeX"){
			c_relX* relX =  static_cast<c_relX*>(constraints->at(i));
			int bodyID1 = (int) relX->bodyID1;
			int bodyID2 = (int) relX->bodyID2;

			Body body1 = bodies->at(bodyID1-1);
			Body body2 = bodies->at(bodyID2-1);

			double relXPhi = getRelX(relX,body1,body2,t,flags);
			phi(counter) = relXPhi;
			counter++;

		}
		else if(type == "RelativeY"){
			c_relY* relY =  static_cast<c_relY*>(constraints->at(i));
			int bodyID1 = (int) relY->bodyID1;
			int bodyID2 = (int) relY->bodyID2;

			Body body1 = bodies->at(bodyID1-1);
			Body body2 = bodies->at(bodyID2-1);

			double relYPhi = getRelY(relY,body1,body2,t,flags);
			phi(counter) = relYPhi;
			counter++;
		}
		else if(type == "RelativeDistance"){
			c_relDist* relDist =  static_cast<c_relDist*>(constraints->at(i));
			int bodyID1 = (int) relDist->bodyID1;
			int bodyID2 = (int) relDist->bodyID2;

			Body body1 = bodies->at(bodyID1-1);
			Body body2 = bodies->at(bodyID2-1);

			double relDistPhi = getRelDist(relDist,body1,body2,t,flags);
			phi(counter) = relDistPhi;
			counter++;
		}
	}

	return phi;
}
arma::mat Solver::getJacobian(std::vector<Body>* bodies,std::vector<c_constraint*> *constraints, double t,int numConstraints){

	arma::mat phi_q = arma::zeros(numConstraints,bodies->size()*3);
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
				c_absDist* absDist =  static_cast<c_absDist*>(constraints->at(i));
				int bodyID = (int) absDist->bodyID1;
				Body body = bodies->at(bodyID-1);
				arma::rowvec distance_q = getAbsDist_jac(absDist,body,t);
				int col = body.start;
				int row = counter;
				phi_q(row,col) = distance_q(0);
				phi_q(row,col+1) = distance_q(1);
				phi_q(row,col+2) = distance_q(2);
				counter++;
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
			}
			else if(type == "TranslationalJoint"){
				c_transJoint* transJoint =  static_cast<c_transJoint*>(constraints->at(i));
				int bodyID1 = (int) transJoint->bodyID1;
				int bodyID2 = (int) transJoint->bodyID2;

				Body body1 = bodies->at(bodyID1-1);
				Body body2 = bodies->at(bodyID2-1);
				arma::mat transJointPhi_q = getTransJoint_jac(transJoint,body1,body2,t);

				int colFor1 = body1.start;
				int colFor2 = body2.start;
				int rowFor1 = counter;
				counter++;
				int rowFor2 = counter;
				counter++;

				phi_q(rowFor1,colFor1) = transJointPhi_q(0,0);
				phi_q(rowFor1,colFor1+1) = transJointPhi_q(0,1);
				phi_q(rowFor1,colFor1+2) = transJointPhi_q(0,2);

				phi_q(rowFor1,colFor2) = transJointPhi_q(0,3);
				phi_q(rowFor1,colFor2+1) = transJointPhi_q(0,4);
				phi_q(rowFor1,colFor2+2) = transJointPhi_q(0,5);

				phi_q(rowFor2,colFor1) = transJointPhi_q(1,0);
				phi_q(rowFor2,colFor1+1) = transJointPhi_q(1,1);
				phi_q(rowFor2,colFor1+2) = transJointPhi_q(1,2);

				phi_q(rowFor2,colFor2) = transJointPhi_q(1,3);
				phi_q(rowFor2,colFor2+1) = transJointPhi_q(1,4);
				phi_q(rowFor2,colFor2+2) = transJointPhi_q(1,5);

			}
			else if(type == "RelativeX"){
				c_relX* relX =  static_cast<c_relX*>(constraints->at(i));
				int bodyID1 = (int) relX->bodyID1;
				int bodyID2 = (int) relX->bodyID2;

				Body body1 = bodies->at(bodyID1-1);
				Body body2 = bodies->at(bodyID2-1);
				arma::rowvec relXPhi_q = getRelX_jac(relX,body1,body2,t);

				int colFor1 = body1.start;
				int colFor2 = body2.start;
				int row = counter;
				counter++;

				phi_q(row,colFor1) = relXPhi_q(0);
				phi_q(row,colFor1+1) = relXPhi_q(1);
				phi_q(row,colFor1+2) = relXPhi_q(2);

				phi_q(row,colFor2) = relXPhi_q(3);
				phi_q(row,colFor2+1) = relXPhi_q(4);
				phi_q(row,colFor2+2) = relXPhi_q(5);

			}
			else if(type == "RelativeY"){
				c_relY* relY =  static_cast<c_relY*>(constraints->at(i));
				int bodyID1 = (int) relY->bodyID1;
				int bodyID2 = (int) relY->bodyID2;

				Body body1 = bodies->at(bodyID1-1);
				Body body2 = bodies->at(bodyID2-1);
				arma::rowvec relYPhi_q = getRelY_jac(relY,body1,body2,t);

				int colFor1 = body1.start;
				int colFor2 = body2.start;
				int row = counter;
				counter++;

				phi_q(row,colFor1) = relYPhi_q(0);
				phi_q(row,colFor1+1) = relYPhi_q(1);
				phi_q(row,colFor1+2) = relYPhi_q(2);

				phi_q(row,colFor2) = relYPhi_q(3);
				phi_q(row,colFor2+1) = relYPhi_q(4);
				phi_q(row,colFor2+2) = relYPhi_q(5);

			}
			else if(type == "RelativeDistance"){
				c_relDist* relDist =  static_cast<c_relDist*>(constraints->at(i));
				int bodyID1 = (int) relDist->bodyID1;
				int bodyID2 = (int) relDist->bodyID2;

				Body body1 = bodies->at(bodyID1-1);
				Body body2 = bodies->at(bodyID2-1);
				arma::rowvec relDistPhi_q = getRelDist_jac(relDist,body1,body2,t);

				int colFor1 = body1.start;
				int colFor2 = body2.start;
				int row = counter;
				counter++;

				phi_q(row,colFor1) = relDistPhi_q(0);
				phi_q(row,colFor1+1) = relDistPhi_q(1);
				phi_q(row,colFor1+2) = relDistPhi_q(2);

				phi_q(row,colFor2) = relDistPhi_q(3);
				phi_q(row,colFor2+1) = relDistPhi_q(4);
				phi_q(row,colFor2+2) = relDistPhi_q(5);
			}
		}

	return phi_q;

}
arma::vec Solver::getNu(std::vector<Body>* bodies,std::vector<c_constraint*> *constraints, double t,int numConstraints){
	int flags = 2;
	arma::vec nu = arma::zeros(numConstraints);
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
			c_absDist* absDist =  static_cast<c_absDist*>(constraints->at(i));
			int bodyID = (int) absDist->bodyID1;
			Body body = bodies->at(bodyID-1);
			double distNu = getAbsDist(absDist,body,t,flags);
			nu(counter) = distNu;
			counter++;
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
		else if(type == "TranslationalJoint"){
			c_transJoint* transJoint =  static_cast<c_transJoint*>(constraints->at(i));
			int bodyID1 = (int) transJoint->bodyID1;
			int bodyID2 = (int) transJoint->bodyID2;

			Body body1 = bodies->at(bodyID1-1);
			Body body2 = bodies->at(bodyID2-1);

			arma::vec transJointNu = getTransJoint(transJoint,body1,body2,t,flags);
			nu(counter) = transJointNu(0);
			counter++;
			nu(counter) = transJointNu(1);
			counter++;
		}
		else if(type == "RelativeX"){
			c_relX* relX =  static_cast<c_relX*>(constraints->at(i));
			int bodyID1 = (int) relX->bodyID1;
			int bodyID2 = (int) relX->bodyID2;

			Body body1 = bodies->at(bodyID1-1);
			Body body2 = bodies->at(bodyID2-1);

			double relXNu = getRelX(relX,body1,body2,t,flags);
			nu(counter) = relXNu;
			counter++;

		}
		else if(type == "RelativeY"){
			c_relY* relY =  static_cast<c_relY*>(constraints->at(i));
			int bodyID1 = (int) relY->bodyID1;
			int bodyID2 = (int) relY->bodyID2;

			Body body1 = bodies->at(bodyID1-1);
			Body body2 = bodies->at(bodyID2-1);

			double relYNu = getRelY(relY,body1,body2,t,flags);
			nu(counter) = relYNu;
			counter++;
		}
		else if(type == "RelativeDistance"){
			c_relDist* relDist =  static_cast<c_relDist*>(constraints->at(i));
			int bodyID1 = (int) relDist->bodyID1;
			int bodyID2 = (int) relDist->bodyID2;

			Body body1 = bodies->at(bodyID1-1);
			Body body2 = bodies->at(bodyID2-1);

			double relDistNu = getRelDist(relDist,body1,body2,t,flags);
			nu(counter) = relDistNu;
			counter++;
		}
	}

	return nu;

}
arma::vec Solver::getGamma(std::vector<Body>* bodies,std::vector<c_constraint*> *constraints, double t,int numConstraints){
	int flags = 3;
	arma::vec gamma = arma::zeros(numConstraints);
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
			c_absDist* absDist =  static_cast<c_absDist*>(constraints->at(i));
			int bodyID = (int) absDist->bodyID1;
			Body body = bodies->at(bodyID-1);
			double distGamma = getAbsDist(absDist,body,t,flags);
			gamma(counter) = distGamma;
			counter++;
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
		else if(type == "TranslationalJoint"){
			c_transJoint* transJoint =  static_cast<c_transJoint*>(constraints->at(i));
			int bodyID1 = (int) transJoint->bodyID1;
			int bodyID2 = (int) transJoint->bodyID2;

			Body body1 = bodies->at(bodyID1-1);
			Body body2 = bodies->at(bodyID2-1);

			arma::vec transJointGamma = getTransJoint(transJoint,body1,body2,t,flags);
			gamma(counter) = transJointGamma(0);
			counter++;
			gamma(counter) = transJointGamma(1);
			counter++;
		}
		else if(type == "RelativeX"){
			c_relX* relX =  static_cast<c_relX*>(constraints->at(i));
			int bodyID1 = (int) relX->bodyID1;
			int bodyID2 = (int) relX->bodyID2;

			Body body1 = bodies->at(bodyID1-1);
			Body body2 = bodies->at(bodyID2-1);

			double relXGamma = getRelX(relX,body1,body2,t,flags);
			gamma(counter) = relXGamma;
			counter++;

		}
		else if(type == "RelativeY"){
			c_relY* relY =  static_cast<c_relY*>(constraints->at(i));
			int bodyID1 = (int) relY->bodyID1;
			int bodyID2 = (int) relY->bodyID2;

			Body body1 = bodies->at(bodyID1-1);
			Body body2 = bodies->at(bodyID2-1);

			double relYGamma = getRelY(relY,body1,body2,t,flags);
			gamma(counter) = relYGamma;
			counter++;
		}
		else if(type == "RelativeDistance"){
			c_relDist* relDist =  static_cast<c_relDist*>(constraints->at(i));
			int bodyID1 = (int) relDist->bodyID1;
			int bodyID2 = (int) relDist->bodyID2;

			Body body1 = bodies->at(bodyID1-1);
			Body body2 = bodies->at(bodyID2-1);

			double relDistGamma = getRelDist(relDist,body1,body2,t,flags);
			gamma(counter) = relDistGamma;
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
		xComponent = (((absX->sP1(0)*cosine) - absX->sP1(1)*sine)*anglePhi_d*anglePhi_d) + absX->c_ddFunction.eval(t);
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
		yComponent = ((absY->sP1(0)*sine + absY->sP1(1)*cosine)*anglePhi_d*anglePhi_d) + absY->c_ddFunction.eval(t);
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

double Solver::getAbsDist(c_absDist* absDist, Body body,double t,int flags){

	double distance;
	double x = body.getQ()(0);
	double y = body.getQ()(0);
	double anglePhi = body.getQ()(2);
	double sine = sin(anglePhi);
	double cosine = cos(anglePhi);
	double groundX = 0;
	double groundY = 0;

	if(flags == 1){
		//void R2DD_ConAbsDist::Phi()	{mPhi = pow((mI->mQ(0,0)+xBarP*cos(mI->mQ(2,0))-yBarP*sin(mI->mQ(2,0))-xGroundP),2)+pow((mI->mQ(1,0)+xBarP*sin(mI->mQ(2,0))+yBarP*cos(mI->mQ(2,0))-yGroundP),2)-pow(mF,2);}
		double xComponent = x + absDist->sP1(0)*cosine - absDist->sP1(1)*sine - groundX ;
		double yComponent = y + absDist->sP1(0)*sine + absDist->sP1(1)*cosine - groundY;

		distance =  pow(xComponent,2) + pow(yComponent,2) - pow(absDist->c_function.eval(t),2);
	}
	else if(flags == 2){
		distance = 0;
	}
	else if(flags == 3){
		distance = 0;
	}


	return distance;

}
double Solver::getRelX(c_relX* relX, Body body1, Body body2,double t, int flags){

	double relXComponent = 0;

	double x1 = body1.getQ()(0);
	double y1= body1.getQ()(1);
	double anglePhi1= body1.getQ()(2);
	double anglePhid1 = body1.getQd()(2);
	double anglePhidd1 = body1.getQdd()(2);
	arma::vec sP1 = relX->sP1;
	double cosine1 = cos(anglePhi1);
	double sine1 = sin(anglePhi1);

	double x2= body2.getQ()(0);
	double y2= body2.getQ()(1);
	double anglePhi2= body2.getQ()(2);
	double anglePhid2 = body2.getQd()(2);
	double anglePhidd2 = body2.getQdd()(2);
	arma::vec sP2 = relX->sP2;
	double cosine2 = cos(anglePhi2);
	double sine2 = sin(anglePhi2);

	if(flags == 1){
		relXComponent = x2 + sP2(0)*cosine2 - sP2(1)*sine2 - x1 - sP1(0)*cosine1 + sP1(1)*sine1 - relX->c_function.eval(t);
	}
	else if(flags == 2){
		relXComponent = relX->c_dFunction.eval(t);
	}
	else if(flags == 3){
		relXComponent = -1*(sP1(0)*cosine1 - sP1(1)*sine1)*anglePhid1*anglePhid1 + (sP2(0)*cosine2 - sP2(1)*sine2)*pow(anglePhid2,2) + relX->c_ddFunction.eval(t);
	}

	return relXComponent;
}
double Solver::getRelY(c_relY* relY, Body body1, Body body2,double t, int flags){

	double relYComponent = 0;

	double x1 = body1.getQ()(0);
	double y1= body1.getQ()(1);
	double anglePhi1= body1.getQ()(2);
	double anglePhid1 = body1.getQd()(2);
	double anglePhidd1 = body1.getQdd()(2);
	arma::vec sP1 = relY->sP1;
	double cosine1 = cos(anglePhi1);
	double sine1 = sin(anglePhi1);

	double x2= body2.getQ()(0);
	double y2= body2.getQ()(1);
	double anglePhi2= body2.getQ()(2);
	double anglePhid2 = body2.getQd()(2);
	double anglePhidd2 = body2.getQdd()(2);
	arma::vec sP2 = relY->sP2;
	double cosine2 = cos(anglePhi2);
	double sine2 = sin(anglePhi2);


	if(flags == 1){
		relYComponent = y2 + sP2(0)*sine2 + sP2(1)*cosine2 - y1 - sP1(0)*sine1 - sP1(1)*cosine1 - relY->c_dFunction.eval(t);
	}
	else if(flags == 2){
		relYComponent = relY->c_dFunction.eval(t);
	}
	else if(flags == 3){
		relYComponent = -1*(sP1(0)*sine1 + sP1(1)*cosine1)*pow(anglePhid1,2) + (sP2(0)*sine2 + sP2(1)*cosine2)*pow(anglePhid2,2) + relY->c_ddFunction.eval(t) ;
	}

	return relYComponent;
}
double Solver::getRelDist(c_relDist* relDist, Body body1, Body body2,double t, int flags){

	double relDistComponent = 0;

	double x1 = body1.getQ()(0);
	double y1= body1.getQ()(1);
	double anglePhi1= body1.getQ()(2);
	double anglePhid1 = body1.getQd()(2);
	double anglePhidd1 = body1.getQdd()(2);
	arma::vec sP1 = relDist->sP1;
	double cosine1 = cos(anglePhi1);
	double sine1 = sin(anglePhi1);

	double x2= body2.getQ()(0);
	double y2= body2.getQ()(1);
	double anglePhi2= body2.getQ()(2);
	double anglePhid2 = body2.getQd()(2);
	double anglePhidd2 = body2.getQdd()(2);
	arma::vec sP2 = relDist->sP2;
	double cosine2 = cos(anglePhi2);
	double sine2 = sin(anglePhi2);

	if(flags == 1){
		double xComponent = x1 + sP1(0)*cosine1 - sP1(1)*sine1 - x2 - sP2(0)*cosine2 + sP2(1)*sine2;
		double yComponent = y1 + sP1(0)*sine1 + sP1(1)*cosine1 - y2 - sP2(0)*sine2 - sP2(1)*cosine2;
		relDistComponent = pow(xComponent,2) + pow(yComponent,2) - pow(relDist->c_function.eval(t),2);
	}
	else if(flags == 2){
		relDistComponent = relDist->c_dFunction.eval(t);
	}
	else if(flags == 3){
		relDistComponent = 0;
	}

	return relDistComponent;
}

arma::vec Solver::getRevJoint(c_revJoint* revoluteJoint, Body body1, Body body2,double t, int flags){

	arma::vec revoluteComponents(2);

	double x1 = body1.getQ()(0);
	double y1= body1.getQ()(1);
	double anglePhi1= body1.getQ()(2);
	double anglePhid1 = body1.getQd()(2);
	double anglePhidd1 = body1.getQdd()(2);
	arma::vec sP1 = revoluteJoint->sP1;
	double cosine1 = cos(anglePhi1);
	double sine1 = sin(anglePhi1);

	double x2= body2.getQ()(0);
	double y2= body2.getQ()(1);
	double anglePhi2= body2.getQ()(2);
	double anglePhid2 = body2.getQd()(2);
	double anglePhidd2 = body2.getQdd()(2);
	arma::vec sP2 = revoluteJoint->sP2;
	double cosine2 = cos(anglePhi2);
	double sine2 = sin(anglePhi2);

	double fddot = revoluteJoint->c_ddFunction.eval(t);

	if(flags == 1){
		revoluteComponents(0) = (x1 + (sP1(0)*cosine1) - (sP1(1)*sine1)) - (x2 + (sP2(0)*cosine2) - (sP2(1)*sine2));
		revoluteComponents(1)= (y1 + (sP1(0)*sine1) + (sP1(1)*cosine1)) - (y2 + (sP2(0)*sine2) + (sP2(1)*cosine2));
	}
	else if(flags == 2){
		revoluteComponents(0) = 0;
		revoluteComponents(1) = 0;
	}
	else if(flags == 3){
		revoluteComponents(0) = (cosine1*sP1(0) - sine1*sP1(1))*anglePhid1*anglePhid1 - (cosine2*sP2(0) - sine2*sP2(1))*anglePhid2*anglePhid2;
		revoluteComponents(1) = (sine1*sP1(0) + cosine1*sP1(1))*anglePhid1*anglePhid1 - (sine2*sP2(0) + cosine2*sP2(1))*anglePhid2*anglePhid2;
	}

	return revoluteComponents;
}
arma::vec Solver::getTransJoint(c_transJoint* transJoint, Body body1, Body body2,double t, int flags){

	arma::vec transComponents(2);
	arma::vec r1 = arma::zeros(2);
	arma::vec r2 = arma::zeros(2);
	arma::mat R = arma::zeros(2,2);
	R(0,1) = -1;
	R(1,0) = 1;
	arma::mat R_t = R.t();

	r1(0) = body1.getQ()(0);
	r1(1) = body1.getQ()(1);
	double anglePhi1= body1.getQ()(2);
	double anglePhid1= body1.getQd()(2);
	double anglePhidd1= body1.getQdd()(2);

	arma::vec sP1 = transJoint->sP1;
	arma::vec vP1 = transJoint->vP1;
	arma::vec vP1_t = vP1.t();
	arma::mat A1 = body1.getA(anglePhi1);
	arma::mat A1_t = A1.t();
	arma::mat B1 = body1.getB(anglePhi1);
	arma::mat B1_t = B1.t();

	r2(0) = body2.getQ()(0);
	r2(1) = body2.getQ()(1);
	double anglePhi2= body2.getQ()(2);
	double anglePhid2= body2.getQd()(2);
	double anglePhidd2= body2.getQdd()(2);
	arma::vec sP2 = transJoint->sP2;
	arma::vec vP2 = transJoint->vP2;
	arma::vec vP2_t = vP2.t();
	arma::mat B2 = body2.getB(anglePhi2);
	arma::mat B2_t = B2.t();

	double anglePhi12 = anglePhi2 - anglePhi1;
	double anglePhid12 = anglePhid2 - anglePhid1;
	arma::vec r12 = r2 - r1;
	arma::mat B12 = body1.getB(anglePhi12);


	if(flags == 1){
		arma::vec v = vP1_t*B1_t*(r12) - vP1_t*B12*sP2 - vP1_t*R_t*sP1;
		transComponents(0) = v(0);
		v = -1*vP1_t*B12*vP2;
		transComponents(1)= v(0);
	}
	else if(flags == 2){
		transComponents(0) = 0;
		transComponents(1)= 0;
	}
	else if(flags == 3){
		arma::vec v = vP1_t*(B12*sP2*anglePhid12*anglePhid12 - B1_t*r12*anglePhid1*anglePhid1 - 2*A1_t*r12*anglePhid1);
		transComponents(0) = v(0);
		transComponents(1)= 0;
		transComponents = transComponents*-1;
	}

	return transComponents;
}


arma::rowvec Solver::getAbsX_jac(c_absX* absX, Body body,double t){

	arma::rowvec xComponents(3);
	double anglePhi = body.getQ()(2);

	xComponents(0) = 1;
	xComponents(1) = 0;
	xComponents(2) = (-1*absX->sP1(0)*sin(anglePhi)) - (absX->sP1(1)*cos(anglePhi));

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
arma::rowvec Solver::getAbsDist_jac(c_absDist* absDist, Body body,double t){
	arma::rowvec distance(3);
	double x = body.getQ()(0);
	double y = body.getQ()(1);
	double anglePhi = body.getQ()(2);
	double sine = sin(anglePhi);
	double cosine = cos(anglePhi);
	double groundX = 0;
	double groundY = 0;
	distance(0) = x + absDist->sP1(0)*cosine - groundX ;
	distance(1) = y + absDist->sP1(1)*sine - groundY ;
	distance(2) = -1*distance(0)*(absDist->sP1(0)*sine - absDist->sP1(1)*cosine) + distance(1)*(absDist->sP1(0)*cosine - absDist->sP1(1)*sine);
	return distance;
}
arma::rowvec Solver::getRelX_jac(c_relX* relX, Body body1, Body body2,double t){

	arma::rowvec relXComponents = arma::zeros(6);

	double anglePhi1= body1.getQ()(2);
	arma::vec sP1 = relX->sP1;
	double cosine1 = cos(anglePhi1);
	double sine1 = sin(anglePhi1);

	double anglePhi2= body2.getQ()(2);
	arma::vec sP2 = relX->sP2;
	double cosine2 = cos(anglePhi2);
	double sine2 = sin(anglePhi2);

	relXComponents(0) = -1;
	relXComponents(2) = sP1(0)*sine1 + sP1(1)*cosine1;

	relXComponents(3) = 1;
	relXComponents(5) = -1*sP2(0)*cosine2 - sP2(1)*sine2;



	return relXComponents;
}
arma::rowvec Solver::getRelY_jac(c_relY* relY, Body body1, Body body2,double t){

	arma::rowvec relYComponents= arma::zeros(6);

	double anglePhi1= body1.getQ()(2);
	arma::vec sP1 = relY->sP1;
	double cosine1 = cos(anglePhi1);
	double sine1 = sin(anglePhi1);


	double anglePhi2= body2.getQ()(2);
	arma::vec sP2 = relY->sP2;
	double cosine2 = cos(anglePhi2);
	double sine2 = sin(anglePhi2);

	relYComponents(1) = -1;
	relYComponents(2) = -sP1(0)*cosine1 + sP1(1)*sine1;

	relYComponents(4) = 1;
	relYComponents(5) = sP2(0)*cosine2 - sP2(1)*sine2;

	return relYComponents;
}
arma::rowvec Solver::getRelDist_jac(c_relDist* relDist, Body body1, Body body2,double t){

	arma::rowvec relDistComponents= arma::zeros(6);

	double x1 = body1.getQ()(0);
	double y1= body1.getQ()(1);
	double anglePhi1= body1.getQ()(2);
	double anglePhid1 = body1.getQd()(2);
	double anglePhidd1 = body1.getQdd()(2);
	arma::vec sP1 = relDist->sP1;
	double cosine1 = cos(anglePhi1);
	double sine1 = sin(anglePhi1);

	double x2= body2.getQ()(0);
	double y2= body2.getQ()(1);
	double anglePhi2= body2.getQ()(2);
	double anglePhid2 = body2.getQd()(2);
	double anglePhidd2 = body2.getQdd()(2);
	arma::vec sP2 = relDist->sP2;
	double cosine2 = cos(anglePhi2);
	double sine2 = sin(anglePhi2);

	return relDistComponents;
}


arma::mat Solver::getRevJoint_jac(c_revJoint* revoluteJoint, Body body1, Body body2,double t){
	arma::mat revJoint(2,6);
	double anglePhi1= body1.getQ()(2);
	double anglePhi2= body2.getQ()(2);
	double cosine1 = cos(anglePhi1);
	double sine1 = sin(anglePhi1);
	double cosine2 = cos(anglePhi2);
	double sine2 = sin(anglePhi2);

	revJoint(0,0) = 1;
	revJoint(0,1) = 0;
	revJoint(0,2) = (-1*revoluteJoint->sP1(0)*sine1) - (revoluteJoint->sP1(1)*cosine1);

	revJoint(0,3) = -1;
	revJoint(0,4) = 0;
	revJoint(0,5) = (revoluteJoint->sP2(0)*sine2) + (revoluteJoint->sP2(1)*cosine2);

	revJoint(1,0) = 0;
	revJoint(1,1) = 1;
	revJoint(1,2) = (revoluteJoint->sP1(0)*cosine1) - (revoluteJoint->sP1(1)*sine1);

	revJoint(1,3) = 0;
	revJoint(1,4) = -1;
	revJoint(1,5) = (-1*revoluteJoint->sP2(0)*cosine2) + (revoluteJoint->sP2(1)*sine2);

	return revJoint;
}
arma::mat Solver::getTransJoint_jac(c_transJoint* transJoint, Body body1, Body body2,double t){

	arma::mat transComponents(2,6);
	arma::vec r1 = arma::zeros(2);
	arma::vec r2 = arma::zeros(2);
	arma::mat R = arma::zeros(2,2);
	R(0,1) = -1;
	R(1,0) = 1;
	arma::mat R_t = R.t();

	r1(0) = body1.getQ()(0);
	r1(1) = body1.getQ()(1);
	double anglePhi1= body1.getQ()(2);
	double anglePhid1= body1.getQd()(2);
	double anglePhidd1= body1.getQdd()(2);

	arma::vec sP1 = transJoint->sP1;
	arma::vec vP1 = transJoint->vP1;
	arma::vec vP1_t = vP1.t();
	arma::mat A1 = body1.getA(anglePhi1);
	arma::mat A1_t = A1.t();
	arma::mat B1 = body1.getB(anglePhi1);
	arma::mat B1_t = B1.t();

	r2(0) = body2.getQ()(0);
	r2(1) = body2.getQ()(1);
	double anglePhi2= body2.getQ()(2);
	double anglePhid2= body2.getQd()(2);
	double anglePhidd2= body2.getQdd()(2);
	arma::vec sP2 = transJoint->sP2;
	arma::vec vP2 = transJoint->vP2;
	arma::vec vP2_t = vP2.t();
	arma::mat B2 = body2.getB(anglePhi2);
	arma::mat B2_t = B2.t();

	double anglePhi12 = anglePhi2 - anglePhi1;
	double anglePhid12 = anglePhid2 - anglePhid1;
	arma::vec r12 = r2 - r1;
	arma::mat A12 = body1.getA(anglePhi12);

	arma::vec temp = arma::zeros(2);

	temp = -1*vP1_t*B1_t;

	transComponents(0,0) = temp(0);
	transComponents(0,1) = temp(1);
	arma::vec v = -1*vP1_t*A1_t*r12 - vP1_t*A12*sP2;
	transComponents(0,2) = v(0);

	transComponents(1,0) = 0;
	transComponents(1,1) = 0;

	v = vP1_t*A12*vP2;
	transComponents(1,2) = v(0);

	temp = temp*-1;

	transComponents(0,3) = temp(0);
	transComponents(0,4) = temp(1);
	v = vP1_t*A12*sP2;
	transComponents(0,5) = v(0);

	transComponents(1,3) = 0;
	transComponents(1,4) = 0;
    v = vP1_t*A12*vP2;
	transComponents(1,5) = v(0);


	return transComponents;
}
