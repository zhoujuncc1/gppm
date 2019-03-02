#include "../../include/Model.h"
#include <helper_functions.h>
#include <helper_cuda.h>
#include <cuda_runtime.h>
GPUFileTrajectoryProvider::GPUFileTrajectoryProvider(std::string filename) : FileTrajectoryProvider(filename)
    {
        dim_array[TRAJ_N_SPECIES_I] = N_SPECIES;
        dim_array[TRAJ_END_TIME_I] = end_time;
        dim_array[TRAJ_SIZE_I] = size;

        trajectories_host = (float *)malloc(size * N_SPECIES * 2 * end_time);

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
        error = cudaMalloc((void **)&trajectories_device, 2 * N_SPECIES * end_time * size * sizeof(float));

        if (error != cudaSuccess)
        {
            printf("cudaMalloc trajectories_device returned error %s (code %d), line(%d)\n", cudaGetErrorString(error), error, __LINE__);
            exit(EXIT_FAILURE);
        }

        std::ifstream input(filename);
        input >> N_SPECIES;
        input >> end_time;
        input >> size;

        float *trajectory_host = (float *)malloc(2 * N_SPECIES * end_time * sizeof(float));
        cudaError_t error;

        for (int k = 0; k < size; k++)
        {
            int index = 0;
            GPUTrajectory traj(N_SPECIES, end_time);
            error = cudaMalloc((void **)&(traj.traj_device), 2 * N_SPECIES * end_time * sizeof(float));
            if (error != cudaSuccess)
            {
                printf("cudaMalloc traj.traj_device returned error %s (code %d), line(%d)\n", cudaGetErrorString(error), error, __LINE__);
                exit(EXIT_FAILURE);
            }

            {
                state_type state;
                for (int i = 0; i < N_SPECIES; i++)
                {
                    double v;
                    input >> v;
                    trajectory_host[index++] = v;
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
                double v;
                for (int i = 0; i < N_SPECIES; i++)
                {
                    input >> v;
                    trajectory_host[index++] = v;
                    state.push_back(v);
                }
                for (int i = 0; i < N_SPECIES; i++)
                {
                    state.push_back(state[i] - traj.m_states.back()[i]);
                }

                traj.m_states.push_back(state);
                traj.m_times.push_back(t);
            }

            error = cudaMemcpy(traj.traj_device, trajectory_host, 2 * N_SPECIES * end_time * sizeof(float), cudaMemcpyHostToDevice);

            if (error != cudaSuccess)
            {
                printf("cudaMemcpy (traj.traj_device, trajectory_host) returned error %s (code %d), line(%d)\n", cudaGetErrorString(error), error, __LINE__);
                exit(EXIT_FAILURE);
            }
            trajectories.push_back(traj);
            memcpy(trajectories_host + k * 2 * N_SPECIES * end_time, trajectory_host, 2 * N_SPECIES * end_time * sizeof(float));
        }
        error = cudaMemcpy(trajectories_device, trajectories_host, size * 2 * N_SPECIES * end_time * sizeof(float), cudaMemcpyHostToDevice);

        if (error != cudaSuccess)
        {
            printf("cudaMemcpy (traj.traj_device, trajectory_host) returned error %s (code %d), line(%d)\n", cudaGetErrorString(error), error, __LINE__);
            exit(EXIT_FAILURE);
        }
        input.close();
    }