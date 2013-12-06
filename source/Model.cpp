/*
 * model.cpp
 *
 *  Created on: Sep 23, 2013
 *      Author: pipe
 */
#include "Body.h"
#include <string>
#include <vector>
#include <iostream>
#include "c_constraint.h"
#include "rapidjson/document.h"
#include "Model.h"
#include "jsonParser.h"
#include <armadillo>


Model::Model(MyJsonDocument& d, std::string type, 	double tend,double outputsteps,double stepsize)
:gravity("9.81 0"),
 name(d["name"].GetString()),
 t(0.0),
 BETA(0.3025),
 GAMMA(0.6),
 numConstraints(0),
 simulationType(type),
 tEnd(tend),
 outputSteps(outputsteps),
 stepSize(stepsize)
{

	simulationSteps = tEnd/stepSize;
	std::cout << gravity << std::endl;
	std::cout << "Creating a model" << endl;
	std::cout << "Model Name: " + name << std::endl;

	const rapidjson::Value& jsonBodies = d["bodies"];

	for(int i = 0; i < jsonBodies.Size(); i++){
		//MyJsonDocument& d2 = b[i];
		const rapidjson::Value& v = jsonBodies[i]; //Getting Body json

		Body b(v); //Creating body
		bodies.push_back(b);
	}

	const rapidjson::Value& jsonConstraints = d["constraints"];
	for(int i = 0; i < jsonConstraints.Size(); i++){
		const rapidjson::Value& v = jsonConstraints[i];
		std::cout << "iteration" << i << std::endl;

		if(std::string(v["type"].GetString()) == "AbsoluteX"){
			//c_absX cX(v);

			constraints.push_back(new c_absX(v));
			numConstraints++;
		}
		else if(std::string(v["type"].GetString()) == "AbsoluteY"){
			//c_absY cY(v);

			constraints.push_back(new c_absY(v));
			numConstraints++;

		}
		else if(std::string(v["type"].GetString()) == "AbsoluteAngle"){
			//c_absAngle cAngle(v);

			constraints.push_back(new c_absAngle(v));
			numConstraints++;

		}
		else if(std::string(v["type"].GetString()) == "AbsoluteDistance"){
			//   c_absDist cDist(v);
			constraints.push_back(new c_absDist(v));
			numConstraints++;
			numConstraints++;

		}
		else if(std::string(v["type"].GetString()) == "RelativeX"){
			constraints.push_back(new c_relX(v));
			numConstraints++;
			numConstraints++;

		}
		else if(std::string(v["type"].GetString()) == "RelativeY"){
			constraints.push_back(new c_relY(v));
			numConstraints++;
			numConstraints++;
		}
		else if(std::string(v["type"].GetString()) == "RelativeDistance"){
			constraints.push_back(new c_relDist(v));
			numConstraints++;
			numConstraints++;

		}
		else if(std::string(v["type"].GetString()) == "RevoluteJoint"){
			constraints.push_back(new c_revJoint(v));
			numConstraints++;
			numConstraints++;

		}
		else if(std::string(v["type"].GetString()) == "TranslationalJoint"){
			constraints.push_back(new c_transJoint(v));
		}

	}
	if(simulationType.compare("Dynamics") == 0){
		const rapidjson::Value& jsonForces = d["forces"];
		std::cout << "here" <<std::endl;
		for(int i = 0; i < jsonForces.Size(); i++){
			const rapidjson::Value& v = jsonForces[i];
			std::cout << "iteration" << i << std::endl;
			if(std::string(v["type"].GetString()) == "PointForce"){
				forces.push_back(new f_pointForce(v));
			}
			else if(std::string(v["type"].GetString()) == "Torque"){
				forces.push_back(new f_torque(v));
			}
			else if(std::string(v["type"].GetString()) == "forceFile"){
				forces.push_back(new f_forceFile(v));

			}
		}
	}
	std::cout << "here"<<std::endl;

	//forces.at(1)->print();

	//Initialize matrices and vectors
	qCurr.set_size(bodies.size()*3);
	qdCurr.set_size(bodies.size()*3);
	qddCurr.set_size(bodies.size()*3);
	phiCurr.set_size(numConstraints);
	phi_qCurr.zeros(numConstraints,bodies.size()*3);
	nu.set_size(numConstraints);
	gamma.set_size(numConstraints);
	QACurr.zeros(bodies.size()*3);
	M.zeros(bodies.size()*3,bodies.size()*3);
	lambdaCurr.zeros(numConstraints);
	QCCurr.zeros(numConstraints*3);
	t = 0;
	//Set initial conditions and mass matrix
	setInitCond();
	updateM();

}
void Model::solveK(){
	std::cout << "Solving " + name << std::endl;
	int maxIterations = 30;
	Solver solve;
	double tolerance = pow(10.0,-8.0);

	double goal = 0.37;

	for(int i = 0; i <= simulationSteps;i++){
		bool converge = false;
		for(int j = 0; j < maxIterations; j++){
			phiCurr = solve.getPhi(&bodies,&constraints,t,numConstraints);
			phi_qCurr = solve.getJacobian(&bodies,&constraints,t,numConstraints);

			arma::vec correction = arma::solve(phi_qCurr,phiCurr);
			qCurr = qCurr - correction;
			updateQ();

			if(arma::norm(correction,2) <= tolerance){
				converge = true;
				break;
			}

		}
		if(!converge){
			std::cout << "Failed to converge after max itr at " << t  << std::endl;
			exit(1);
		}

		phi_qCurr = solve.getJacobian(&bodies,&constraints,t,numConstraints);
		q_list.push_back(qCurr);

		nu = solve.getNu(&bodies,&constraints,t,numConstraints);
		qdCurr = arma::solve(phi_qCurr,nu);
		//qdCurr = arma::solve(phi_qCurr,nu);
		updateQd();
		qd_list.push_back(qdCurr);


		gamma = solve.getGamma(&bodies,&constraints,t,numConstraints);
		qddCurr = arma::solve(phi_qCurr,gamma);
		updateQdd();
		qdd_list.push_back(qddCurr);
		std::cout << "Time = " << t << std::endl;
		std::cout << gamma << std::endl;

		t+=stepSize;

	}

}
void Model::solveD(){
	std::cout << "Dynamics Analysis: " << std::endl;

	Solver solve;
	/**
	 * Solve for initial accelerations and lambdas before start time iterations
	 */
	updateForces(); //Update forces at time 0
	updateQA(); //update generalized forces at time 0
	gamma = solve.getGamma(&bodies,&constraints,t,numConstraints); //get gamma to solve for initial conditions
	phi_qCurr = solve.getJacobian(&bodies,&constraints,t,numConstraints); //get jacobian to solve for initial conditions
	QAGamma = arma::join_cols(QACurr,gamma);
	arma::mat phi_qT = phi_qCurr.t();
	arma::mat J = join_rows(join_cols(M,phi_qCurr),join_cols(phi_qT,zeros(numConstraints,numConstraints)));
	qddLambda = arma::solve(J,QAGamma);
	qddCurr = qddLambda.rows(0,(bodies.size()*3) -1);
	lambdaCurr = qddLambda.rows(bodies.size()*3,qddLambda.n_elem-1);

	updateQdd();

	q_list.push_back(qCurr);
	qd_list.push_back(qdCurr);
	qdd_list.push_back(qddCurr);
	lambda_list.push_back(lambdaCurr);
	phi_q_list.push_back(phi_qCurr);
	force_list.push_back(QACurr);
	//std::cout << QACurr << std::endl;
	int maxIterations = 100;
	double tolerance = 1.0e-7;

	/**
	 * Start time iterations
	 */
	t += stepSize;

	for(int i = 1; i <= simulationSteps;i++){
		updateQ();
	    updateQd();
		updateQdd();
		phiCurr = solve.getPhi(&bodies,&constraints,t,numConstraints);
		arma::vec q0 = qCurr;
		arma::vec qd0 = qdCurr;
		arma::vec qdd0 = qddCurr;
		//std::cout << "initial acc = " << qdd0 <<std::endl;
		bool converge = false;
		arma::vec correction;

		for(int j = 0; j < maxIterations; j++){
			/**
			 *Newmarks Method
			 */

			qCurr = q0 + stepSize*qd0 + (stepSize*stepSize/2)*((1-2*BETA) * qdd0 + 2*BETA*qddCurr);
			//std::cout << qCurr << std::endl;
			qdCurr = qd0 + stepSize*((1-GAMMA)*qdd0 + GAMMA*qddCurr);
			//std::cout << qdCurr << std::endl;
			//if(j == 0){
				phi_qCurr = solve.getJacobian(&bodies,&constraints,t,numConstraints);
				phi_qT = phi_qCurr.t();
				J = join_rows(join_cols(M,phi_qCurr),join_cols(phi_qT,zeros(numConstraints,numConstraints)));
			//}
			//Update vectors and matrices with our new positions and velocities
			updateQ();
			updateQd();
			updateQdd();

			phiCurr = solve.getPhi(&bodies,&constraints,t,numConstraints);
			updateForces();
			updateQA();

			arma::mat psi = M*qddCurr+phi_qT*lambdaCurr-QACurr;
			arma::mat omega = (1/(BETA*stepSize*stepSize)) * phiCurr;

			arma::mat psi_omega = arma::join_cols(psi,omega);
			//std::cout << psi_omega << std::endl;
			correction = arma::solve(J,psi_omega);
			//std::cout << correction << std::endl;

			qddCurr -= correction.rows(0,(bodies.size()*3) -1);
			lambdaCurr -= correction.rows(bodies.size()*3,correction.n_elem-1);

			if(arma::norm(correction.rows(0,(bodies.size()*3) -1),"inf") <= tolerance){
				std::cout << "Converging at " << j << "norm  = "<<arma::norm(correction.rows(0,(bodies.size()*3)-1),"inf")<<  std::endl;
				converge = true;
				break;
			}

		}
		if(!converge){
			std::cout << phiCurr<<std::endl;

			std::cout << "Failed to converge at t=" << t << "norm  = "<<arma::norm(correction.rows(0,(bodies.size()*3) -1),"inf")<< std::endl;
            //exit(1);

		}

		qCurr = q0 + stepSize*qd0 + (stepSize*stepSize/2)*((1-2*BETA) * qdd0 + 2*BETA*qddCurr);

		qdCurr = qd0 + stepSize*((1-GAMMA)*qdd0 + GAMMA*qddCurr);

		updateQ();
		updateQd();
		updateQdd();

		q_list.push_back(qCurr);
		qd_list.push_back(qdCurr);
		qdd_list.push_back(qddCurr);
		lambda_list.push_back(lambdaCurr);
		phi_q_list.push_back(phi_qCurr);
		force_list.push_back(QACurr);
		t+=stepSize;
	}


}

