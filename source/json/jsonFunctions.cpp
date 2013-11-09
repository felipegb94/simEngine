/*
 * jsonFunctions.cpp
 *
 *  Created on: Nov 6, 2013
 *      Author: pipe
 */

#include "jsonFunctions.h"
void jsonFunctions::save(rapidjson::Document& doc){
	StringBuffer strbuf;
	Writer<StringBuffer> writer(strbuf);
	doc.Accept(writer);

    std::string plotsData = strbuf.GetString();


	std::ofstream dataFile ("plots/data.json");
	if (dataFile.is_open()){
	    dataFile << plotsData;
	    dataFile.close();
	}
	else std::cout << "Unable to open file" << std::endl;

}
