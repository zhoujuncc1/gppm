/*
 * ParameterSet.h
 *
 *  Created on: 2 Nov 2016
 *      Author: zhoujun
 */

#ifndef SRC_PARAMETERSET_H_
#define SRC_PARAMETERSET_H_
#include "Bltl/Bltl.h"
#include <map>
class ParameterSet {
public:
	ParameterSet(Bltl* bltl);
	virtual ~ParameterSet();

	std::map<std::string, Parameter*> unknown_prd_set;
	std::map<std::string, Parameter*> known_prd_set;
	std::map<std::string, TimeVariable*> unknown_time_set;
	std::map<std::string, TimeVariable*> known_time_set;
	std::map<std::string, Parameter*> all_set;


private:
	Bltl* bltl;
	void findParameters(Bltl* bltl);
	void init_time_range();
};

#endif /* SRC_PARAMETERSET_H_ */
