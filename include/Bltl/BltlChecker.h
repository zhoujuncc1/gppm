#ifndef BLTLCHECKER_H
#define BLTLCHECKER_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "Node.h"
#include "TimeVariable.h"
#include "../Model.h"
#include "Bltl.h"
#include "Prd.h"
using namespace std;

class BltlChecker{
public:
  BltlChecker(){}
	BltlChecker(Bltl* bltl){
        this->bltl=bltl;
    }
	virtual ~BltlChecker(){}
	virtual int check(Trajectory traj)=0;
	virtual vector<int> check(){return vector<int>();}

	Bltl* bltl;
	map<string, Prd*> prds;
	int isGPU;

};

class RecursiveBltlChecker: public BltlChecker{
public:
  RecursiveBltlChecker(){}
	RecursiveBltlChecker(Bltl* bltl, map<string, Prd*> prds, Trajectory traj):BltlChecker(bltl){
		isGPU=false;
		this->prds=prds;
        length = traj.m_states.size();
		for(auto prd_itr = prds.begin(); prd_itr!=prds.end(); prd_itr++)
			values[prd_itr->first] = (int*)malloc(length*sizeof(int));
	}

	virtual ~RecursiveBltlChecker(){
		//for(auto itr = values.begin(); itr!=values.end(); itr++)
		    //delete (itr->second);
	}

	virtual int check(Trajectory traj){
		eval_prds(traj);
        int result = eval_formula(bltl, 0);
		return result;
	}
	int length;

private:
	map<string, int*> values;
	void eval_prds(Trajectory traj){
		for(auto prd_itr = prds.begin(); prd_itr!=prds.end(); prd_itr++){
			Prd* prd = prd_itr->second;
			int *value_arr = values[prd_itr->first];
			for(int i = 0 ; i< length; i++){
				value_arr[i]=traj.m_states[i][prd->varId]>=prd->left->value && traj.m_states[i][prd->varId]<=prd->right->value;
            }
		}
	}
	int eval_formula(Bltl* bltl, int t){
		switch(bltl->operation){
			case op_F:
				return eval_F(bltl, t, bltl->getTime()->value);
			case op_G:
				return eval_G(bltl, t, bltl->getTime()->value);
			case op_U:
				return eval_U(bltl, t, bltl->getTime()->value);
			case op_X:
				return eval_formula(bltl->getChild1(), t+1);
			case op_NOT:
				return !eval_formula(bltl->getChild1(), t);
			case op_AND:
				return eval_formula(bltl->getChild1(), t) && eval_formula(bltl->getChild2(), t);
			case op_OR:
				return eval_formula(bltl->getChild1(), t) || eval_formula(bltl->getChild2(), t);
			case op_PRD:
                if(t>=length){
                    return 0;
                }
                else{
				    return values[bltl->getPrdName()][t];
                }
            default:
                printf("Something is wrong\n!");
                exit(1);
        }
	}

	int eval_F(Bltl* bltl, int t, int temp){
		if (temp == 0)
        	return 0;
		else
			return eval_formula(bltl->getChild1(), t) || eval_F(bltl, t + 1, temp-1);
	}
	int eval_G(Bltl* bltl, int t, int temp){
		if (temp == 0)
        	return 1;
		else
			return eval_formula(bltl->getChild1(), t) && eval_G(bltl, t + 1, temp-1);
	}

	int eval_U(Bltl* bltl, int t, int temp){
		if(temp == 0)
        	return 1;
		else
			return eval_formula(bltl->getChild2(), t) || (eval_formula(bltl->getChild1(), t) && eval_U(bltl, t + 1, temp-1));
	}
};


class GPUBltlChecker : public RecursiveBltlChecker
{
  public:
	GPUBltlChecker(Bltl *bltl, map<string, Prd *> prds, GPUFileTrajectoryProvider* traj_provider);
	virtual ~GPUBltlChecker()
	{
		//for(auto itr = values.begin(); itr!=values.end(); itr++)
		//delete (itr->second);
	}

	virtual vector<int> check();

  private:
	float *traj_device;
	GPUFileTrajectoryProvider* traj_provider;
	int *dim_array_device;
	void init_value_array(Bltl* bltl);
	void do_init_array(Bltl* bltl);
	void eval_prds();
	void eval_prd(Prd* prd);
	bool* eval_bltl_recursive(Bltl *bltl);
	vector<int> eval_bltl(Bltl *bltl);
	bool* result_arr;
    };

#endif // BLTLCHECKER_H
