/*
 * ParameterSet.h
 *
 *  Created on: 2 Nov 2016
 *      Author: zhoujun
 */

#ifndef SRC_PARAMETERSET_H_
#define SRC_PARAMETERSET_H_
#include "Bltl/Bltl.h"
#include "Model.h"
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

	std::vector<std::pair<double, double> > ranges;
    void init_prd_range();
    //prase to trees where smallest are roots
    void parse_constraint_tree(std::string input);
    //Smallest are roots
    std::vector<Parameter*> tree_roots;

private:
	Bltl* bltl;
	void findParameters(Bltl* bltl);
	void init_time_range();
};

#endif /* SRC_PARAMETERSET_H_ */
