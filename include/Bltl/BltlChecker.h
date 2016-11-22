#ifndef BLTLCHECKER_H
#define BLTLCHECKER_H

#include <iostream>
#include <string>
#include <vector>
#include "Node.h"
#include "TimeVariable.h"
#include "../Model.h"
using namespace std;

class BltlChecker {
public:
	BltlChecker(Bltl* bltl) {
		roots.push_back(buildTree(bltl));
		nTime = 0;
	}
	~BltlChecker() {
		for (vector<Node*>::iterator it = roots.begin(); it != roots.end();
				++it) {
			delete *it;
		}
		roots.clear();
	}

	vector<int> check(vector<Trajectory> trajs){
		vector<int> sat;
		for(auto itr = trajs.begin(); itr!= trajs.end(); itr++) {
			int value = update(itr->m_states[0],0);
            int i=1;
			while(value==-1&&i<itr->m_states.size()-1){
				value=update(itr->m_states[i++],0);
			}
			if(value==-1)
				value=update(itr->m_states[i],1);
			sat.push_back(value);
		}
		return sat;

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
	string formulaString;
	int nTime;
};

#endif // BLTLCHECKER_H
