/*
 * Function.cpp
 *
 *  Created on: Sep 26, 2013
 *      Author: pipe
 */
#include "symbolicc++.h"
#include "Function.h"
#include <armadillo>


#define sim2D_pi 3.141592653589793
#define sim2D_e 2.718281828459

Function::Function(){
	try{
		double e = sim2D_e;
		p.DefineVar("t", &t);
		p.DefineVar("x", &x);
		p.DefineVar("y", &y);
		p.DefineVar("phi", &anglePhi);
		p.DefineConst("pi",sim2D_pi);
		p.DefineConst("e",sim2D_e);

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

double Function::eval(double val){
	t = val;

	return p.Eval();
}
double Function::eval(double time, double angle){
	t = time;
	anglePhi = angle;
	return p.Eval();
}
double Function::evalX(double time, double xPos){
	x = xPos;
	t = time;
	return p.Eval();
}

double Function::eval2(double yVal){

	y = yVal;
	return p.Eval();
}
void Function::print(){
	std::cout << p.GetExpr()<<std::endl;
}
std::string Function::getString(){
	return p.GetExpr();
}
void Function::testSymbolic(){
	/**
	std::cout << "Testing SymbolicC++" <<std::endl;
	Symbolic zero;
	std::cout << zero << std::endl;
	string function = "t^2 + cos(2*t)";
	Symbolic t("t");
	Symbolic f = t^2 + cos(2*t) ;
	//Symbolic f = function;


	std::cout << f.subst_all(t==12) <<std::endl;
	std::cout << df(f,t) <<std::endl;
	*/
}


