#ifndef PENDULUMSYSTEM_H
#define PENDULUMSYSTEM_H

#include <vecmath.h>
#include <vector>
#ifdef _WIN32
#include "GL/freeglut.h"
#else
#include <GL/glut.h>
#endif

#include "particleSystem.h"

class PendulumSystem: public ParticleSystem
{
public:
	PendulumSystem(int numParticles);
	
	
	vector<Vector3f> evalF(vector<Vector3f> state);
	vector<Vector3f> getPos(vector<Vector3f> state) { vector<Vector3f> a; for (int i = 0; i < state.size(); i = i + 2) { a.push_back(state[i]); }return a; }; //return even indexes
	vector<Vector3f> getVel(vector<Vector3f> state) { vector<Vector3f> a; for (int i = 1; i < state.size(); i = i + 2) { a.push_back(state[i]); }return a; }; // return odd indexes
	//pos vel pos vel
	void draw();
	/*
	vector<Vector3f> getVelocity() { return m_velocity;};
	void setVelocity(vector<Vector3f> state) {m_velocity = state; };
	void setState(vector<Vector3f>state) {
		if (m_vVecState.size() != 0) {
			vector<Vector3f> saver;
			for (int i = 0; i < state.size(); i++) {
				saver.push_back(state[i] - m_vVecState[i]);
			}
			m_velocity = saver;
			m_vVecState = state;
		}
		else {
			m_vVecState = state;
		}
		};
	
protected:

	vector<Vector3f> m_velocity;*/

};

#endif
