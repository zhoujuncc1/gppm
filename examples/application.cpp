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
    std::string filename(argv[1]);
    Miner miner(filename);
    miner.mine();
    std::cout<<miner.state->to_string();

    return 0;

}