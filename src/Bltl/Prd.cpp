/*
 * Prd.cpp
 *
 *  Created on: 1 Nov 2016
 *      Author: zhoujun
 */

#include <Bltl/Prd.h>
#include <Bltl/Parameter.h>

#include <string>


Prd::Prd(const std::string k, const std::string v, const std::string l,
		const std::string r) {
	name = k;
	varId = std::stoi(v);
	if (l.compare("") == 0)
		left = new Parameter(v + ".left");
	else
		left = new Parameter(v + ".left", std::stod(l));
	if (r.compare("") == 0)
		right = new Parameter(v + ".right");
	else
		right = new Parameter(v + ".right", std::stod(l));
}

Prd::~Prd() {
	// TODO Auto-generated destructor stub
}

