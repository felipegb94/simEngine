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


//#include "Symbolic/headers/symbolicc++.h"



using namespace std;

int main(int argc, char** argv){
	cout << "In this program I'm assuming that the given json model.adm file will follow a certain format, where keys will have an expected name." <<endl;
	arma::wall_clock timer; //Keeps track of how much time my program is taking to run.
	timer.tic();
	cout << "If no information is displaying after this line that means that you didn't input the two integer arguments for bodyID and constraintID." <<endl;
	double bodyID = double(atof(argv[1]));
	double constraintID =double(atof(argv[2]));
	cout << "Start of Assignment" <<endl;

	//try{
	//bodyID = atof(argv[1]);
	//constraintID = atof(argv[2]);
/**
	}catch(exception& e){
		cout << "Exception catched" << endl;
		cout << "Please enter two valid integer as arguments. First integer corresponds to body ID and the second one to constraint ID." << endl;
		return 0;
	}*/






	cout << "Part 1. " << endl;

	MyJsonDocument d = parseJSON("input.acf");

	//static const char* kTypeNames[] = { "Null", "False", "True", "Object", "Array", "String", "Number" };
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

	cout << "Part 2. " << endl;
	MyJsonDocument d4 = parseJSON("model2.adm");
	Model m(d4);

	cout <<"Body ID specified: ";
	cout << bodyID <<endl;
	vector<Body> bodies = m.getBodies();

	for(std::vector<int>::size_type i = 0; i != bodies.size(); i++) {
		if(bodies[i].getID() == bodyID){
			Body toPrint = bodies[i];
			toPrint.print();

		}
	}
	//m.getConstraints().at(1)->print();


	vector<c_constraint*> constraints = m.getConstraints();
	for(std::vector<int>::size_type i = 0; i != constraints.size(); i++) {
		/**
		if( constraints[i].name == "abs_x"){
			//c_absX tmpX = (c_absX)constraints[i];
			cout << tmpX.xGround <<endl;
		}*/
		if(constraints[i]->id == constraintID){
			cout <<"Constraint ID specified: ";
			cout << constraints[i] -> id <<endl;
			constraints[i]->print();

		}
	}






	cout << "took " << timer.toc() << " seconds" << endl;



  return 0;
  }
void runme(double bodyID, double constraintID){
	cout<<"running run me"<<endl;
}



