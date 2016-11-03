/*
 * Model.h
 *
 *  Created on: 2 Nov 2016
 *      Author: zhoujun
 */

#ifndef SRC_BLTL_MODEL_H_
#define SRC_BLTL_MODEL_H_
#include<vector>
#include<cstdlib>
#include<ctime>
#include<boost/numeric/odeint.hpp>
#include<boost/random.hpp>
typedef std::vector<double> state_type;

typedef boost::uniform_int<> NumberDistribution;
typedef boost::mt19937 RandomNumberGenerator;
typedef boost::variate_generator<RandomNumberGenerator&, NumberDistribution> Generator;

class Model {
public:
	Model() {
		x_variation = 0.05;
	}
	void simulate(std::vector<state_type>& m_states,
			std::vector<double>& m_times, double dt) {

		NumberDistribution distribution(1.0 - x_variation, 1.0 + x_variation);
		RandomNumberGenerator generator;
		Generator numberGenerator(generator, distribution);
		generator.seed(std::time(0)); // seed with the current time

		state_type x0;
		for (int i = 0; i < N_SPECIES; i++) {
			x0[i] = x_init[i] *= numberGenerator();
		}
		m_times.push_back(0.0);
		m_state.push_back(x0);
		std::vector<state_type> output;
		for (double i = dt; i <= end_time; i += dt) {
			integrate(odefun, x0, 0.0, dt, 0.1);
			output=x0;
			m_times.push_back(i);
			m_state.push_back(output);
		}
	}

protected:
	const int N_SPECIES;
	const int N_PARAMS;

	const double end_time;
	const double x_variation;

	double* x_init;

	virtual void odefun(const state_type &x, state_type &dxdt, const double t);
};

class Egfngf: Model {
public:
	Egfngf() :
			Model() {
		N_SPECIES = 32;
		N_PARAMS = 48;
		end_time = 62.0;
		x_init = {10002000, 456000, 80000, 0, 10000, 0, 120000, 0, 120000, 0,
			120000, 0, 120000, 120000, 0, 120000, 0, 600000, 0, 600000, 0,
			120000, 0, 120000, 0, 120000, 0, 120000, 0, 120000, 120000, 120000};
		p = {2.185e-005, 0.012101, 1.3821e-007, 0.0072381, 694.731, 6086070,
			389.428, 2112.66, 1611.97, 896896, 32.344, 35954.3, 1509.36,
			1432410, 0.8841, 62464.6, 185.759, 4768350, 125.089, 157948, 2.8324,
			518753, 9.8537, 1007340, 8.8912, 3496490, 0.02137, 763523, 10.6737,
			184912, 0.077107, 272056, 0.056628, 653951, 15.1212, 119355,
			146.912, 12876.2, 1.4015, 10965.6, 27.265, 295990, 2.21, 1025460,
			0.12633, 1061.71, 441.287, 10879500};
	}
protected:
	double *p;

	void odefun(const state_type &x, state_type &dxdt, const double t) {
		dxdt[0] = -(p[0] * x[0] * x[2]) + (p[1] * x[3]);
		dxdt[1] = -(p[2] * x[1] * x[4]) + (p[3] * x[5]);
		dxdt[2] = -(p[0] * x[0] * x[2]) + (p[1] * x[3]);
		dxdt[3] = +(p[0] * x[0] * x[2]) - (p[1] * x[3]);
		dxdt[4] = -(p[2] * x[1] * x[4]) + (p[3] * x[5]);
		dxdt[5] = +(p[2] * x[1] * x[4]) - (p[3] * x[5]);
		dxdt[6] = -((p[4] * x[3] * x[6] / (x[6] + p[5])))
		- ((p[6] * x[5] * x[6] / (x[6] + p[7])))
		+ ((p[8] * x[9] * x[7] / (x[7] + p[9])));
		dxdt[7] = +((p[4] * x[3] * x[6] / (x[6] + p[5])))
		+ ((p[6] * x[5] * x[6] / (x[6] + p[7])))
		- ((p[8] * x[9] * x[7] / (x[7] + p[9])));
		dxdt[8] = -((p[26] * x[20] * x[8] / (x[8] + p[27])));
		dxdt[9] = +((p[26] * x[20] * x[8] / (x[8] + p[27])));
		dxdt[10] = -((p[10] * x[7] * x[10] / (x[10] + p[11])))
		+ ((p[12] * x[12] * x[11] / (x[11] + p[13])));
		dxdt[11] = +((p[10] * x[7] * x[10] / (x[10] + p[11])))
		- ((p[12] * x[12] * x[11] / (x[11] + p[13])));
		dxdt[12] = 0;
		dxdt[13] = -((p[14] * x[11] * x[13] / (x[13] + p[15])))
		+ ((p[44] * x[31] * x[14] / (x[14] + p[45])))
		+ ((p[34] * x[24] * x[14] / (x[14] + p[35])));
		dxdt[14] = +((p[14] * x[11] * x[13] / (x[13] + p[15])))
		- ((p[44] * x[31] * x[14] / (x[14] + p[45])))
		- ((p[34] * x[24] * x[14] / (x[14] + p[35])));
		dxdt[15] = -((p[42] * x[28] * x[15] / (x[15] + p[43])))
		+ ((p[46] * x[31] * x[16] / (x[16] + p[47])));
		dxdt[16] = +((p[42] * x[28] * x[15] / (x[15] + p[43])))
		- ((p[46] * x[31] * x[16] / (x[16] + p[47])));
		dxdt[17] = -((p[16] * x[14] * x[17] / (x[17] + p[17])))
		- ((p[18] * x[16] * x[17] / (x[17] + p[19])))
		+ ((p[20] * x[30] * x[18] / (x[18] + p[21])));
		dxdt[18] = +((p[16] * x[14] * x[17] / (x[17] + p[17])))
		+ ((p[18] * x[16] * x[17] / (x[17] + p[19])))
		- ((p[20] * x[30] * x[18] / (x[18] + p[21])));
		dxdt[19] = -((p[22] * x[18] * x[19] / (x[19] + p[23])))
		+ ((p[24] * x[30] * x[20] / (x[20] + p[25])));
		dxdt[20] = +((p[22] * x[18] * x[19] / (x[19] + p[23])))
		- ((p[24] * x[30] * x[20] / (x[20] + p[25])));
		dxdt[21] = -((p[28] * x[3] * x[21] / (x[21] + p[29])))
		- ((p[30] * x[11] * x[21] / (x[21] + p[31])));
		dxdt[22] = +((p[28] * x[3] * x[21] / (x[21] + p[29])))
		+ ((p[30] * x[11] * x[21] / (x[21] + p[31])));
		dxdt[23] = -((p[32] * x[22] * x[23] / (x[23] + p[33])));
		dxdt[24] = +((p[32] * x[22] * x[23] / (x[23] + p[33])));
		dxdt[25] = -((p[36] * x[5] * x[25] / (x[25] + p[37])));
		dxdt[26] = +((p[36] * x[5] * x[25] / (x[25] + p[37])));
		dxdt[27] = -((p[38] * x[26] * x[27] / (x[27] + p[39])))
		+ ((p[40] * x[29] * x[28] / (x[28] + p[41])));
		dxdt[28] = +((p[38] * x[26] * x[27] / (x[27] + p[39])))
		- ((p[40] * x[29] * x[28] / (x[28] + p[41])));
		dxdt[29] = 0;
		dxdt[30] = 0;
		dxdt[31] = 0;

	}
};
#endif /* SRC_BLTL_MODEL_H_ */
