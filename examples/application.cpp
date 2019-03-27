//
// Created by Zhou Jun on 20/11/16.
//

#include "Mining.h"
#include <ctime>
#include <fstream>
#include <iostream>

int main(int argc, char** argv)
{
    using namespace std;
    if (argc < 2) {
        printf("ERROR: must give input file name!\n");
        exit(1);
    }
    clock_t begin = clock();
    string filename(argv[1]);
    std::ifstream f(filename);
    if (f.is_open())
        std::cout << f.rdbuf();
    f.close();
    vector<Miner*> miners = MinerBuilder::buildMiner(filename, false);
    for (auto itr = miners.begin(); itr != miners.end(); itr++)
        (*itr)->mine();
    for (auto itr = miners.begin(); itr != miners.end(); itr++)
        cout << (*itr)->to_string() << endl;
    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    cout << "Time: " << elapsed_secs << endl;
    return 0;
}
