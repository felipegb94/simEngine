/*
 * project2.cpp
 *
 *  Created on: Nov 29, 2013
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

	cout << "Project 2 : " << endl;

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

	MyJsonDocument d2 = parseJSON("models/sliderCrank.adm");
	Model m(d2, simulation, tend,outputSteps,stepSize);
	m.setSimulationType(simulation);
	m.setStepSize(stepSize);
	m.setTEnd(tend);
	m.setOutputSteps(outputSteps);
	m.solveD();

	cout << "Time elapsed for dynamics analysis: ";
	cout << timer.toc() <<endl;

	arma::vec sp1 = arma::zeros(2);

	sp1(0) = 0;
	sp1(1) = 0;
	int bodyID = 2;
	double spX = 3;
	double spY = 0;

	std::vector<arma::vec> reactionForces = m.getReactionForces(1,1,sp1);

	std::vector<arma::vec> qgen = m.getQList();
	std::vector<arma::vec> qQ = m.getQList(bodyID,spX,spY);

	std::vector<arma::vec> qdgen = m.getQdList();
	std::vector<arma::vec> qdQ = m.getQdList(bodyID,spX,spY);

	std::vector<arma::vec> qddgen = m.getQddList();
	std::vector<arma::vec> qddQ = m.getQddList(bodyID,spX,spY);

	std::vector<arma::vec> lambdas = m.getLambdaList();




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
		xVelPlot[1][i] = qdgen.at(i)(m.getBodies().at(0).start+2);
		plot1X.PushBack(xVelPlot[0][i],allocator);
		plot1Y.PushBack(xVelPlot[1][i],allocator);

		yVelPlot[0][i] = m.getStepSize()*i;
		yVelPlot[1][i] = qgen.at(i)(6);
		plot2X.PushBack(yVelPlot[0][i],allocator);
		plot2Y.PushBack(yVelPlot[1][i],allocator);

		xAccPlot[0][i] = m.getStepSize()*i;
		xAccPlot[1][i] = qdgen.at(i)(6);
		plot3X.PushBack(xAccPlot[0][i],allocator);
		plot3Y.PushBack(xAccPlot[1][i],allocator);

		yAccPlot[0][i] = m.getStepSize()*i;
		yAccPlot[1][i] = reactionForces.at(i)(0);
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

	//plot4.PushBack(plot4X,allocator);
	//plot4.PushBack(plot4Y,allocator);

	doc.AddMember("Plot1",plot1,allocator);
	doc.AddMember("Plot2",plot2,allocator);
	doc.AddMember("Plot3",plot3,allocator);
	//doc.AddMember("Plot4",plot4,allocator);
    std::cout << "data"<<std::endl;

	jsonFunctions json;
	json.save(doc);
    std::cout << "data"<<std::endl;

	timer.tic();
	system( "python ../../repos/simEngine/python/plot2D.py" );


	cout << "took " << timer.toc() << " seconds to plot" << endl;

	ofstream outputFile;
	outputFile.open("sliderCrank.res");
	for (int i = 0; i < m.getSimulationSteps();i++){

		outputFile << "t = " << i*m.getStepSize() << ", q(" << i*m.getStepSize() << ") = " << qgen.at(i).t();
		outputFile << ", qd(" << i*m.getStepSize() << ") = " << qdgen.at(i).t();
		outputFile << ", qdd(" << i*m.getStepSize() << ") = " << qddgen.at(i).t();
		outputFile << ", lamda(" << i*m.getStepSize() << ") = " << lambdas.at(i).t() << endl;

	}


}
