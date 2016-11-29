/*
 * bltl_parser.cpp
 *
 *  Created on: 1 Nov 2016
 *      Author: zhoujun
 */

#include <map>
#include <sstream>

#include <Bltl/Prd.h>
#include <Bltl/Bltl.h>

std::map<std::string, Prd*> parse_prd(std::vector<std::string> inputs) {
	std::map<std::string, Prd*> map;
	for(auto input = inputs.begin(); input !=inputs.end(); input++) {
		std::string item = *input;
		size_t begin = 0;
		size_t found = item.find(":");
		std::string k = item.substr(0, found);

		begin = found + 1;
		found = item.find(":", begin);
		std::string v = item.substr(begin, found-begin);

		begin = found + 1;
		found = item.find(":", begin);
		std::string l = item.substr(begin, found-begin);
		std::string r = item.substr(found+1);
		map[k]=new Prd(k,v,l,r);
	}
	return map;
}

void link_prd(Bltl* bltl, std::map<std::string, Prd*> prds){
	if(bltl->getOperation()==op_PRD)
		bltl->setPrd(prds[bltl->getPrdName()]);
	else{
		if(bltl->getChild1())
			link_prd(bltl->getChild1(), prds);
		if(bltl->getChild2())
			link_prd(bltl->getChild2(), prds);
	}
}

