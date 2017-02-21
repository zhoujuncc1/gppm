//
// Created by Zhou Jun on 20/11/16.
//

#include <iostream>
#include <fstream>
#include "Mining.h"

int main(int argc, char** argv){
    if(argc<2){
        printf("ERROR: must give input file name!\n");
        exit(1);
    }
    string filename(argv[1]);
    vector<Miner*> miners = MinerBuilder::buildMiner(filename);
    for(auto itr = miners.begin(); itr!=miners.end(); itr++)
        (*itr)->mine();
    for(auto itr = miners.begin(); itr!=miners.end(); itr++)
        cout<<(*itr)->to_string()<<endl;


    return 0;

}
