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
	cout << "In this program I'm assuming that the given json model.adm file will follow a certain format, where keys will have an expected name." <<endl;
	arma::wall_clock timer; //Keeps track of how much time my program is taking to run.
	timer.tic();
	cout << "If no information is displaying after this line that means that you didn't input the two integer arguments for bodyID and constraintID." <<endl;
	double constraintID = double(atof(argv[1]));
	cout << "Start of Assignment" <<endl;


	//static const char* kTypeNames[] = { "Null", "False", "True", "Object", "Array", "String", "Number" };

	cout << "Part 1. Displaying new constraints. Enter the ID of the constraint you want to print in the arguments for the program. " << endl;
	MyJsonDocument d4 = parseJSON("model2.adm");
	Model m(d4);



	vector<c_constraint*> constraints = m.getConstraints();
	for(std::vector<int>::size_type i = 0; i != constraints.size(); i++) {

		if(constraints[i]->id == constraintID){
			cout <<"Constraint ID specified: ";
			cout << constraints[i] -> id <<endl;
			constraints[i]->print();

		}
	}






	cout << "took " << timer.toc() << " seconds" << endl;



  return 0;
  }




