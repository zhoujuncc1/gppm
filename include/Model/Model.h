/*
 * Model.h
 *
 *  Created on: 2 Nov 2016
 *      Author: zhoujun
 */

#ifndef SRC_BLTL_MODEL_H_
#define SRC_BLTL_MODEL_H_

class Model {
public:
	Model();
	virtual ~Model();
	double* simulate(int n, int t);
};

#endif /* SRC_BLTL_MODEL_H_ */
