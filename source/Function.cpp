/*
 * Function.cpp
 *
 *  Created on: Sep 26, 2013
 *      Author: pipe
 */
#include "Function.h"



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
/*	cout <<"SymbolicC++ Test" <<endl;
	 Symbolic alpha("alpha");
	 Symbolic X, XI, dX, result;

	 X = ( ( cos(alpha), sin(alpha) ),
	       (-sin(alpha), cos(alpha) ) );

	 cout << X << endl;

	 XI = X[alpha == -alpha]; cout << XI << endl;
	 dX = df(X, alpha);       cout << dX << endl;

	 result = XI * dX;        cout << result << endl;
	 result = result[(cos(alpha)^2) == 1 - (sin(alpha)^2)];
	 cout << result << endl;*/
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


