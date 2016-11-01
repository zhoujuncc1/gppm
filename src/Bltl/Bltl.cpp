/*
 * Bltl.cpp
 *
 *  Created on: 27 Oct 2016
 *      Author: zhoujun
 */
#include <sstream>

#include "Bltl/Bltl.h"

Bltl::Bltl(Operation op, TimeVariable *t){
	operation=op;
	time=t;
	child1=child2=NULL;
}

Bltl::Bltl(Operation op, char* p) :
		prd(std::string(p)), operation(op) {
	child1=child2=NULL;
}

Bltl::~Bltl() {
	// TODO Auto-generated destructor stub
}

void Bltl::setChild1(Bltl* c1) {
	child1 = c1;
}
void Bltl::setChild2(Bltl* c2) {
	child2 = c2;
}

std::string Bltl::getPrd() {
	return prd;
}
std::string Bltl::getOpString() {
	switch (operation) {
	case op_PRD:
		return "PRD";
	case op_F:
		return "F";
	case op_G:
		return "G";
	case op_NOT:
		return "~";
	case op_X:
		return "X";
	case op_AND:
		return "&";
	case op_OR:
		return "|";
	case op_U:
		return "U";
	}
}

Bltl* Bltl::getChild1() {
	return child1;
}
Bltl* Bltl::getChild2() {
	return child2;
}
std::string Bltl::to_string() {
	std::stringstream ss;
	switch (operation) {
	case op_PRD:
		ss << prd;
		break;
	case op_F:
	case op_G:
	case op_NOT:
	case op_X:
		ss << getOpString();
		ss << '(' << getChild1()->to_string() << ')';
		break;
	case op_AND:
	case op_OR:
	case op_U:
		ss << '(' << getChild1()->to_string() << ')';
		ss << getOpString();
		ss << '(' << getChild2()->to_string() << ')';
		break;
	}
	return ss.str();
}
