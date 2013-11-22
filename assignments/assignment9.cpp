/*
 * assignment9.cpp
 *
 *  Created on: Nov 19, 2013
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

	cout << "Start of Assignment 9 " <<endl;
	cout << "Part 1.2. Solve the IVP with Forward Euler: " <<endl;
	Function yDot;
	yDot.setFunction("sin(y)");
	yDot.print();
	arma::vec y = arma::zeros(1001);
	arma::vec t = arma::zeros(1001);
	double fEuler[2][t.n_elem];
	double bEuler[2][t.n_elem];
	y(0) = 1;
	fEuler[0][0] = 0;
	fEuler[1][0] = y(0);
	double stepSize = 0.01;
	double tolerance = pow(10.0,-10.0);
	for(int i = 1; i < t.n_elem; i++){
		t(i) = i*stepSize;
		fEuler[0][i] = t(i);
		y(i) = y(i-1) + (stepSize*yDot.eval2(y(i-1)));
		fEuler[1][i] = y(i);

	}

	cout << "Part 1.3. Solve the IVP with Backward Euler: " <<endl;

	y = arma::zeros(1001);
	t = arma::zeros(1001);
	Function f;
	f.setFunction("y - 0.01*sin(y)");
	f.print();
	cout << f.eval2(1) << endl;
	Function fPrime;
	fPrime.setFunction("1 - 0.01*cos(y)");
	fPrime.print();
	cout << fPrime.eval2(1) << endl;

	int maxIterations = 20;
	y(0) = 1.0;
	double yCurr = y(0);
	bEuler[0][0] = 0;
	bEuler[1][0] = y(0);
	for(int i = 1; i < t.n_elem; i++){
			/**
			 * Start of Newton Raphson
			 */
		double correction = (f.eval2(y(i-1)) - y(i-1))/fPrime.eval2(y(i-1));
		y(i) = y(i-1) - correction;
		bEuler[1][i] = y(i);
		t(i) = i*stepSize;
		bEuler[0][i] = t(i);

	}

	Document doc;
	doc.SetObject();
	rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
	Value plot1(kArrayType);
	Value plot1X(kArrayType);
    Value plot1Y(kArrayType);
	Value plot2(kArrayType);
	Value plot2X(kArrayType);
    Value plot2Y(kArrayType);
	for(int i = 0; i < t.n_elem; i++){

		plot1X.PushBack(fEuler[0][i],allocator);
		plot1Y.PushBack(fEuler[1][i],allocator);

		plot2X.PushBack(bEuler[0][i],allocator);
		plot2Y.PushBack(bEuler[1][i],allocator);

	}
	plot1.PushBack(plot1X,allocator);
	plot1.PushBack(plot1Y,allocator);
	plot2.PushBack(plot2X,allocator);
	plot2.PushBack(plot2Y,allocator);

	doc.AddMember("Plot1",plot1,allocator);
	doc.AddMember("Plot2",plot2,allocator);

	StringBuffer strbuf;
	Writer<StringBuffer> writer(strbuf);
	doc.Accept(writer);

    std::string plotsData = strbuf.GetString();


	ofstream dataFile ("plots/data.json");
	if (dataFile.is_open()){
	    dataFile << plotsData;
	    dataFile.close();
	}
	else cout << "Unable to open file";
	system( "python ../../repos/simEngine/python/plot2D.py" );

	cout << "Part 2. Assignment 9 : " << endl;

	MyJsonDocument d = parseJSON("models/doublePend.acf");
	string simulation = string(d["simulation"].GetString());
	double tend = d["tend"].GetDouble();
	stepSize = d["stepSize"].GetDouble();
	double outputSteps = d["outputSteps"].GetDouble();
	cout << "Simulation = "  + simulation <<endl;
	cout << "tend = ";
	cout << tend << endl;
	cout << "stepSize = ";
	cout << stepSize << endl;
	cout << "outputSteps = ";
	cout << outputSteps << endl;

	MyJsonDocument d2 = parseJSON("models/doublePend.adm");
	Model m(d2);
	m.setSimulationType(simulation);
	m.setStepSize(stepSize);
	m.setTEnd(tend);
	m.setOutputSteps(outputSteps);
	m.solveD();


	cout << timer.toc() <<endl;



}
