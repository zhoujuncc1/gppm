//
// Created by Zhou Jun on 20/11/16.
//

#include <iostream>
#include <fstream>
#include <string>
#include "Mining.h"


int main(int argc, char** argv){
    if(argc<2){
        printf("ERROR: must give input file name!\n");
        exit(1);
    }
    std::string infilename(argv[1]);
    std::string outfilename(argv[2]);

    Miner* miner = MinerBuilder::buildMiner(infilename);
    for(int i =2; i < argc; i+=2){
        std::string key(argv[i]);
        miner->params->weights[key] = atof(argv[i+1]);
    }
    miner->mine();
    
    ofstream myfile;
    myfile.open (outfilename);
    myfile<<miner->state->to_string();
    myfile.close();

    return 0;

}
