/*
 * Bltl.h
 *
 *  Created on: 27 Oct 2016
 *      Author: zhoujun
 */

#ifndef BLTL_BLTL_H_
#define BLTL_BLTL_H_

#include <cstdlib>
#include <iostream>

class TimeVariable;

enum Operation {op_F, op_G, op_U, op_X, op_NOT, op_AND, op_OR, op_PRD};
class Bltl {
public:
	Bltl(Operation op, TimeVariable *t=NULL);
	Bltl(Operation op, char* p);

	virtual ~Bltl();

	void setChild1(Bltl* c1);
	void setChild2(Bltl* c2);
	void setPrd(Prd* p);
	Bltl* getChild1();
	Bltl* getChild2();
	std::string getPrdName();
	Prd* getPrd();
	std::string getOpString();
	TimeVariable* getTime();
	std::string to_string();


private:
	Operation operation;
	TimeVariable *time;
	Bltl* child1;
	Bltl* child2;
	std::string prd_name;
	Prd* prd;

};

#endif /* BLTL_BLTL_H_ */
