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

class Prd {
public:
	Prd(std::string k, std::string v, std::string l, std::string r);
	virtual ~Prd();
	std::string name;
	int varId;
	Parameter* left;
	Parameter* right;
};

#endif /* PRD_H_ */
