/*
 * ParameterSet.h
 *
 *  Created on: 2 Nov 2016
 *      Author: zhoujun
 */

#ifndef SRC_PARAMETERSET_H_
#define SRC_PARAMETERSET_H_

class ParameterSet {
public:
	ParameterSet(Bltl* bltl);
	virtual ~ParameterSet();

	std::map<std::string, Prd*> unknown_prd_set;
	std::map<std::string, Prd*> known_prd_set;
	std::map<std::string, Prd*> unknown_time_set;
	std::map<std::string, Prd*> known_time_set;
	std::map<std::string, Prd*> all_set;


private:
	void findParameters(Bltl* bltl);
	Model* model;
};

#endif /* SRC_PARAMETERSET_H_ */
