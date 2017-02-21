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
	isFlag=false;
	if (l.compare("") == 0)
		left = new Parameter(k + ".left");
	else
		left = new Parameter(k + ".left", std::stod(l));
	if (r.compare("") == 0)
		right = new Parameter(k + ".right");
	else
		right = new Parameter(k + ".right", std::stod(r));
	left->varId = varId;
	right->varId=varId;
}

Prd::Prd(const std::string k, const std::string v, const std::string flag) {
	name = k;
	varId = std::stoi(v);
	isFlag=true;

	if(flag.compare("MAX")==0)
		this->flag = MAX;
	else if(flag.compare("MIN")==0)
		this->flag = MIN;
}

void Prd::resolveFlag(double l, double r){
	left = new Parameter(name + ".left", l);
	right = new Parameter(name + ".right", r);
	left->varId = varId;
	right->varId=varId;
}

Prd::~Prd() {
	// TODO Auto-generated destructor stub
}

