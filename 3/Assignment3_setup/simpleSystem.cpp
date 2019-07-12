
#include "simpleSystem.h"
#include <iostream>
using namespace std;

SimpleSystem::SimpleSystem()
{
	vector<Vector3f> state;
	state.push_back(Vector3f(1, 1, 1));
	this->setState(state);

}

// TODO: implement evalF
// for a given state, evaluate f(X,t)
vector<Vector3f> SimpleSystem::evalF(vector<Vector3f> state)
{
	vector<Vector3f> f;
	for (int i = 0; i < state.size(); i++) {
		f.push_back(Vector3f(-state[i].y(), state[i].x(), 0));
	}
	// YOUR CODE HERE

	return f;
}

// render the system (ie draw the particles)
void SimpleSystem::draw()
{
	for (int i=0;i<this->getState().size();i++){
		Vector3f pos = this->getState()[i];//YOUR PARTICLE POSITION
	    glPushMatrix();
		glTranslatef(pos[0], pos[1], pos[2] );
		glutSolidSphere(0.075f,10.0f,10.0f);
		glPopMatrix();
	}
}
