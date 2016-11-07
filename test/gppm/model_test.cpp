/*
 * prd_test.cpp
 *
 *  Created on: 1 Nov 2016
 *      Author: zhoujun
 */

#include <iostream>
#include <map>
#include <string>

#include "../../include/Model.h"
#include "../gtest/include/gtest/gtest.h"

TEST(ModelTest, ProduceTraj){
	std::vector<state_type> m_states;
	std::vector<double> m_times;
	Model::simulate(m_states, m_times, 1.0);
	for(int t = 0; t < Model::end_time; t++)
		printf("%lf ", m_states[t][10]);
}

