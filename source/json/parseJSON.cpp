/*
 * parseJSON.cpp
 *
 *  Created on: Sep 23, 2013
 *      Author: pipe
 */

//#include "Symbolic/headers/symbolic/symbolicc++.h"
#include "../includes/rapidjson/document.h"
#include "jsonParser.h"
using namespace std;


/**
 * The following function makes use of the rapidjson library to parse a json string that is
 * read from a file. It returns a Document object, which is defined in the rapidjson library.
 * Note: A document object is very similar to an associative array.
 *
 */
rapidjson::Document parseJSON(string filename){

	string line;
	string json;
	ifstream myfile (filename.c_str()); //file to read.
	if (myfile.is_open()){
		while (getline (myfile,line) ){
			json += line;
		}
		    myfile.close();
		}
	else cout << "Unable to open file";

    rapidjson::Document d;
    d.Parse<0>(json.c_str());

	return d;

}

