/*
 * assignment7.cpp
 *
 *  Created on: Oct 29, 2013
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

	system( "python ../../repos/simEngine/python/updateJSON.py" );
	timer.tic();

	cout << "In this program I'm assuming that the given json model.adm file will follow a certain format, where keys will have an expected name." <<endl;
	//cout << "If no information is displaying after this line that means that you didn't input the two integer arguments for bodyID and constraintID." <<endl;
	//double constraintID = double(atof(argv[1]));

	cout << "Start of Assignment" <<endl;


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

	std::vector<arma::vec> q = m.getQList();
	std::vector<arma::vec> qd = m.getQdList();
	std::vector<arma::vec> qdd = m.getQddList();

	double xAccPlot[2][(int)m.getOutputSteps()];
	double trajectoryPlot[2][(int)m.getOutputSteps()];

	Document doc;
	doc.SetObject();
	rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();

	Value plot1(kArrayType);
	Value plot1X(kArrayType);
    Value plot1Y(kArrayType);
	Value plot2(kArrayType);
	Value plot2X(kArrayType);
    Value plot2Y(kArrayType);

	for(int i = 0; i <= m.getOutputSteps(); i++){
		xAccPlot[0][i] = m.getStepSize()*i;
		xAccPlot[1][i] = qdd.at(i)(0);

		plot1X.PushBack(xAccPlot[0][i],allocator);
		plot1Y.PushBack(xAccPlot[1][i],allocator);


		trajectoryPlot[0][i] = q.at(i)(0);
		trajectoryPlot[1][i] = q.at(i)(1);
		plot2X.PushBack(trajectoryPlot[0][i],allocator);
		plot2Y.PushBack(trajectoryPlot[1][i],allocator);

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


	cout << "took " << timer.toc() << " seconds for part 2" << endl;



  return 0;
}
