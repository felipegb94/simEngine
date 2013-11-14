/*
 * assignment8.cpp
 *
 *  Created on: Nov 11, 2013
 *      Author: pipe
 */

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <armadillo>

#include "jsonParser.h"
#include "rapidjson/document.h"
#include "Body.h"
#include "c_constraint.h"
#include "Model.h"
#include "Function.h"
#include <stdlib.h>
#include <math.h>

#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"

#include "f_force.h"

using namespace rapidjson;

using namespace std;

int main(int argc, char** argv){
	arma::wall_clock timer; //Keeps track of how much time my program is taking to run.

	timer.tic();

	cout << "In this program I'm assuming that the given json model.adm file will follow a certain format, where keys will have an expected name." <<endl;
	//cout << "If no information is displaying after this line that means that you didn't input the two integer arguments for bodyID and constraintID." <<endl;

	cout << "Start of Assignment 8 " <<endl;


	MyJsonDocument d = parseJSON("models/forces.adm");
	cout << "parsing is fine" << endl;

	const rapidjson::Value& jsonForces = d["forces"];
	std::cout << "here" <<std::endl;

	for(int i = 0; i < jsonForces.Size(); i++){
		const rapidjson::Value& v = jsonForces[i];
		std::cout << "iteration" << i << std::endl;
		if(std::string(v["type"].GetString()) == "PointForce"){
			std::cout << "here" <<std::endl;
			f_pointForce force1(v);
			force1.print();
		}
		else if(std::string(v["type"].GetString()) == "Torque"){
			//c_absY cY(v);
			f_torque force2(v);
			force2.print();
		}

	}

	cout << "Force structures are fine. Part 1 Done" << endl;

	cout << timer.toc() <<endl;

/**
	vector<c_constraint*> constraints = m.getConstraints();
	for(std::vector<int>::size_type i = 0; i != constraints.size(); i++) {

			constraints[i]->print();


	}
*/

}