void Model::setInitCond(){

	for(std::vector<int>::size_type i = 0; i < bodies.size();i++){
		int index = bodies.at(i).start;
		arma::vec q0 = bodies.at(i).getQ();
		arma::vec qd0 = bodies.at(i).getQd();
		qCurr(index) = q0(0);
		qCurr(index+1) = q0(1);
		qCurr(index+2) = q0(2);
		qdCurr(index) = qd0(0);
		qdCurr(index+1) = qd0(1);
		qdCurr(index+2) = qd0(2);
		std::cout << qdCurr <<std::endl;
	}
	std::cout<<"Initial q = "<<std::endl;
	std::cout<<qCurr<<std::endl;
	std::cout<<"Initial qd = "<<std::endl;
	std::cout<<qdCurr<<std::endl;
	updateQ();
	updateQd();
	//q_list.push_back(qCurr);
	//qd_list.push_back(qdCurr);
}
void Model::updateQ(){
	for(int i = 0; i < bodies.size(); i++){
		arma::vec tmp = arma::zeros(3);
		tmp(0) = qCurr(bodies.at(i).start);
		tmp(1) = qCurr(bodies.at(i).start+1);
		tmp(2) = qCurr(bodies.at(i).start+2);
		bodies.at(i).setQ(tmp);
	}
}
void Model::updateQd(){
	for(int i = 0; i < bodies.size(); i++){
		arma::vec tmp = arma::zeros(3);
		tmp(0) = qdCurr(bodies.at(i).start);
		tmp(1) = qdCurr(bodies.at(i).start+1);
		tmp(2) = qdCurr(bodies.at(i).start+2);
		bodies.at(i).setQd(tmp);
	}
}
void Model::updateQdd(){
	for(int i = 0; i < bodies.size(); i++){
		arma::vec tmp = arma::zeros(3);
		tmp(0) = qddCurr(bodies.at(i).start);
		tmp(1) = qddCurr(bodies.at(i).start+1);
		tmp(2) = qddCurr(bodies.at(i).start+2);
		bodies.at(i).setQdd(tmp);
	}
}
void Model::updateM(){
	arma::vec diagonal;
	diagonal.zeros(bodies.size()*3);
	for(int i = 0; i < bodies.size();i++){
		Body b = bodies.at(i);
		diagonal(b.start) = b.getMass();
		diagonal(b.start+1) = b.getMass();
		diagonal(b.start+2) = b.getJBar();
	}
	M.diag() = diagonal;
}
void Model::updateForces(){
	for(int i = 0; i < forces.size();i++){
		int bodyIndex = forces.at(i)->bodyID - 1;
		if(forces.at(i)->type == "forceFile"){
			double xVal = qCurr(bodies.at(bodyIndex).start);
			double xDotVal = qdCurr(bodies.at(bodyIndex).start);
			forces.at(i)->updateForce(t,xVal,xDotVal);
		}
		else{
			int angleIndex = bodies.at(bodyIndex).start + 2;
			double currentAngle = qCurr(angleIndex);
			forces.at(i)->updateForce(t,currentAngle);
		}

	}
}
void Model::updateQA(){
	QACurr.zeros(bodies.size()*3);
	for(int i = 0; i < bodies.size(); i++){
		double mass = bodies.at(i).getMass();
		QACurr(bodies.at(i).start) += gravity(0)*mass;
		QACurr(bodies.at(i).start+1) += gravity(1)*mass;
	}
	for(int i = 0; i < forces.size(); i++){
		int bodyIndex = forces.at(i)->bodyID - 1;
		int forceIndex = bodies.at(bodyIndex).start;
		arma::vec tmpForce = forces.at(i)->getForce();
		QACurr(forceIndex) += tmpForce(0);
		QACurr(forceIndex+1) += tmpForce(1);
		QACurr(forceIndex+2) += tmpForce(2);
	}

}
/**
 * The following getQList methods overload the getQList method that return the generalized coords for the center of mass and instead
 * of returning the gen coord at center of mass, it will return the gen coords at point Q.
 */
