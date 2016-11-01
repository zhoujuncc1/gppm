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
class Parameter {
public:
	Parameter(std::string k);
	Parameter(std::string k, double v);

	virtual ~Parameter();
	std::string name;
	double value;
	bool isfix;
	std::pair range;

};

#endif /* PARAMETER_H_ */
