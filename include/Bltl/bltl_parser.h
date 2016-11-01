/*
 * bltl_parser.h
 *
 *  Created on: 31 Oct 2016
 *      Author: zhoujun
 */

#ifndef BLTL_PARSER_H_
#define BLTL_PARSER_H_
#include <map>
#include "Bltl.h"
#include "Prd.h"
Bltl* parse_bltl(std::string str);

std::map<std::string, Prd*> parse_prd(std::string str);
#endif /* BLTL_PARSER_H_ */
