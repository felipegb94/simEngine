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


Model::Model(MyJsonDocument& d)
:gravity("0 -9.81"),
 name(d["name"].GetString()),
 t(0.0)
{


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
			std::cout << "here" <<std::endl;

			constraints.push_back(new c_absX(v));
		}
		else if(std::string(v["type"].GetString()) == "AbsoluteY"){
			//c_absY cY(v);

			constraints.push_back(new c_absY(v));
		}
		else if(std::string(v["type"].GetString()) == "AbsoluteAngle"){
			//c_absAngle cAngle(v);

			constraints.push_back(new c_absAngle(v));
		}
		else if(std::string(v["type"].GetString()) == "AbsoluteDistance"){
			//   c_absDist cDist(v);

			constraints.push_back(new c_absDist(v));
		}
		else if(std::string(v["type"].GetString()) == "RelativeX"){
			constraints.push_back(new c_relX(v));
		}
		else if(std::string(v["type"].GetString()) == "RelativeY"){
			constraints.push_back(new c_relY(v));
		}
		else if(std::string(v["type"].GetString()) == "RelativeDistance"){
			constraints.push_back(new c_relDist(v));
		}
		else if(std::string(v["type"].GetString()) == "RevoluteJoint"){
			constraints.push_back(new c_revJoint(v));
		}
		else if(std::string(v["type"].GetString()) == "TranslationalJoint"){
			constraints.push_back(new c_transJoint(v));
		}



	}

}
void Model::solve(){
	std::cout << "Solving " + name << std::endl;
	int maxIterations = 30;

	//Setting initial conditions for q. Necessary for newton raphson.
	qCurr = arma::zeros(bodies.size()*3);
	qdCurr = arma::zeros(bodies.size()*3);
	qddCurr = arma::zeros(bodies.size()*3);
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
	}
	std::cout<<"Initial q = "<<std::endl;
	std::cout<<qCurr<<std::endl;

	Solver solve;
	double tolerance = pow(10.0,-8.0);


	std::cout<<"Part 2b "<<std::endl;
	double goal = 0.37;
	for(int i = 0; i <= outputSteps;i++){
		for(int i = 0; i < maxIterations; i++){

			phiCurr = solve.getPhi(&bodies,&constraints,t);
			phi_qCurr = solve.getJacobian(&bodies,&constraints,t);
			arma::vec correction = arma::solve(phi_qCurr,phiCurr);
			qCurr = qCurr - correction;
			for(int i = 0; i < bodies.size(); i++){
				arma::vec tmp = bodies.at(i).getQ() - correction;
				bodies.at(i).setQ(tmp);
			}
			//phi_qCurr = solve.getJacobian(&bodies,&constraints,t);


			if(arma::norm(correction,2) <= tolerance){
				break;
			}

		}
		q_list.push_back(qCurr);
		phi_qCurr = solve.getJacobian(&bodies,&constraints,t);

		nu = solve.getNu(&bodies,&constraints,t);


		for(int i = 0; i < bodies.size(); i++){
			arma::vec tmp = arma::zeros(3);
			tmp(0) = nu(bodies.at(i).start);
			qdCurr(bodies.at(i).start) = tmp(0);
			tmp(1) = nu(bodies.at(i).start+1);
			qdCurr(bodies.at(i).start+1) = tmp(1);
			tmp(2) = nu(bodies.at(i).start+2);
			qdCurr(bodies.at(i).start+2) = tmp(2);
			bodies.at(i).setQd(tmp);
		}
		qd_list.push_back(qdCurr);

		gamma = solve.getGamma(&bodies,&constraints,t);

		for(int i = 0; i < bodies.size(); i++){
			arma::vec tmp = arma::zeros(3);
			tmp(0) = gamma(bodies.at(i).start);
			qddCurr(bodies.at(i).start) = tmp(0);
			tmp(1) = gamma(bodies.at(i).start+1);
			qddCurr(bodies.at(i).start) = tmp(0);
			tmp(2) = gamma(bodies.at(i).start+2);
			qddCurr(bodies.at(i).start) = tmp(0);
			bodies.at(i).setQdd(tmp);
		}
		qdd_list.push_back(qddCurr);
		t+=stepSize;

	}

}
