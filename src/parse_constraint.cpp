//
// Created by Zhou Jun on 14/11/16.
//

#include "../include/parse_constraint.h"
#include <sstream>

vector<pair<string, string> > parse_constraint(string str){
    vector<pair<string, string> > constraints;

    stringstream ss;
    ss.str(str);
    string item;
    while (getline(ss, item)) {
        size_t begin = 0;
        size_t found;
        string low, high;
        if((found=item.find("<"))!=string::npos) {
            low = item.substr(0, found);
            high = item.substr(found + 1, string::npos);
        }
        else if((found=item.find(">"))!=string::npos) {
            high = item.substr(0, found);
            low = item.substr(found + 1, string::npos);
        }

        constraints.push_back(pair<string, string>(low, high));
    }
    return constraints;
}
