/*
 * bltl_parser.h
 *
 *  Created on: 31 Oct 2016
 *      Author: zhoujun
 */

#ifndef BLTL_PARSER_H_
#define BLTL_PARSER_H_

#include "Bltl.h"
#include "Prd.h"
#include <iostream>
#include <map>
#include <string>
#include <vector>
Bltl* parse_bltl(std::string str);

std::map<std::string, Prd*> parse_prd(std::vector<std::string> inputs);

void link_prd(Bltl* bltl, std::map<std::string, Prd*> prds);

std::map<std::string, std::string> read_property_values(std::string input);

#endif /* BLTL_PARSER_H_ */
