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

#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"

using namespace rapidjson;



using namespace std;

int main(int argc, char** argv){
	arma::wall_clock timer; //Keeps track of how much time my program is taking to run.
	timer.tic();

	cout << "In this program I'm assuming that the given json model.adm file will follow a certain format, where keys will have an expected name." <<endl;

	cout << "If no information is displaying after this line that means that you didn't input the two integer arguments for bodyID and constraintID." <<endl;
	//double constraintID = double(atof(argv[1]));

	cout << "Start of Assignment" <<endl;

	cout << "Part 2. " << endl;

	MyJsonDocument d = parseJSON("models/finalExam.acf");

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


	MyJsonDocument d4 = parseJSON("models/finalExam.adm");
	cout << "parsing is fine" << endl;

	Model m(d4, simulation, tend,outputSteps,stepSize);

	vector<c_constraint*> constraints = m.getConstraints();

	for(std::vector<int>::size_type i = 0; i != constraints.size(); i++) {
			constraints[i]->print();
	}

	m.solveD();
	cout << "took " << timer.toc() << " seconds to solve" << endl;
	timer.tic();
	arma::vec Q = arma::zeros(2);
	arma::vec P = arma::zeros(2);
	double constraintID = 1;

	Q(0) = -2;
	int bodyID = 1;

	std::vector<arma::vec> qgen = m.getQList();
	std::vector<arma::vec> qQ = m.getQList(2,Q(0),Q(1));

    std::vector<arma::vec> qdgen = m.getQdList();

	std::vector<arma::vec> qddgen = m.getQddList();
	std::vector<arma::vec> reactionForces = m.getReactionForces(constraintID, bodyID,P);
	cout << reactionForces.at(0).n_elem <<endl;
	//arma::vec sP1 = arma::zeros(2);
	//sP1(0) = -0.02;
	//std::vector<arma::vec> requiredTorque = m.getReactionForces(7, 1,sP1);

	double xPlot[2][(int)m.getSimulationSteps()];
	double yPlot[2][(int)m.getSimulationSteps()];
	double xAccPlot[2][(int)m.getSimulationSteps()];
	double yAccPlot[2][(int)m.getSimulationSteps()];

	rapidjson::Document doc;
	doc.SetObject();
	rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();

	rapidjson::Value plot1(kArrayType);
	rapidjson::Value plot1X(kArrayType);
	rapidjson::Value plot1Y(kArrayType);
	rapidjson::Value plot2(kArrayType);
	rapidjson::Value plot2X(kArrayType);
	rapidjson::Value plot2Y(kArrayType);


	rapidjson::Value plot3(kArrayType);
	rapidjson::Value plot3X(kArrayType);
	rapidjson::Value plot3Y(kArrayType);

	rapidjson::Value plot4(kArrayType);
	rapidjson::Value plot4X(kArrayType);
	rapidjson::Value plot4Y(kArrayType);
		cout << "here" << endl;

		for(int i = 0; i <= m.getSimulationSteps(); i++){

			xPlot[0][i] = qgen.at(i)(4);
			xPlot[1][i] = qgen.at(i)(3);

			plot1X.PushBack(xPlot[0][i],allocator);
			plot1Y.PushBack(xPlot[1][i],allocator);

			yPlot[0][i] = m.getStepSize()*i;
			yPlot[1][i] = qgen.at(i)(0);
			plot2X.PushBack(yPlot[0][i],allocator);
			plot2Y.PushBack(yPlot[1][i],allocator);

			xAccPlot[0][i] = m.getStepSize()*i;
			xAccPlot[1][i] = reactionForces.at(i)(2);
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

		cout << "took " << timer.toc() << " seconds to plot" << endl;






  return 0;
  }
