/*
 * myAverageStdDev.cpp
 *
 *  Created on: Sep 17, 2013
 *      Author: pipe
 */
#include <armadillo>
#include <string>
#include <muParser.h>
#include <vector>
#include "../includes/averageStdDev.h"

using namespace std;
//using namespace arma;
using namespace mu;

vector<double> myAverageStdDev(string function)
{
	vector<double>averageStdDev(2);
		arma::vec myVec(101);

		try{
			double val = 0;
			Parser p;
			double pi = 3.1416;
			p.DefineVar("t", &val);
			p.DefineVar("pi",&pi);
			p.SetExpr(function);
			for(int i = 0; i < 101 ; i++){
				myVec(i) = p.Eval();
				val += 0.1;
			}
		}catch(Parser::exception_type &e){
			cout << e.GetMsg() << endl;
		}

		averageStdDev[0] = arma::mean(myVec);
		averageStdDev[1] = arma::stddev(myVec);


		return averageStdDev;
}
