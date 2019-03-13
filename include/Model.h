/*
 * Model.h
 *
 *  Created on: 2 Nov 2016
 *      Author: zhoujun
 */

#ifndef SRC_BLTL_MODEL_H_
#define SRC_BLTL_MODEL_H_
#include <vector>
#include <cstdlib>
#include <ctime>
#include <boost/numeric/odeint/integrate/integrate.hpp>
#include <boost/random.hpp>
#include <fstream>
using namespace boost::numeric::odeint;

typedef std::vector<float> state_type;

typedef boost::uniform_real<> NumberDistribution;
typedef boost::mt19937 RandomNumberGenerator;
typedef boost::variate_generator<RandomNumberGenerator &, NumberDistribution> Generator;
#define TRAJ_N_SPECIES_I 0
#define TRAJ_END_TIME_I 1
#define TRAJ_SIZE_I 2

class Trajectory
{
  public:
    Trajectory(int N_SPECIES, double dt)
    {
        this->N_SPECIES = N_SPECIES;
        this->dt = dt;
    }
    double dt;
    int N_SPECIES;
    std::vector<state_type> m_states;
    std::vector<float> m_times;
};


class FileTrajectoryProvider
{
  public:
    FileTrajectoryProvider(){}
    FileTrajectoryProvider(std::string filename)
    {
        std::ifstream input(filename);
        input >> N_SPECIES;
        input >> end_time;
        input >> size;
        for (int k = 0; k < size; k++)
        {
            Trajectory traj(N_SPECIES, end_time);
            {
                state_type state;
                for (int i = 0; i < N_SPECIES; i++)
                {
                    float v;
                    input >> v;
                    state.push_back(v);
                }
                for (int i = 0; i < N_SPECIES; i++)
                {
                    state.push_back(0);
                }

                traj.m_states.push_back(state);
                traj.m_times.push_back(0);
            }
            for (int t = 1; t < end_time; t++)
            {
                state_type state;
                float v;
                for (int i = 0; i < N_SPECIES; i++)
                {
                    input >> v;
                    state.push_back(v);
                }
                for (int i = 0; i < N_SPECIES; i++)
                {
                    state.push_back(state[i] - traj.m_states.back()[i]);
                }

                traj.m_states.push_back(state);
                traj.m_times.push_back(t);
            }
            trajectories.push_back(traj);
        }
        input.close();
        /*
        std::ofstream output("testdata.txt");
        output << N_SPECIES << " "<<end_time << " " <<size<<"\n";
        for(int k = 0; k < size; k++){
            for(int t = 0 ; t < end_time; t++){
                for(int i = 0; i < N_SPECIES; i++){
                    output<<trajectories[k].m_states[t][i];
                    if(i < N_SPECIES-1)
                        output << " ";
                }
                output << "\n";
            }
        }
        output.close();
        */
    }

    std::vector<Trajectory> getTrajectories(int size)
    {
	if(size > this->size)
		size=this->size;
        std::vector<Trajectory> trajs;
        for (int i = 0; i < size; i++)
            trajs.push_back(trajectories[i]);
        return trajs;
    }

    int size;
    int N_SPECIES;
    int end_time;
    std::vector<Trajectory> trajectories;
};

class GPUFileTrajectoryProvider : public FileTrajectoryProvider
{
  public:
    GPUFileTrajectoryProvider(std::string filename);

    float *getTrajectories_device()
    {
        return trajectories_device;
    }

    float *getTrajectories_host()
    {
        return trajectories_host;
    }

    int dim_array[3];
    int *dim_array_device;
    float *trajectories_device;
    float *trajectories_host;
};
#endif /* SRC_BLTL_MODEL_H_ */
