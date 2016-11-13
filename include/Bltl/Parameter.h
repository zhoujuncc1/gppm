/*
 * Parameter.h
 *
 *  Created on: 1 Nov 2016
 *      Author: zhoujun
 */

#ifndef PARAMETER_H_
#define PARAMETER_H_
#include <string>
#include <utility>
#include <list>
class Parameter {
public:
	Parameter();
	Parameter(std::string k);
	Parameter(std::string k, double v);

	virtual ~Parameter();
	std::string name;
	double value;
	bool isfix;
	std::pair<double, double> range;

	std::list<Parameter*> parents;
	std::list<Parameter*> children;

};

#endif /* PARAMETER_H_ */
