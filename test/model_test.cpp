/*
 * prd_test.cpp
 *
 *  Created on: 1 Nov 2016
 *      Author: zhoujun
 */

#include <iostream>
#include <map>
#include <string>

#include <Model.h>
#include <gtest/gtest.h>

TEST(ModelTest, ProduceTraj){
    ASSERT_EQ(Model::N_SPECIES, 32);
	Trajectory traj = Model::simulate(1.0);
	for(int t = 0; t < Model::end_time; t++)
		printf("%lf ", traj.m_states[10][t]);
}

