/*
 * Function.cpp
 *
 *  Created on: Sep 26, 2013
 *      Author: pipe
 */
#include "../includes/Function.h"
#include <muParser.h>
#include <string>
#include <iostream>
#define sim2D_pi 3.141592653589793
#define sim2D_e 2.718281828459

Function::Function(){
	try{
		double pi = sim2D_pi;
		double e = sim2D_e;
		p.DefineVar("t", &t);
		p.DefineVar("pi",&pi);
		p.DefineVar("e",&e);

	}catch(Parser::exception_type &e){
		std::cout << "Error when initializing function" << std::endl;
		std::cout << e.GetMsg() << std::endl;
	}
}
void Function::setFunction(std::string function){
	try{

		p.SetExpr(function);
	}catch(Parser::exception_type &e){
		std::cout << "Error setting function" << std::endl;
		std::cout << e.GetMsg() << std::endl;

	}
}
double Function::eval(int numSteps, double stepSize){
	return 0;
}
double Function::eval(double val){
	t = val;
	return p.Eval();
}
void Function::print(){
	std::cout << p.GetExpr()<<std::endl;
}

