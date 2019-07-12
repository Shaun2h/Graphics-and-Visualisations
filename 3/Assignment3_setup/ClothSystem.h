#ifndef CLOTHSYSTEM_H
#define CLOTHSYSTEM_H

#include <vecmath.h>
#include <vector>
#ifdef _WIN32
#include "GL/freeglut.h"
#else
#include <GL/glut.h>
#endif

#include "particleSystem.h"

class ClothSystem: public ParticleSystem
{
///ADD MORE FUNCTION AND FIELDS HERE
public:
	ClothSystem(int numParticles,int cloth_length);
	vector<Vector3f> evalF(vector<Vector3f> state);
	vector<vector<Vector3f>> get_relevant(vector<Vector3f> input, int target);
	void draw();

private:

};


#endif
