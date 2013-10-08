/*
 * Function.h
 *
 *  Created on: Sep 26, 2013
 *      Author: pipe
 */

#ifndef FUNCTION_H_
#define FUNCTION_H_

#include <muParser.h>

#include <string>
#include <iostream>
using namespace mu;
using namespace std;

#define sim2D_pi 3.141592653589793
#define sim2D_e 2.718281828459

class Function{

private:
	Parser p;
	double t;
public:

	Function();
	void setFunction(std::string function);
	double eval(int numSteps, double stepSize);
	double eval(double val);
	void print();

};


#endif /* FUNCTION_H_ */
