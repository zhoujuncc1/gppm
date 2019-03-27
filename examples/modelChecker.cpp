/*
 * modelChecker.cpp
 *
 *  Created on: Feb 27, 2019
 *      Author: zhoujun
 */

#include "ModelChecking.h"
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
    vector<ModelChecker*> checkers = ModelChecker::buildChecker(filename);
    vector<int> n_pass;
    for (auto itr = checkers.begin(); itr != checkers.end(); itr++) {
        vector<int> results = (*itr)->check();
        int sum = 0;
        for (int i = 0; i < results.size(); i++)
            sum += results[i];
        n_pass.push_back(sum);
    }
    for (int i = 0; i < n_pass.size(); i++)
        cout << n_pass[i] << endl;
    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    cout << "Time: " << elapsed_secs << endl;
    return 0;
}
