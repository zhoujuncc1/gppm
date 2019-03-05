#include "../../include/Model.h"
#include <helper_functions.h>
#include <helper_cuda.h>
#include <cuda_runtime.h>
GPUFileTrajectoryProvider::GPUFileTrajectoryProvider(std::string filename)
    {

      printf("Enter GPU Traj Provider!\n");
        std::ifstream input(filename);
        input >> N_SPECIES;
        input >> end_time;
        input >> size;
    
        dim_array[TRAJ_N_SPECIES_I] = N_SPECIES;
        dim_array[TRAJ_END_TIME_I] = end_time;
        dim_array[TRAJ_SIZE_I] = size;


        cudaError_t error;
        error = cudaMalloc((void **)&dim_array_device, 3 * sizeof(int));
        if (error != cudaSuccess)
        {
            printf("cudaMalloc dim_array_device returned error %s (code %d), line(%d)\n", cudaGetErrorString(error), error, __LINE__);
            exit(EXIT_FAILURE);
        }

        error = cudaMemcpy(dim_array_device, dim_array, 3 * sizeof(float), cudaMemcpyHostToDevice);

        if (error != cudaSuccess)
        {
            printf("cudaMemcpy (dim_array_device, dim_array) returned error %s (code %d), line(%d)\n", cudaGetErrorString(error), error, __LINE__);
            exit(EXIT_FAILURE);
        }

        trajectories_host = (float *)malloc(size * N_SPECIES * 2 * end_time*sizeof(float));
        error = cudaMalloc((void **)&trajectories_device, 2 * N_SPECIES * end_time * size * sizeof(float));

        if (error != cudaSuccess)
        {
            printf("cudaMalloc trajectories_device returned error %s (code %d), line(%d)\n", cudaGetErrorString(error), error, __LINE__);
            exit(EXIT_FAILURE);
        }

        int index = 0;

        for (int k = 0; k < size; k++)
        {
            Trajectory traj(N_SPECIES, end_time);

            {
                state_type state;
                for (int i = 0; i < N_SPECIES; i++)
                {
                    float v;
                    input >> v;
                    trajectories_host[index++] = v;
                    state.push_back(v);
                }
                for (int i = 0; i < N_SPECIES; i++)
                {
                    state.push_back(0);
                    trajectories_host[index++] = 0;
                }

                traj.m_states.push_back(state);
                traj.m_times.push_back(0);
            }
            for (int t = 1; t < end_time; t++)
            {
                state_type state;
                for (int i = 0; i < N_SPECIES; i++)
                {
                    float v;
                    input >> v;
                    trajectories_host[index++] = v;
                    state.push_back(v);
                }
                for (int i = 0; i < N_SPECIES; i++)
                {
                    state.push_back(state[i] - traj.m_states.back()[i]);
                    trajectories_host[index++] = state[i] - traj.m_states.back()[i];
                }

                traj.m_states.push_back(state);
                traj.m_times.push_back(t);
            }
            trajectories.push_back(traj);

        }
        error = cudaMemcpy(trajectories_device, trajectories_host, size * 2 * N_SPECIES * end_time * sizeof(float), cudaMemcpyHostToDevice);

        if (error != cudaSuccess)
        {
            printf("cudaMemcpy (traj.trajectories_device, trajectories_host) returned error %s (code %d), line(%d)\n", cudaGetErrorString(error), error, __LINE__);
            exit(EXIT_FAILURE);
        }
        input.close();
    }
