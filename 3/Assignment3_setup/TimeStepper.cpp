#include "TimeStepper.hpp"
#include <cstdlib>
#include <iostream>
///TODO: implement Explicit Euler time integrator here
void ForwardEuler::takeStep(ParticleSystem* particleSystem, float stepSize)
{
	
	vector<Vector3f> s = particleSystem->evalF(particleSystem->getState());
	vector<Vector3f> working = particleSystem->getState();
	vector<Vector3f> newbie;
	for (int i = 0; i < working.size(); i++) {
		newbie.push_back(Vector3f(working[i].x() + stepSize * s[i].x(), working[i].y() + stepSize * s[i].y(), working[i].z() + stepSize * s[i].z()));
	}
	particleSystem->setState(newbie); //i set the state here.
	//particleSystem->getState()[0].print();
}

///TODO: implement Trapzoidal rule here
void Trapzoidal::takeStep(ParticleSystem* particleSystem, float stepSize)
{
	vector<Vector3f> state0 = particleSystem->getState();
	vector<Vector3f> derivative1 = particleSystem->evalF(state0);
	vector<Vector3f> state1;
	for (int i = 0; i < state0.size(); i++) {
		state1.push_back(Vector3f(state0[i].x() + stepSize * derivative1[i].x(), state0[i].y() + stepSize * derivative1[i].y(), state0[i].z() + stepSize * derivative1[i].z()));
	}
	vector<Vector3f> derivative2 = particleSystem->evalF(state1);
	vector<Vector3f> state2;
	for (int i = 0; i < state1.size(); i++) {
		state2.push_back(Vector3f(state1[i].x() + stepSize * derivative2[i].x(), state1[i].y() + stepSize * derivative2[i].y(), state1[i].z() + stepSize * derivative2[i].z()));
	}
	vector<Vector3f> newstate;
	for (int i = 0; i < state2.size(); i++) {
		newstate.push_back(Vector3f(state1[i].x() / 2 + state2[i].x() / 2, state1[i].y() / 2 + state2[i].y() / 2, state1[i].z() / 2 + state2[i].z() / 2));
	}
	particleSystem->setState(newstate);
	//particleSystem->getState()[0].print();
}

void RK4::takeStep(ParticleSystem* particleSystem, float stepSize) {
	vector<Vector3f> state0 = particleSystem->getState();
	vector<Vector3f> derivative1 = particleSystem->evalF(state0); //k_1
	vector<Vector3f> state1; //now state1 is X(t = t_i)+1/2*k_1
	for (int i = 0; i < state0.size(); i++) {
		state1.push_back(Vector3f(state0[i].x() + 0.5 * stepSize * derivative1[i].x(),
			state0[i].y() + 0.5 * stepSize *  derivative1[i].y(),
			state0[i].z() + 0.5 * stepSize * derivative1[i].z()));
	}

	//cout << "derivative1" << endl;
	//derivative1[0].print();


	vector<Vector3f> derivative2 = particleSystem->evalF(state1); //k_2
	vector<Vector3f> state2; 
	for (int i = 0; i < state0.size(); i++) {
		state2.push_back(Vector3f(state0[i].x() + 0.5 * stepSize * derivative2[i].x(),
			state0[i].y() + 0.5 * stepSize * derivative2[i].y(),
			state0[i].z() + 0.5 * stepSize * derivative2[i].z()));
	}
	//cout << "derivative2" << endl;
	//derivative2[0].print();
	vector<Vector3f> derivative3 = particleSystem->evalF(state2); //k_3
	vector<Vector3f> state3; 
	for (int i = 0; i < state0.size(); i++) {
		state3.push_back(Vector3f(state0[i].x() + stepSize * derivative3[i].x(),
			state0[i].y() + stepSize * derivative3[i].y(),
			state0[i].z() + stepSize * derivative3[i].z()));
	}
	//cout << "derivative3" << endl;
	//derivative3[0].print();
	vector<Vector3f> derivative4 = particleSystem->evalF(state3);
	vector<Vector3f> finality;
	//cout << "derivative4" << endl;
	//derivative4[0].print();
	for (int i = 0; i < state0.size(); i++) {
		finality.push_back(
			Vector3f(state0[i].x() + ( derivative1[i].x() + 2 * derivative2[i].x() + 2 * derivative3[i].x() + derivative4[i].x())*stepSize/6, 
			state0[i].y() + (derivative1[i].y() + 2 * derivative2[i].y() + 2 * derivative3[i].y() + derivative4[i].y()) * stepSize/6, 
			state0[i].z() + (derivative1[i].z() + 2 * derivative2[i].z() + 2 * derivative3[i].z() + derivative4[i].z()) * stepSize/6));
	}
	particleSystem->setState(finality);
	//cout<<"Final"<<endl;
	//finality[0].print();
}








