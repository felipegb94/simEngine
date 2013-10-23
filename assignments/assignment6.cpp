/*
 * assignment6.cpp
 *
 *  Created on: Oct 23, 2013
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




using namespace std;

int main(int argc, char** argv){

	//system( "python python/test.py" );
	//arma::vec x = arma::zeros(10);
	//x.fill(arma::datum::pi);
	//cout << arma::cos(2*x) << endl;


	cout << "In this program I'm assuming that the given json model.adm file will follow a certain format, where keys will have an expected name." <<endl;
	arma::wall_clock timer; //Keeps track of how much time my program is taking to run.
	timer.tic();
	cout << "If no information is displaying after this line that means that you didn't input the two integer arguments for bodyID and constraintID." <<endl;
	//double constraintID = double(atof(argv[1]));

	cout << "Start of Assignment" <<endl;

	cout << "Part 1. " << endl;

	Function x;
	x.setFunction("(3*x) + sin(x*y) - 4");
	Function y;
	y.setFunction("x - exp(cos(y))");

	Function x1;
	x1.setFunction("3 + y*cos(x*y)");
	x1.print();
	Function y1;
	y1.setFunction("x*cos(x*y)");
	y1.print();
	Function x2;
	x2.setFunction("1");
	x2.print();
	Function y2;
	y2.setFunction("sin(y) * exp(cos(y))");
	y2.print();


	arma::vec q = zeros(2);
	q(0) = 1.5;
	q(1) = 1.5;

	arma::vec phi = arma::zeros(2);
	arma::mat phi_q = arma::zeros(2,2);


	double tolerance = pow(10.0,-8.0);
	arma::vec qTemp;
	qTemp = q;
	cout << "Initial Guess:  q = " << endl;
	cout << q << endl;


	for(int i = 0 ; i < 10; i++){
		cout << "Iteration ";
		cout << i << endl;


		phi(0) = x.eval2(qTemp(0),qTemp(1));
		phi(1) = y.eval2(qTemp(0),qTemp(1));
		phi_q(0,0) = x1.eval2(qTemp(0),qTemp(1));
		phi_q(0,1) = y1.eval2(qTemp(0),qTemp(1));
		phi_q(1,0) = x2.eval2(qTemp(0),qTemp(1));
		phi_q(1,1) = y2.eval2(qTemp(0),qTemp(1));

		arma::vec correction = arma::solve(phi_q,phi);
		std::cout << "q = " << std::endl;

		qTemp = qTemp - correction;
		std::cout << qTemp << std::endl;

		std::cout << "Correction = ";
		std::cout << arma::norm(correction,2) << std::endl;

		if(arma::norm(correction,2) <= tolerance){
			q = qTemp;
			break;
		}
	}
	cout << " " << endl;
	cout << " " << endl;
	cout << "" << endl;
	cout << "took " << timer.toc() << " seconds for part 1" << endl;

	cout << "Part 2. " << endl;

	timer.tic();
	MyJsonDocument d = parseJSON("models/simplePend.acf");

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

	MyJsonDocument d4 = parseJSON("models/simplePend.adm");
	cout << "parsing is fine" << endl;
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

	m.solve();





	cout << "took " << timer.toc() << " seconds for part 2" << endl;



  return 0;
  }
