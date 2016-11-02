/*
 * Parameter.cpp
 *
 *  Created on: 1 Nov 2016
 *      Author: zhoujun
 */

#include "../../include/Bltl/Parameter.h"

#include <iostream>

Parameter::Parameter(std::string k):name(k) {
	isfix=false;
}

Parameter::Parameter(std::string k, double v):name(k), value(v) {
	isfix=true;
}

Parameter::~Parameter() {
	// TODO Auto-generated destructor stub
}

