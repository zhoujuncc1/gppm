/*
 * Prd.h
 *
 *  Created on: 1 Nov 2016
 *      Author: zhoujun
 */

#ifndef PRD_H_
#define PRD_H_

#include <iostream>
#include "Parameter.h"

enum FLAG {MAX, MIN, T0};

class Prd {
public:
	Prd(std::string k, std::string v, std::string l, std::string r);
	Prd(std::string k, std::string v, std::string flag);
	void resolveFlag(double l, double r);
	virtual ~Prd();
	std::string name;
	int varId;
	Parameter* left;
	Parameter* right;
	FLAG flag;
	bool isFlag;
    float weight_sign=0;
    bool* value_array_device_default_true;
    bool* value_array_device_default_false;

};

#endif /* PRD_H_ */
