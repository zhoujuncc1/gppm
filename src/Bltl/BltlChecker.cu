
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cuda_runtime.h>
#include "../../include/helper_functions.h"
#include "../../include/helper_cuda.h"

#include "../../include/Bltl/BltlChecker.h"
#include "../../include/Bltl/Bltl.h"

__global__ void eval_prd_kernel(float *traj, bool *value, int varId, float left, float right, int *dim_array_device)
{
	int n = threadIdx.x;
    int t = threadIdx.y;
	int index = n*2*dim_array_device[TRAJ_N_SPECIES_I]*dim_array_device[TRAJ_END_TIME_I] +t*2*dim_array_device[TRAJ_N_SPECIES_I]+varId;
	value[t] = traj[index]<=right && traj[index]>=left;
}




__global__ void eval_G_kernel(bool* value_arr, bool* sub_value_arr, int distance, int *dim_array_device){
	int n = threadIdx.x;
    int t = threadIdx.y;
    int index = n*dim_array_device[TRAJ_END_TIME_I]+t;
    value_arr[t] = true;
	for(int k = 0 ; k < distance && t+k<dim_array_device[TRAJ_END_TIME_I]; k++){
	    value_arr[index] = value_arr[index] && sub_value_arr[index+k];
	}
}

__global__ void eval_F_kernel(bool* value_arr, bool* sub_value_arr, int distance, int *dim_array_device){
	int n = threadIdx.x;
    int t = threadIdx.y;
    int index = n*dim_array_device[TRAJ_END_TIME_I]+t;
	value_arr[t] = false;
	for(int k = 0 ; k < distance && t+k<dim_array_device[TRAJ_END_TIME_I]; k++){
	    value_arr[index] = value_arr[index] || sub_value_arr[index+k];
	}
}

void GPUBltlChecker::eval_prds()
{
	dim3 threadsPerBlock_prd(traj_provider->dim_array[TRAJ_SIZE_I], length);
	
	for (auto prd_itr = prds.begin(); prd_itr != prds.end(); prd_itr++)
	{
		Prd *prd = prd_itr->second;
		eval_prd_kernel<<<1, threadsPerBlock_prd>>>(traj_provider->trajectories_device, prd->value_array_device, prd->varId, prd->left->value, prd->right->value, dim_array_device);
	}
	cudaDeviceSynchronize();
}

bool* GPUBltlChecker::eval_bltl_recursive(Bltl *bltl)
{
    if(bltl->getOperation() == op_PRD)
        return bltl->getPrd()->value_array_device;
	else
	{
        bool* sub_value_arr = eval_bltl_recursive(bltl->getChild1());
		cudaError_t error = cudaMalloc((void **)&(bltl->value_array_device), traj_provider->dim_array[TRAJ_SIZE_I]*length * sizeof(bool));
		if (error != cudaSuccess)
		{
			printf("cudaMalloc bltl->value_array_device returned error %s (code %d), line(%d)\n", cudaGetErrorString(error), error, __LINE__);
			exit(EXIT_FAILURE);
		}
		dim3 threadsPerBlock(traj_provider->dim_array[TRAJ_SIZE_I], length);
        if(bltl->getOperation() == op_G)
            eval_G_kernel<<<1, threadsPerBlock>>>(bltl->value_array_device, sub_value_arr, bltl->getTime()->value, dim_array_device);
        else if(bltl->getOperation() == op_F)
            eval_F_kernel<<<1, threadsPerBlock>>>(bltl->value_array_device, sub_value_arr, bltl->getTime()->value, dim_array_device);
        cudaDeviceSynchronize();
        return bltl->value_array_device;
	}
}
vector<int> GPUBltlChecker::eval_bltl(Bltl *bltl){
    eval_bltl_recursive(bltl);
    bool* result_arr = (bool*) malloc(traj_provider->dim_array[TRAJ_SIZE_I]*length);

    cudaError_t error = cudaMemcpy(result_arr, bltl->value_array_device, traj_provider->dim_array[TRAJ_SIZE_I]*length * sizeof(bool), cudaMemcpyDeviceToHost);

    if (error != cudaSuccess)
    {
        printf("cudaMemcpy (traj.traj_device, trajectory_host) returned error %s (code %d), line(%d)\n", cudaGetErrorString(error), error, __LINE__);
        exit(EXIT_FAILURE);
    }
    
    vector<int> results;
    for(int i = 0; i < traj_provider->dim_array[TRAJ_SIZE_I]; i++)
        results.push_back(result_arr[i*length]);
    return results;
}

GPUBltlChecker::GPUBltlChecker(Bltl *bltl, map<string, Prd *> prds, GPUFileTrajectoryProvider* traj_provider)
	{
        isGPU=true;
		this->bltl=bltl;
		this->prds=prds;
		this->traj_provider = traj_provider;
		cudaError_t error;
		for (auto prd_itr = prds.begin(); prd_itr != prds.end(); prd_itr++)
		{
			Prd *prd = prd_itr->second;
			error = cudaMalloc((void **)&(prd->value_array_device), traj_provider->dim_array[TRAJ_SIZE_I]*length * sizeof(bool));
			if (error != cudaSuccess)
			{
				printf("cudaMalloc prd->value_array_device returned error %s (code %d), line(%d)\n", cudaGetErrorString(error), error, __LINE__);
				exit(EXIT_FAILURE);
			}
		}
        dim_array_device = traj_provider->dim_array_device;
        
	}
