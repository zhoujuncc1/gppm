/*
 * Bltl.cpp
 *
 *  Created on: 27 Oct 2016
 *      Author: zhoujun
 */

#include "Bltl.h"

Bltl::Bltl(Operation operation, TimeVariable *t):operation(operation),time(t){}

Bltl::Bltl(Operation operation, char* prd):prd(prd),operation(operation){}


Bltl::~Bltl() {
	// TODO Auto-generated destructor stub
}

void Bltl::setChild1(Bltl* c1){
	child1=c1;
}
void Bltl::setChild2(Bltl* c2){
	child2=c2;
}
