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

enum FLAG {MAX, MIN};

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
	
};

#endif /* PRD_H_ */
