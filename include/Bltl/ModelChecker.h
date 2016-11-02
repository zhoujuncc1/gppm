#ifndef MODELCHECKER_H
#define MODELCHECKER_H

#include <iostream>
#include <string>
#include <vector>
#include "Node.h"
#include "TimeVariable.h"
using namespace std;

class ModelChecker {
public:
	ModelChecker(Bltl* bltl) {
		roots.push_back(buildTree(bltl));
		nTime = 0;
	}
	~ModelChecker() {
		for (vector<Node*>::iterator it = roots.begin(); it != roots.end();
				++it) {
			delete *it;
		}
		roots.clear();
	}

	int update(int *levels, int isLast) {
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
		switch (bltl->operation) {
		case op_PRD:
			Prd* prd = bltl->getPrd();
			return new AtomicNode(prd->varId, &(prd->left->value),
					&(prd->right->value));
		case op_F:
			Node* node = new FNode(&(bltl->getTime()->value));
			node->setChild1(buildTree(bltl->getChild1()));
			return node;
		case op_G:
			Node* node = new GNode(&(bltl->getTime()->value));
			node->setChild1(buildTree(bltl->getChild1()));
			return node;
		case op_NOT:
			Node* node = new NotNode();
			node->setChild1(buildTree(bltl->getChild1()));
			return node;
		case op_X:
			Node* node = new XNode();
			node->setChild1(buildTree(bltl->getChild1()));
			return node;
		case op_AND:
			Node* node = new AndNode();
			node->setChild1(buildTree(bltl->getChild1()));
			node->setChild2(buildTree(bltl->getChild2()));
			return node;
		case op_OR:
			Node* node = new OrNode();
			node->setChild1(buildTree(bltl->getChild1()));
			node->setChild2(buildTree(bltl->getChild2()));
			return node;
		case op_U:
			Node* node = new UNode(&(bltl->getTime()->value));
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

#endif
