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
    ParameterSet(Bltl* bltl, std::map<std::string, Prd*> prds);

    std::map<std::string, Parameter*> unknown_prd_set;
    std::map<std::string, Parameter*> known_prd_set;
    std::map<std::string, TimeVariable*> unknown_time_set;
    std::map<std::string, TimeVariable*> known_time_set;
    std::map<std::string, Parameter*> all_set;

    //Smallest are roots
    std::vector<Parameter*> tree_roots;
    std::map<std::string, double> weights;
    std::map<std::string, Prd*> prds;

    void init_prd_range();
    void init_time_range();

    //prase to trees where smallest are roots
    void parse_constraint_tree(std::vector<std::string> inputs);

    void parse_weight(std::vector<std::string> inputs);
    Bltl* bltl;

 private:

    std::vector<std::pair<double, double> > ranges;
    std::vector<std::pair<double, double> > min;
    std::vector<std::pair<double, double> > max;
    
    void init_traj_ranges();
    void findParameters(Bltl* bltl);
    void resolveFlags(std::map<std::string, Prd*> prds);
};

#endif /* SRC_PARAMETERSET_H_ */
