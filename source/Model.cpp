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
	std::cout << "here" <<std::endl;
	for(int i = 0; i < jsonConstraints.Size(); i++){
		const rapidjson::Value& v = jsonConstraints[i];


		if(std::string(v["type"].GetString()) == "AbsoluteX"){
			//c_absX cX(v);
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
	double tolerance = 0.00001;
	//Setting initial conditions for q. Necessary for newton raphson.
	for(std::vector<int>::size_type i = 0; i < bodies.size();i++){
		arma::vec temp = bodies.at(i).getQ();
		qCurr.insert_rows(qCurr.n_elem,temp);
		std::cout << bodies.at(i).getA() << std::endl;
		std::cout << bodies.at(i).getB() << std::endl;
	}

	std::cout << qCurr << std::endl;

	Solver solve;

	for(int i = 0; i < 1; i++){
		phiCurr = solve.getPhi(&bodies,&constraints,t);
		phi_qCurr = solve.getJacobian(&bodies,&constraints,t);
	}
	nu = solve.getNu(&bodies,&constraints,t);



}
