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
#include "jsonFunctions.h"

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
	/**
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
	**/
	cout << "Part 2. Assignment 9 : " << endl;

	MyJsonDocument d = parseJSON("models/doublePend.acf");
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

	MyJsonDocument d2 = parseJSON("models/doublePend.adm");
	Model m(d2, simulation, tend,outputSteps,stepSize);
	m.setSimulationType(simulation);
	m.setStepSize(stepSize);
	m.setTEnd(tend);
	m.setOutputSteps(outputSteps);
	m.solveD();
	arma::vec sp1 = arma::zeros(2);
	sp1(0) = -1;
	sp1(1) = 0;
	cout << "Time elapsed for part 2: ";
	cout << timer.toc() <<endl;
	int bodyID = 2;
	double spX = 3;
	double spY = 0;

	std::vector<arma::vec> reactionForces = m.getReactionForces(3,2,sp1,2);

	std::vector<arma::vec> qgen = m.getQList();
	std::vector<arma::vec> qQ = m.getQList(bodyID,spX,spY);

	std::vector<arma::vec> qdgen = m.getQdList();
	std::vector<arma::vec> qdQ = m.getQdList(bodyID,spX,spY);

	std::vector<arma::vec> qddgen = m.getQddList();
	std::vector<arma::vec> qddQ = m.getQddList(bodyID,spX,spY);




	double xVelPlot[2][(int)m.getSimulationSteps()];
	double yVelPlot[2][(int)m.getSimulationSteps()];

	double xAccPlot[2][(int)m.getSimulationSteps()];
	double yAccPlot[2][(int)m.getSimulationSteps()];

	Document doc;
	doc.SetObject();
	rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();

	Value plot1(kArrayType);
	Value plot1X(kArrayType);
    Value plot1Y(kArrayType);

	Value plot2(kArrayType);
	Value plot2X(kArrayType);
    Value plot2Y(kArrayType);

	Value plot3(kArrayType);
	Value plot3X(kArrayType);
    Value plot3Y(kArrayType);

	Value plot4(kArrayType);
	Value plot4X(kArrayType);
    Value plot4Y(kArrayType);
    std::cout << "data"<<std::endl;
	for(int i = 0; i < m.getSimulationSteps(); i++){

		xVelPlot[0][i] = m.getStepSize()*i;
		xVelPlot[1][i] = reactionForces.at(i)(0);
		plot1X.PushBack(xVelPlot[0][i],allocator);
		plot1Y.PushBack(xVelPlot[1][i],allocator);

		yVelPlot[0][i] = m.getStepSize()*i;
		yVelPlot[1][i] = reactionForces.at(i)(1);
		plot2X.PushBack(yVelPlot[0][i],allocator);
		plot2Y.PushBack(yVelPlot[1][i],allocator);

		xAccPlot[0][i] = m.getStepSize()*i;
		xAccPlot[1][i] = reactionForces.at(i)(2);
		plot3X.PushBack(xAccPlot[0][i],allocator);
		plot3Y.PushBack(xAccPlot[1][i],allocator);

		yAccPlot[0][i] = m.getStepSize()*i;
		yAccPlot[1][i] = qddQ.at(i)(m.getBodies().at(1).start+1);
		plot4X.PushBack(yAccPlot[0][i],allocator);
		plot4Y.PushBack(yAccPlot[1][i],allocator);


	}
    std::cout << "data2"<<std::endl;

	plot1.PushBack(plot1X,allocator);
	plot1.PushBack(plot1Y,allocator);

	plot2.PushBack(plot2X,allocator);
	plot2.PushBack(plot2Y,allocator);

	plot3.PushBack(plot3X,allocator);
	plot3.PushBack(plot3Y,allocator);

	plot4.PushBack(plot4X,allocator);
	plot4.PushBack(plot4Y,allocator);

	doc.AddMember("Plot1",plot1,allocator);
	doc.AddMember("Plot2",plot2,allocator);
	doc.AddMember("Plot3",plot3,allocator);
	doc.AddMember("Plot4",plot4,allocator);
    std::cout << "data"<<std::endl;

	jsonFunctions json;
	json.save(doc);
    std::cout << "data"<<std::endl;

	timer.tic();
	system( "python ../../repos/simEngine/python/plot2D.py" );


	cout << "took " << timer.toc() << " seconds to plot" << endl;


}
