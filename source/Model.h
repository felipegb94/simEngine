/*
 * model.h
 *
 *  Created on: Sep 23, 2013
 *      Author: pipe
 */

#ifndef MODEL_H_
#define MODEL_H_
#include <vector>
#include <string>
#include "Body.h"
#include "c_constraint.h"
typedef rapidjson::GenericDocument<rapidjson::UTF8<> > MyJsonDocument;


class Model{
private:
	std::string name;
	std::vector<Body> bodies;
	std::vector<c_constraint*> constraints;
	std::vector<double> gravity;
public:

	Model(MyJsonDocument& d);
	//input could be a json file?
	const std::vector<Body>& getBodies(){return bodies;}
	const std::vector<c_constraint*>& getConstraints(){return constraints;}
	std::string getName(){return name;}

};
#endif /* MODEL_H_ */
