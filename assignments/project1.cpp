/*
 * project1.cpp
 *
 *  Created on: Nov 5, 2013
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

using namespace rapidjson;



using namespace std;

int main(int argc, char** argv){
	arma::wall_clock timer; //Keeps track of how much time my program is taking to run.

	timer.tic();

	cout << "In this program I'm assuming that the given json model.adm file will follow a certain format, where keys will have an expected name." <<endl;
	//cout << "If no information is displaying after this line that means that you didn't input the two integer arguments for bodyID and constraintID." <<endl;
	//double constraintID = double(atof(argv[1]));

	cout << "Start of Assignment" <<endl;


	MyJsonDocument d = parseJSON("models/dPend.acf");

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

	MyJsonDocument d4 = parseJSON("models/dPend.adm");
	cout << "parsing is fine" << endl;
	Model m(d4, simulation, tend,outputSteps,stepSize);

	//Set simulation properties (type of simulation, stepsize, outputsteps, tend)



	m.solveK();

	int bodyID = 2;
	double spX = 3;
	double spY = 0;

	std::vector<arma::vec> qgen = m.getQList();
	std::vector<arma::vec> qQ = m.getQList(bodyID,spX,spY);

	std::vector<arma::vec> qdgen = m.getQdList();
	std::vector<arma::vec> qdQ = m.getQdList(bodyID,spX,spY);

	std::vector<arma::vec> qddgen = m.getQddList();
	std::vector<arma::vec> qddQ = m.getQddList(bodyID,spX,spY);




	double xVelPlot[2][(int)m.getOutputSteps()];
	double yVelPlot[2][(int)m.getOutputSteps()];

	double xAccPlot[2][(int)m.getOutputSteps()];
	double yAccPlot[2][(int)m.getOutputSteps()];

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

	for(int i = 0; i <= m.getOutputSteps(); i++){

		xVelPlot[0][i] = m.getStepSize()*i;
		xVelPlot[1][i] = qdQ.at(i)(m.getBodies().at(1).start);
		plot1X.PushBack(xVelPlot[0][i],allocator);
		plot1Y.PushBack(xVelPlot[1][i],allocator);

		yVelPlot[0][i] = m.getStepSize()*i;
		yVelPlot[1][i] = qdQ.at(i)(m.getBodies().at(1).start+1);
		plot2X.PushBack(yVelPlot[0][i],allocator);
		plot2Y.PushBack(yVelPlot[1][i],allocator);

		xAccPlot[0][i] = m.getStepSize()*i;
		xAccPlot[1][i] = qddQ.at(i)(m.getBodies().at(1).start);
		plot3X.PushBack(xAccPlot[0][i],allocator);
		plot3Y.PushBack(xAccPlot[1][i],allocator);

		yAccPlot[0][i] = m.getStepSize()*i;
		yAccPlot[1][i] = qddQ.at(i)(m.getBodies().at(1).start+1);
		plot4X.PushBack(yAccPlot[0][i],allocator);
		plot4Y.PushBack(yAccPlot[1][i],allocator);




	}
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

	jsonFunctions json;
	json.save(doc);

	cout << "took " << timer.toc() << " seconds for part 2" << endl;
	timer.tic();
	system( "python ../../repos/simEngine/python/plot2D.py" );


	cout << "took " << timer.toc() << " seconds to plot" << endl;
	ofstream outputFile;
	outputFile.open("dPend.res");
	for (int i = 0; i < m.getOutputSteps();i++){

		outputFile << "t = " << i*m.getStepSize() << ", xQ(" << i*m.getStepSize() << ") = " << qQ.at(i)(3);
		outputFile << ", yQ(" << i*m.getStepSize() << ") = " << qQ.at(i)(4);
		outputFile << ", anglephiQ(" << i*m.getStepSize() << ") = " << qQ.at(i)(5);

		outputFile << ", xdQ(" << i*m.getStepSize() << ") = " << qdQ.at(i)(3);
		outputFile << ", ydQ(" << i*m.getStepSize() << ") = " << qdQ.at(i)(4);
		outputFile << ", anglephidQ(" << i*m.getStepSize() << ") = " << qdQ.at(i)(5);

		outputFile << ", xddQ(" << i*m.getStepSize() << ") = " << qddQ.at(i)(3);
		outputFile << ", yddQ(" << i*m.getStepSize() << ") = " << qddQ.at(i)(4);
		outputFile << ", anglephiddQ(" << i*m.getStepSize() << ") = " << qddQ.at(i)(5) << endl;
	}



  return 0;
}
