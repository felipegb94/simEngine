/*
 * assignment4.cpp
 *
 *  Created on: Oct 14, 2013
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




using namespace std;

int main(int argc, char** argv){

	//system( "python python/test.py" );
	arma::vec x = arma::zeros(10);
	x.fill(arma::datum::pi);
	cout << arma::cos(2*x) << endl;



	cout << "In this program I'm assuming that the given json model.adm file will follow a certain format, where keys will have an expected name." <<endl;
	arma::wall_clock timer; //Keeps track of how much time my program is taking to run.
	timer.tic();
	cout << "If no information is displaying after this line that means that you didn't input the two integer arguments for bodyID and constraintID." <<endl;
	//double constraintID = double(atof(argv[1]));

	cout << "Start of Assignment" <<endl;

	cout << "Parsing sliderCrank.acf" << endl;

	MyJsonDocument d = parseJSON("models/sliderCrank.acf");

	string simulation = string(d["simulation"].GetString());
	double tend = d["tend"].GetDouble();
	double stepSize = d["stepSize"].GetDouble();
	double outputSteps = d["outputSteps"].GetDouble();


	cout << "Simulation = "  + simulation <<endl;
	cout << "tend = ";
	cout << tend << endl;
	cout << "stepSize = ";
	cout << stepSize << endl;
	cout << "outputSteps = ";
	cout << outputSteps << endl;


	//static const char* kTypeNames[] = { "Null", "False", "True", "Object", "Array", "String", "Number" };

	MyJsonDocument d4 = parseJSON("models/sliderCrank.adm");
	Model m(d4);

	//Set simulation properties (type of simulation, stepsize, outputsteps, tend)
	m.setSimulationType(simulation);
	m.setTEnd(tend);
	m.setOutputSteps(outputSteps);
	m.setStepSize(stepSize);

	vector<c_constraint*> constraints = m.getConstraints();
	for(std::vector<int>::size_type i = 0; i != constraints.size(); i++) {

			constraints[i]->print();


	}






	cout << "took " << timer.toc() << " seconds" << endl;



  return 0;
  }
