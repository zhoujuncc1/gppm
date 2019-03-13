/*
 * modelChecker.cpp
 *
 *  Created on: Feb 27, 2019
 *      Author: zhoujun
 */

#include <boost/bind.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>

#include <fstream>
#include <ctime>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <string>
#include <thread>
#include <cstdlib>

#include "ModelChecking.h"
#include "Mining.h"
#include "Bltl/bltl_parser.h"
#include "miner_utils.h"

#include "State.h"

using boost::asio::ip::tcp;

const int max_length = 1024;

typedef boost::shared_ptr<tcp::socket> socket_ptr;

void session(socket_ptr sock, State *state)
{
    try
    {
        for (;;)
        {
            char data[max_length];

            boost::system::error_code error;
            size_t length = sock->read_some(boost::asio::buffer(data), error);
            if (error == boost::asio::error::eof)
                break; // Connection closed cleanly by peer.
            else if (error)
                throw boost::system::system_error(error); // Some other error.

            data[length] = 0;
            if (data[0] == '!')
            {
                std::string str = "[";
                str+="[";
                for (auto itr = state->paramset->unknown_prd_set.begin(); itr != state->paramset->unknown_prd_set.end(); itr++)
                {
                    str += ("\"" + itr->first + ":" +std::to_string(itr->second->range.first) +":"+ std::to_string(itr->second->range.second)+":"+ std::to_string(itr->second->value)+"\"" + ',');
                }
                str+="],";
                str+="[";
                for (auto itr = state->paramset->unknown_time_set.begin(); itr != state->paramset->unknown_time_set.end(); itr++)
                {
                    str += ("\"" + itr->first + ":" +std::to_string(itr->second->range.first) +":"+ std::to_string(itr->second->range.second)+":"+ std::to_string(itr->second->value)+"\"" + ',');
                }
                str += "]]";
                boost::asio::write(*sock, boost::asio::buffer(str.c_str(), str.size()));
            }
            else
            {
                std::string str(data);
                auto values = read_property_values(str);
                for (auto itr = state->paramset->unknown_prd_set.begin(); itr != state->paramset->unknown_prd_set.end(); itr++)
                {
                    if (values.find(itr->first) != values.end())
                        itr->second->value = atof(values[itr->first].c_str());
                }
                for (auto itr = state->paramset->unknown_time_set.begin(); itr != state->paramset->unknown_time_set.end(); itr++)
                {
                    if (values.find(itr->first) != values.end())
                        itr->second->value = atoi(values[itr->first].c_str());
                }
                double score = loss(state);
                printf("score: %f\n", score);
                boost::asio::write(*sock, boost::asio::buffer(&score, sizeof(double)));
            }

            printf("%s !\n", data);
        }
    }
    catch (std::exception &e)
    {
        std::cerr << "Exception in thread: " << e.what() << "\n";
    }
}

void server(boost::asio::io_service &io_service, unsigned short port, State *state)
{
    tcp::acceptor a(io_service, tcp::endpoint(tcp::v4(), port));
    for (;;)
    {
        socket_ptr sock(new tcp::socket(io_service));
        a.accept(*sock);
        boost::thread t(boost::bind(session, sock, state));
    }
}

int main(int argc, char *argv[])
{
    try
    {
        if (argc != 3)
        {
            std::cerr << "Usage: scoreServer <inputfile> <port>\n";
            return 1;
        }

        string filename(argv[1]);
        std::ifstream f(filename);
        if (f.is_open())
            std::cout << f.rdbuf();
        f.close();
        vector<Miner *> miners = MinerBuilder::buildMiner(filename, true);
        Miner *miner = miners[0];
        State *state = new State(miner->params);
        state->prd_values = generate_prd(miner->params->tree_roots, miner->initial_state);
        state->time_values = generate_time(miner->params->unknown_time_set, miner->initial_state);

        state->trajectories = &(miner->trajectories);
        state->bltlChecker = miner->bltlChecker;

        boost::asio::io_service io_service;

        using namespace std; // For atoi.
        server(io_service, atoi(argv[2]), state);
    }
    catch (std::exception &e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
