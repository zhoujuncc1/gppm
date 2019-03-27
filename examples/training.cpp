//
// Created by Zhou Jun on 20/11/16.
//

#include "Mining.h"
#include <fstream>
#include <iostream>

int main(int argc, char** argv)
{
    if (argc < 2) {
        printf("ERROR: must give input file name!\n");
        exit(1);
    }
    std::string infilename(argv[1]);
    std::string outfilename(argv[2]);

    vector<Miner*> miners = MinerBuilder::buildMiner(infilename);
    for (auto itr = miners.begin(); itr != miners.end(); itr++) {
        (*itr)->mine();
        ofstream myfile;
        myfile.open(outfilename);
        myfile << (*itr)->state->to_string() << endl;
        myfile.close();
    }

    return 0;
}
