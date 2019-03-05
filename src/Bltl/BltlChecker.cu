#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cuda_runtime.h>
#include "../../include/helper_functions.h"
#include "../../include/helper_cuda.h"

#include "../../include/Bltl/BltlChecker.h"
#include "../../include/Bltl/Bltl.h"

__global__ void fill_bool_array(bool* to_fill, bool value, int n_threads) {
	int n = blockIdx.x * n_threads + threadIdx.x;
	to_fill[n] = value;
}

__global__ void eval_prd_kernel(float *traj, bool *value_default_true,
		bool *value_default_false, int varId, float left, float right,
		int *dim_array_device) {
	int n = blockIdx.x;
	int t = threadIdx.x;
	int index = n * 2 * dim_array_device[TRAJ_N_SPECIES_I]
			* dim_array_device[TRAJ_END_TIME_I]
			+ t * 2 * dim_array_device[TRAJ_N_SPECIES_I] + varId;
	value_default_true[n * 2 * dim_array_device[TRAJ_END_TIME_I] + t] =
			traj[index] <= right && traj[index] >= left;
	value_default_false[n * 2 * dim_array_device[TRAJ_END_TIME_I] + t] =
			value_default_true[n * 2 * dim_array_device[TRAJ_END_TIME_I] + t];
}

__global__ void eval_G_kernel(bool* value_arr_default_true,
		bool* value_arr_default_false, bool* sub_value_arr, int distance,
		int *dim_array_device) {
	int n = blockIdx.x;
	int t = threadIdx.x;
	int index = n * 2 * dim_array_device[TRAJ_END_TIME_I] + t;
	value_arr_default_true[index] = true;
	for (int k = 0; k < distance; k++) {
		value_arr_default_true[index] = value_arr_default_true[index]
				&& sub_value_arr[index + k];
	}
	value_arr_default_false[index] = value_arr_default_true[index];
}

__global__ void eval_F_kernel(bool* value_arr_default_true,
		bool* value_arr_default_false, bool* sub_value_arr, int distance,
		int *dim_array_device) {
	int n = blockIdx.x;
	int t = threadIdx.x;
	int index = n * 2 * dim_array_device[TRAJ_END_TIME_I] + t;
	value_arr_default_false[index] = false;
	for (int k = 0; k < distance; k++) {
		value_arr_default_false[index] = value_arr_default_false[index]
				|| sub_value_arr[index + k];
	}
	value_arr_default_true[index] = value_arr_default_false[index];
}

__global__ void eval_AND_kernel(bool* value_arr_default_true,
		bool* value_arr_default_false, bool* sub_value_arr1,
		bool* sub_value_arr2, int *dim_array_device) {
	int n = blockIdx.x;
	int t = threadIdx.x;
	int index = n * 2 * dim_array_device[TRAJ_END_TIME_I] + t;
	value_arr_default_true[index] = sub_value_arr1[index]
			&& sub_value_arr2[index];
	value_arr_default_false[index] = value_arr_default_true[index];

}

__global__ void eval_OR_kernel(bool* value_arr_default_true,
		bool* value_arr_default_false, bool* sub_value_arr1,
		bool* sub_value_arr2, int *dim_array_device) {
	int n = blockIdx.x;
	int t = threadIdx.x;
	int index = n * 2 * dim_array_device[TRAJ_END_TIME_I] + t;
	value_arr_default_true[index] = sub_value_arr1[index]
			|| sub_value_arr2[index];
	value_arr_default_false[index] = value_arr_default_true[index];

}
void GPUBltlChecker::eval_prds() {
	dim3 block_number_prd(traj_provider->dim_array[TRAJ_SIZE_I]);
	dim3 threadsPerBlock_prd(length);
	for (auto prd_itr = prds.begin(); prd_itr != prds.end(); prd_itr++) {
		Prd *prd = prd_itr->second;
		eval_prd_kernel<<<block_number_prd, threadsPerBlock_prd>>>(
				traj_provider->trajectories_device,
				prd->value_array_device_default_true,
				prd->value_array_device_default_false, prd->varId,
				prd->left->value, prd->right->value, dim_array_device);
		cudaDeviceSynchronize();

	}
}

void GPUBltlChecker::eval_prd(Prd* prd) {
	dim3 block_number_prd(traj_provider->dim_array[TRAJ_SIZE_I]);
	dim3 threadsPerBlock_prd(length);
	eval_prd_kernel<<<block_number_prd, threadsPerBlock_prd>>>(
			traj_provider->trajectories_device,
			prd->value_array_device_default_true,
			prd->value_array_device_default_false, prd->varId, prd->left->value,
			prd->right->value, dim_array_device);
	cudaDeviceSynchronize();
}

