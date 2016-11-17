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
#include<boost/numeric/odeint/integrate/integrate.hpp>
#include<boost/random.hpp>

using namespace boost::numeric::odeint;

typedef std::vector<double> state_type;

typedef boost::uniform_real<> NumberDistribution;
typedef boost::mt19937 RandomNumberGenerator;
typedef boost::variate_generator<RandomNumberGenerator&, NumberDistribution> Generator;


class Trajectory{
public:
	Trajectory(int N_SPECIES, double dt){
		this->N_SPECIES=N_SPECIES;
		this->dt=dt;
	}
	double dt;
	int N_SPECIES;
	std::vector<state_type > m_states;
	std::vector<double> m_times;
};


class Model {
public:
	static Trajectory simulate(double dt) {
		Trajectory traj = Trajectory(Model::N_SPECIES, dt);
		NumberDistribution distribution(1.0 - Model::x_variation,
				1.0 + Model::x_variation);
		RandomNumberGenerator generator;
		Generator numberGenerator(generator, distribution);
		generator.seed(std::time(0)); // seed with the current time

		state_type x0;
		for (int i = 0; i < Model::N_SPECIES; i++) {
			x0.push_back(Model::x[i] * numberGenerator());
		}
		for (double i = 0; i < Model::end_time; i += dt) {
            traj.m_times.push_back(i);
            traj.m_states.push_back(x0);
            integrate(Model::odefun, x0, 0.0, dt, 0.001);
		}
		traj.m_times.push_back(Model::end_time);
		traj.m_states.push_back(x0);
        return traj;
	}

	static void odefun ( const state_type &x , state_type &dxdt , const double /* t */ );

	static const int N_SPECIES;
	static const int N_PARAMS;
	static const double x[];
	static const double p[];
	static const double end_time;

private:

	static const double x_variation;

};


#endif /* SRC_BLTL_MODEL_H_ */
