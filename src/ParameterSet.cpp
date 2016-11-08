/*
 * ParameterSet.cpp
 *
 *  Created on: 2 Nov 2016
 *      Author: zhoujun
 */

#include "../include/ParameterSet.h"
#include "../include/Bltl/Bltl.h"
#include "../include/Bltl/TimeVariable.h"
#include "../include/Model.h"


ParameterSet::ParameterSet(Bltl* bltl) {
	this->bltl = bltl;
	findParameters(bltl);

}

ParameterSet::~ParameterSet() {
	// TODO Auto-generated destructor stub
}

void ParameterSet::findParameters(Bltl* bltl){
	if(bltl->getTime()){
		if(bltl->getTime()->isfix)
			known_time_set[bltl->getTime()->name]=bltl->getTime();
		else
			unknown_time_set[bltl->getTime()->name]=bltl->getTime();
		all_set[bltl->getTime()->name]=bltl->getTime();
	}

	if(bltl->getOperation()==op_PRD){
		if(bltl->getPrd()->left->isfix)
			known_prd_set[bltl->getPrd()->left->name]=bltl->getPrd()->left;
		else
			unknown_prd_set[bltl->getPrd()->left->name]=bltl->getPrd()->left;

		all_set[bltl->getPrd()->left->name]=bltl->getPrd()->left;

		if(bltl->getPrd()->right->isfix)
			known_prd_set[bltl->getPrd()->right->name]=bltl->getPrd()->right;
		else
			unknown_prd_set[bltl->getPrd()->right->name]=bltl->getPrd()->right;

		all_set[bltl->getPrd()->right->name]=bltl->getPrd()->right;
	}
	if(bltl->getChild1())
		findParameters(bltl->getChild1());
	if(bltl->getChild2())
		findParameters(bltl->getChild2());
}
