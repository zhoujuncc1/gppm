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
	BltlChecker(Bltl* bltl){
        this->bltl=bltl;
    }
	virtual ~BltlChecker(){}
	virtual int check(Trajectory traj)=0;

	Bltl* bltl;
	map<string, Prd*> prds;
};

class RecursiveBltlChecker: public BltlChecker{
public:
	RecursiveBltlChecker(Bltl* bltl, map<string, Prd*> prds, Trajectory traj):BltlChecker(bltl){
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

private:
	map<string, int*> values;
    int length;
	void eval_prds(Trajectory traj){
		for(auto prd_itr = prds.begin(); prd_itr!=prds.end(); prd_itr++){
			Prd* prd = prd_itr->second;
			int *value_arr = values[prd_itr->first];
			for(int i = 0 ; i< length; i++){
				value_arr[i]=traj.m_states[i][prd->varId]>prd->left->value && traj.m_states[i][prd->varId]<prd->right->value;
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

class TreeBltlChecker : public BltlChecker{
public:
	TreeBltlChecker(Bltl* bltl) : BltlChecker(bltl){
		source = buildTree(bltl);
		nTime = 0;
	}
	virtual ~TreeBltlChecker() {
		for (vector<Node*>::iterator it = roots.begin(); it != roots.end();
				++it) {
			delete *it;
		}
		roots.clear();
	}

    void clean(){
        for (vector<Node*>::iterator it = roots.begin(); it != roots.end();
             ++it) {
            delete *it;
        }
        roots.clear();
        nTime=0;
    }

	virtual int check(Trajectory traj){
		clean();
		roots.push_back(source->duplicate());
		int value = update(traj.m_states[0],0);
        int i=1;
		while(value==-1&&i<traj.m_states.size()-1){
			value=update(traj.m_states[i++],0);
		}
        if(value==-1)
            value=update(traj.m_states[i],1);
        if(value==1)
            return 1;
        else
            return 0;

	}

	int update(state_type levels, int isLast) {
		roots[nTime]->update(levels, isLast);
		int tf = roots[0]->evalNode();

		if (tf == -1) {
			roots.push_back(roots[nTime]->duplicate());
			roots[nTime]->link(roots[nTime + 1]);
		}

		nTime = nTime + 1;
		return tf;
	}

	Node* buildTree(Bltl* bltl) {
		Node* node;
		switch (bltl->getOperation()) {
		case op_PRD:
			return new AtomicNode(bltl->getPrd()->varId, &(bltl->getPrd()->left->value),
					&(bltl->getPrd()->right->value));
		case op_F:
			node = new FNode(&(bltl->getTime()->value));
			node->setChild1(buildTree(bltl->getChild1()));
			return node;
		case op_G:
			node = new GNode(&(bltl->getTime()->value));
			node->setChild1(buildTree(bltl->getChild1()));
			return node;
		case op_NOT:
			node = new NotNode();
			node->setChild1(buildTree(bltl->getChild1()));
			return node;
		case op_X:
			node = new XNode();
			node->setChild1(buildTree(bltl->getChild1()));
			return node;
		case op_AND:
			node = new AndNode();
			node->setChild1(buildTree(bltl->getChild1()));
			node->setChild2(buildTree(bltl->getChild2()));
			return node;
		case op_OR:
			node = new OrNode();
			node->setChild1(buildTree(bltl->getChild1()));
			node->setChild2(buildTree(bltl->getChild2()));
			return node;
		case op_U:
			node = new UNode(&(bltl->getTime()->value));
			node->setChild1(buildTree(bltl->getChild1()));
			node->setChild2(buildTree(bltl->getChild2()));
			return node;
		}
	}

private:
	vector<Node*> roots;
	int nTime;
	Node* source;
};

#endif // BLTLCHECKER_H
