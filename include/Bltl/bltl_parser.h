/*
 * bltl_parser.h
 *
 *  Created on: 31 Oct 2016
 *      Author: zhoujun
 */

#ifndef BLTL_PARSER_H_
#define BLTL_PARSER_H_

#include <iostream>
#include <map>

#include "Prd.h"
#include "Bltl.h"

Bltl* parse_bltl(std::string str);

std::map<std::string, Prd*> parse_prd(std::string str);

void linkPrds(Bltl* bltl, std::map<std::string, Prd*> &prds);
#endif /* BLTL_PARSER_H_ */