const std::vector<arma::vec>& Model::getQList(int bodyID, double spX, double spY){
	this->qp_list = this->q_list;
	Body b = bodies.at(bodyID - 1);
	std:: cout << "start" << b.start << std::endl;


	for(int i = 0; i < qp_list.size();i++){

		arma::vec r(2);
		r(0) = qp_list.at(i)(b.start);
		r(1) = qp_list.at(i)(b.start+1);
		double anglePhi = qp_list.at(i)(b.start+2);
		double sine = sin(anglePhi);
		double cosine = cos(anglePhi);
		qp_list.at(i)(b.start) = r(0) + spX*cosine - spY*sine;
		qp_list.at(i)(b.start+1) = r(1) + spX*sine + spY*cosine;

	}

	return qp_list;
}
const std::vector<arma::vec>& Model::getQdList(int bodyID, double spX, double spY){
	this->qp_list = this->q_list;
	this->qdp_list = this->qd_list;
	Body b = bodies.at(bodyID - 1);


	for(int i = 0; i < qdp_list.size();i++){
		arma::vec rd(2);
		rd(0) = qdp_list.at(i)(b.start);
		rd(1) = qdp_list.at(i)(b.start+1);
		double anglePhi = q_list.at(i)(b.start+2);
		double anglePhid = qd_list.at(i)(b.start+2);
		double sine = sin(anglePhi);
		double cosine = cos(anglePhi);
		qdp_list.at(i)(b.start) = rd(0) - (sine*spX)*anglePhid - (cosine*spY)*anglePhid;
		qdp_list.at(i)(b.start+1) = rd(1) + (cosine*spX)*anglePhid - (sine*spY)*anglePhid;

	}
	return qdp_list;

}
const std::vector<arma::vec>& Model::getQddList(int bodyID, double spX, double spY){
	this->qddp_list = this->qdd_list;
	Body b = bodies.at(bodyID - 1);


	for(int i = 0; i < qddp_list.size();i++){
		arma::vec rdd(2);
		rdd(0) = qdd_list.at(i)(b.start);
		rdd(1) = qdd_list.at(i)(b.start+1);

		double anglePhi = q_list.at(i)(b.start+2);
		double anglePhid = qd_list.at(i)(b.start+2);
		double anglePhidd = qdd_list.at(i)(b.start+2);

		double sine = sin(anglePhi);
		double cosine = cos(anglePhi);

		qddp_list.at(i)(b.start) = rdd(0) - cosine*spX*anglePhid*anglePhid - sine*spX*anglePhidd + sine*spY*anglePhid*anglePhid - cosine*spY*anglePhidd;
		qddp_list.at(i)(b.start+1) = rdd(1)  - sine*spX*anglePhid*anglePhid + cosine*spX*anglePhidd - cosine*spY*anglePhid*anglePhid - sine*spY*anglePhidd;
		//std::cout << rdd(1) <<  " " << anglePhi << " "<< sine <<  " " <<cosine <<  " " << anglePhid <<  " " <<anglePhidd <<  " " <<spX <<  " " <<spY <<  " " <<std::endl;

	}
	return qddp_list;

}
const std::vector<arma::vec>& Model::getReactionForces(int constraintID, int bodyID1,arma::vec sp1,int bodyID2 ,arma::vec sp2){

	int numBodies = constraints.at(constraintID-1)->numBodies;
	double anglePhiCurr;
	arma::mat phi_qR;
	arma::mat phi_qPhi;
	arma::vec lambdaK;
	arma::mat B;
	arma::vec fK;
	arma::vec tK;
	arma::vec reactionF = arma::zeros(3);



	if(numBodies == 1){
		Body b = bodies.at(bodyID1-1);

		for(int i = 0; i <= simulationSteps;i++){
			phi_qCurr = phi_q_list.at(i);
			lambdaCurr = lambda_list.at(i);
			qCurr = q_list.at(i);
			anglePhiCurr = qCurr(b.start+2);
			B = b.getB(anglePhiCurr);

			phi_qR = phi_qCurr.submat(constraintID-1,b.start,constraintID-1,b.start+1);
			phi_qPhi = phi_qCurr.submat(constraintID-1,b.start+2,constraintID-1,b.start+2);
			lambdaK = lambdaCurr(constraintID-1);

			fK = -1*phi_qR.t()*lambdaK;
			reactionF(0) = fK(0);
			reactionF(1) = fK(1);

			tK = sp1.t()*B.t()*fK - phi_qPhi.t()*lambdaK;
			reactionF(2) = tK(0);
			reactionForces_list.push_back(reactionF);


		}

	}
	else{
		Body b1 = bodies.at(bodyID1-1);
		//Body b2 = bodies.at(bodyID2-1);

		for(int i = 0; i <= simulationSteps;i++){
			phi_qCurr = phi_q_list.at(i);
			lambdaCurr = lambda_list.at(i);
			qCurr = q_list.at(i);
			anglePhiCurr = qCurr(b1.start+2);
			B = b1.getB(anglePhiCurr);

			phi_qR = phi_qCurr.submat(constraintID-1,b1.start,constraintID,b1.start+1);
			phi_qPhi = phi_qCurr.submat(constraintID-1,b1.start+2,constraintID,b1.start+2);
			lambdaK = lambdaCurr.rows(constraintID-1,constraintID);

			fK = -1*phi_qR.t()*lambdaK;
			reactionF(0) = fK(0);
			reactionF(1) = fK(1);

			tK = sp1.t()*B.t()*fK - phi_qPhi.t()*lambdaK;
			reactionF(2) = tK(0);

			reactionForces_list.push_back(reactionF);
		}

		//arma::mat phi_qR2 = phi_qCurr.submat(constraintID-1,b2.start,constraintID-1,b2.start+1);

	}



	return reactionForces_list;
}

