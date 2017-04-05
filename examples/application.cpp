//
// Created by Zhou Jun on 20/11/16.
//

#include <iostream>
#include <fstream>
#include <ctime>
#include "Mining.h"

int main(int argc, char** argv){
    using namespace std;
    if(argc<2){
        printf("ERROR: must give input file name!\n");
        exit(1);
    }
    clock_t begin = clock();
    string filename(argv[1]);
    vector<Miner*> miners = MinerBuilder::buildMiner(filename);
    for(auto itr = miners.begin(); itr!=miners.end(); itr++)
        (*itr)->mine();
    for(auto itr = miners.begin(); itr!=miners.end(); itr++)
        cout<<(*itr)->to_string()<<endl;
    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    cout << "Time: " << elapsed_secs << endl;
    return 0;

}