bool* GPUBltlChecker::eval_bltl_recursive(Bltl *bltl) {
	if (bltl->getOperation() == op_PRD) {
		eval_prd(bltl->getPrd());
	}
	else {
		if (bltl->getChild1())
			eval_bltl_recursive(bltl->getChild1());
		if (bltl->getChild2())
			eval_bltl_recursive(bltl->getChild2());

		dim3 block_number_prd(traj_provider->dim_array[TRAJ_SIZE_I]);
		dim3 threadsPerBlock_prd(length);

		if (bltl->getOperation() == op_G)
			eval_G_kernel<<<block_number_prd, threadsPerBlock_prd>>>(
					bltl->value_array_device_default_true,
					bltl->value_array_device_default_false,
					bltl->getChild1()->value_array_device_default_true,
					bltl->getTime()->value, dim_array_device);
		else if (bltl->getOperation() == op_F)
			eval_F_kernel<<<block_number_prd, threadsPerBlock_prd>>>(
					bltl->value_array_device_default_true,
					bltl->value_array_device_default_false,
					bltl->getChild1()->value_array_device_default_false,
					bltl->getTime()->value, dim_array_device);
		else if (bltl->getOperation() == op_AND)
			eval_AND_kernel<<<block_number_prd, threadsPerBlock_prd>>>(
					bltl->value_array_device_default_true,
					bltl->value_array_device_default_false,
					bltl->getChild1()->value_array_device_default_true,
					bltl->getChild2()->value_array_device_default_true,
					dim_array_device);
		else if (bltl->getOperation() == op_OR)
			eval_OR_kernel<<<block_number_prd, threadsPerBlock_prd>>>(
					bltl->value_array_device_default_true,
					bltl->value_array_device_default_false,
					bltl->getChild1()->value_array_device_default_true,
					bltl->getChild2()->value_array_device_default_true,
					dim_array_device);

		cudaDeviceSynchronize();
	}
}

vector<int> GPUBltlChecker::eval_bltl(Bltl *bltl) {
	eval_bltl_recursive(bltl);

	cudaError_t error = cudaMemcpy(result_arr,
			bltl->value_array_device_default_true,
			2 * traj_provider->dim_array[TRAJ_SIZE_I] * length * sizeof(bool),
			cudaMemcpyDeviceToHost);

	if (error != cudaSuccess) {
		printf(
				"cudaMemcpy (result_arr, bltl->value_array_device) returned error %s (code %d), line(%d)\n",
				cudaGetErrorString(error), error, __LINE__);
		exit(EXIT_FAILURE);
	}

	vector<int> results;
	for (int i = 0; i < traj_provider->dim_array[TRAJ_SIZE_I]; i++)
		results.push_back(result_arr[i * 2 * length]);
	return results;
}

void GPUBltlChecker::do_init_array(Bltl* bltl) {
	cudaMalloc((void **) &(bltl->value_array_device_default_true),
			2 * traj_provider->dim_array[TRAJ_SIZE_I] * length * sizeof(bool));

	cudaMemset(bltl->value_array_device_default_true, true,
			2 * traj_provider->dim_array[TRAJ_SIZE_I] * length * sizeof(bool));

	cudaMalloc((void **) &(bltl->value_array_device_default_false),
			2 * traj_provider->dim_array[TRAJ_SIZE_I] * length * sizeof(bool));

	cudaMemset(bltl->value_array_device_default_false, false,
			2 * traj_provider->dim_array[TRAJ_SIZE_I] * length * sizeof(bool));
}

void GPUBltlChecker::init_value_array(Bltl* bltl) {
	do_init_array(bltl);
	if (bltl->getOperation() == op_PRD) {
		bltl->getPrd()->value_array_device_default_true =
				bltl->value_array_device_default_true;
		bltl->getPrd()->value_array_device_default_false =
				bltl->value_array_device_default_false;
	} else {
		if (bltl->getChild1())
			init_value_array(bltl->getChild1());
		if (bltl->getChild2())
			init_value_array(bltl->getChild2());
	}
}

vector<int> GPUBltlChecker::check() {
	//eval_prds();
	vector<int> result = eval_bltl(bltl);
	return result;
}

GPUBltlChecker::GPUBltlChecker(Bltl *bltl, map<string, Prd *> prds,
		GPUFileTrajectoryProvider* traj_provider) {
	isGPU = true;
	this->bltl = bltl;
	this->prds = prds;
	this->traj_provider = traj_provider;
	this->length = traj_provider->dim_array[TRAJ_END_TIME_I];
	dim_array_device = traj_provider->dim_array_device;
	init_value_array(this->bltl);
	result_arr = (bool*) malloc(
			2 * traj_provider->dim_array[TRAJ_SIZE_I] * length * sizeof(bool));
}
