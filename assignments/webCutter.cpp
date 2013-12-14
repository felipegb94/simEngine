/*
 * webCutter.cpp
 *
 *  Created on: Dec 10, 2013
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

using namespace rapidjson;



using namespace std;

int main(int argc, char** argv){
	arma::wall_clock timer; //Keeps track of how much time my program is taking to run.

	timer.tic();

	cout << "In this program I'm assuming that the given json model.adm file will follow a certain format, where keys will have an expected name." <<endl;
	//cout << "If no information is displaying after this line that means that you didn't input the two integer arguments for bodyID and constraintID." <<endl;
	//double constraintID = double(atof(argv[1]));

	cout << "Start of Assignment" <<endl;


	MyJsonDocument d = parseJSON("models/webCutter.acf");

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

	MyJsonDocument d4 = parseJSON("models/webCutter.adm");
	cout << "parsing is fine" << endl;
	Model m(d4, simulation, tend,outputSteps,stepSize);



	m.solveD();

	double bodyID1 = 2;
	double spX1 = 0.24;
	double spY1 = 0.04;
	double bodyID2 = 3;
	double spX2 = 0;
	double spY2 = 0.13;

	std::vector<arma::vec> qgen = m.getQList();
	std::vector<arma::vec> qQ1 = m.getQList(bodyID1,spX1,spY1);

	std::vector<arma::vec> qdgen = m.getQdList();
	std::vector<arma::vec> qQ2 = m.getQList(bodyID2,spX2,spY2);

	std::vector<arma::vec> qddgen = m.getQddList();
	std::vector<arma::vec> qddQ = m.getQddList(bodyID1,spX1,spY1);

	arma::vec sP1 = arma::zeros(2);
	sP1(0) = -0.02;
	std::vector<arma::vec> requiredTorque = m.getReactionForces(7, 1,sP1);

	double xPlot[2][(int)m.getSimulationSteps()];
	double yPlot[2][(int)m.getSimulationSteps()];
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
	cout << "here" << endl;

	for(int i = 0; i <= m.getSimulationSteps(); i++){

		xPlot[0][i] = m.getStepSize()*i;
		xPlot[1][i] = qQ1.at(i)(4);

		plot1X.PushBack(xPlot[0][i],allocator);
		plot1Y.PushBack(xPlot[1][i],allocator);

		yPlot[0][i] = m.getStepSize()*i;
		yPlot[1][i] = qQ2.at(i)(7);
		plot2X.PushBack(yPlot[0][i],allocator);
		plot2Y.PushBack(yPlot[1][i],allocator);

		xAccPlot[0][i] = m.getStepSize()*i;
		xAccPlot[1][i] = requiredTorque.at(i)(2);
		plot3X.PushBack(xAccPlot[0][i],allocator);
		plot3Y.PushBack(xAccPlot[1][i],allocator);
/**
		yAccPlot[0][i] = m.getStepSize()*i;
		yAccPlot[1][i] = qddQ.at(i)(1);
		plot4X.PushBack(yAccPlot[0][i],allocator);
		plot4Y.PushBack(yAccPlot[1][i],allocator);
*/
	}
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


	cout << "took " << timer.toc() << " seconds for part 2" << endl;
	/**
	ofstream outputFile;
	outputFile.open("torque.res");
	cout << requiredTorque.at(2) <<endl;
	for (int i = 0; i < m.getSimulationSteps();i++){
		outputFile << i*m.getStepSize() << " ";

	}
	outputFile << endl;

	for (int i = 0; i < m.getSimulationSteps();i++){
		outputFile << requiredTorque.at(i)(2) << " ";

	}
	outputFile<<endl;
	*/

  return 0;
}
