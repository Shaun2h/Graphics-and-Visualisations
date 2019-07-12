#include <iostream>
#include "pendulumSystem.h"

PendulumSystem::PendulumSystem(int numParticles):ParticleSystem(numParticles)
{
	Vector3f grav = Vector3f(0, -0.9, 0); //some constant nonsense. Toggle to make pretty
	float springRestLength = 0.5;
	Vector3f basedisplace = Vector3f(1.1, 1.1, 0); //displacement between particles
	m_numParticles = numParticles;
	vector<Vector3f> state;
	// fill in code for initializing the state based on the number of particles
	for (int i = 0; i < m_numParticles; i++) {
		state.push_back(Vector3f(0, 0, 0)+basedisplace*i); //push back pos
		state.push_back(Vector3f(0, 0, 0)+basedisplace*i); //push back velo
		// for this system, we care about the position and the velocity

	}
	this->setState(state);
}


// TODO: implement evalF
// for a given state, evaluate f(X,t)
/*Vector3f PendulumSystem::computeSpring(float restLenght, float springConstant, Vector3f position1, Vector3f position2) {
	// spring force depends on the number of particles
	Vector3f f;
	if (this->m_numParticles == 0) {
		f = Vector3f(0, 0, 0);
	}
	else {
		Vector3f distanceVec = Vector3f(position1.x() - position2.x(), position1.y() - position2.y(), position1.z() - position2.z());
		float distance = (distanceVec).abs();
		f = -1.0 * springConstant * (distance - restLenght) * (distanceVec.normalized());
	}
	return f;
}*/
vector<Vector3f> PendulumSystem::evalF(vector<Vector3f> state)
{
	Vector3f grav = Vector3f(0, -0.9, 0); //some constant nonsense. Toggle to make pretty
	float springRestLength = 0.5;
	Vector3f basedisplace = Vector3f(1.1, 1.1, 0); //displacement between particles
	vector<Vector3f> f;
	vector<Vector3f> lastVelocity = this->getVel(state);
	vector<Vector3f> position = this->getPos(state);
	float mass = 0.5;//some mass constant
	// the first two entries (initial velocity and accelerations) in f are zero vectors
	f.push_back(Vector3f(0, 0, 0));
	f.push_back(Vector3f(0, 0, 0));

	float springconstant = 50; //normally quite large.. 100 is alright maybe.
	float dragQueenConstant = 1.28; //drag affects all. this is the typical drag value for something like a plate perpendicular to flow in 3d.
	// YOUR CODE HERE
	for (int i = 1; i < m_numParticles; i++) { //start from 1. end before.
		// velocity
		if (i < m_numParticles - 1) { //has before after
			Vector3f grav = Vector3f(0, -9.81, 0) * mass;
			Vector3f drag = dragQueenConstant * lastVelocity[i]; //affected by last velo.
			Vector3f anchor = position[i];
			Vector3f relative = position[i - 1];
			Vector3f direction = Vector3f(anchor.x() - relative.x(), anchor.y() - relative.y(), anchor.z() - relative.z()).normalized();
			float distance = Vector3f(anchor.x() - relative.x(), anchor.y() - relative.y(), anchor.z() - relative.z()).abs();
			// get distance.
			Vector3f a = springconstant * (springRestLength - distance) * direction;
			anchor = position[i];
			relative = position[i + 1];
			direction = Vector3f(anchor.x() - relative.x(), anchor.y() - relative.y(), anchor.z() - relative.z()).normalized();
			distance = Vector3f(anchor.x() - relative.x(), anchor.y() - relative.y(), anchor.z() - relative.z()).abs();
			a = a + springconstant * (springRestLength - distance) * direction;
			a = a + grav - drag;
			a = a / mass;
			//a = accel though.
			f.push_back(position[i] + lastVelocity[i]); //this gets you your new position
			f.push_back(lastVelocity[i] + a); //this gets you your new velocity;

		}
		else {
			Vector3f grav = Vector3f(0, -9.81, 0) * mass;
			Vector3f drag = dragQueenConstant * lastVelocity[i]; //after only.
			Vector3f anchor = position[i];
			Vector3f relative = position[i - 1];
			Vector3f direction = Vector3f(anchor.x() - relative.x(), anchor.y() - relative.y(), anchor.z() - relative.z()).normalized();
			float distance = Vector3f(anchor.x() - relative.x(), anchor.y() - relative.y(), anchor.z() - relative.z()).abs();
			// get distance.
			Vector3f a = springconstant * (springRestLength - distance) * direction;
			a = a + grav - drag;
			a = a / mass;
			f.push_back(position[i] + lastVelocity[i]); //this gets you your new position
			f.push_back(lastVelocity[i] + a); //this gets you your new velocity;
		}
	}
	/*
	vector<Vector3f> f;
	vector<Vector3f> lastVelocity = this->getVelocity();
	float mass = 0.5;//some mass constant
	// the first two entries (initial velocity and accelerations) in f are zero vectors
	f.push_back(Vector3f(0, 0, 0));

	float springconstant = 1;
	float dragQueenConstant = 0.01; //drag affects all.
	// YOUR CODE HERE
	for (int i = 1; i < m_numParticles; i++) { //start from 1. end before.
		// velocity
		if (i<m_numParticles-1){ //has before after
			Vector3f grav = Vector3f(0, -9.81, 0) * mass;
			Vector3f drag = dragQueenConstant * lastVelocity[i]; //affected by last velo.
			Vector3f anchor = position[i];
			Vector3f relative = position[i - 1];
			Vector3f direction = Vector3f(anchor.x() - relative.x(), anchor.y() - relative.y(), anchor.z() - relative.z()).normalized();
			float distance = Vector3f(anchor.x() - relative.x(), anchor.y() - relative.y(), anchor.z() - relative.z()).abs();
			// get distance.
			Vector3f a = springconstant * (springRestLength - distance) * direction;
			anchor = position[i]; 
			relative = position[i+1];
			direction = Vector3f(anchor.x() - relative.x(), anchor.y() - relative.y(), anchor.z() - relative.z()).normalized();
			distance = Vector3f(anchor.x() - relative.x(), anchor.y() - relative.y(), anchor.z() - relative.z()).abs();
			a =a+ springconstant * (springRestLength - distance) * direction;
			a = a +grav-drag;
			a = a / mass;
			f.push_back(a); //the distance negative means outward push. correct.
		}
		else {
			Vector3f grav = Vector3f(0, -9.81, 0) * mass;
			Vector3f drag = dragQueenConstant * lastVelocity[i]; //after only.
			Vector3f anchor = position[i];
			Vector3f relative = position[i - 1];
			Vector3f direction = Vector3f(anchor.x() - relative.x(), anchor.y() - relative.y(), anchor.z() - relative.z()).normalized();
			float distance = Vector3f(anchor.x() - relative.x(), anchor.y() - relative.y(), anchor.z() - relative.z()).abs();
			// get distance.
			Vector3f a = springconstant * (springRestLength - distance) * direction;
			a = a + grav - drag;
			a = a / mass;
			f.push_back(a); //the distance negative means outward push. correct.
		}
	}
	*/
	
	//this->setVelocity(f);
	//vector<Vector3f> saved;
	/*for (int i = 0; i < f.size(); i++) {
		saved.push_back(lastVelocity[i]+f[i]);
	}
	this->setVelocity(saved);*/
	/*
	if (state.size() > 3) {
		f.push_back(Vector3f(0.0f, 0.0f, 0.0f)); //the first is not supposed to move.
		for (int i = 1; i < state.size()-1; i++) {
			Vector3f displace1 = state[i] - state[i - 1]; // this gives us the displacement relative to particle before.
			Vector3f displace2 = state[i] - state[i + 1]; //this gives us displacement relative to particle after.
			//we now take their normal values and multiply by their direction and the spring constant
			float abs1 = sqrt(pow(displace1.x(),2) + pow(displace1.y(),2) + pow(displace1.z(),2));
			float abs2 = sqrt(pow(displace2.x(), 2) + pow(displace2.y(), 2) + pow(displace2.z(), 2));
			f.push_back(Vector3f(-springconstant * displace1.normalized()*abs1 +springconstant * displace2.normalized()*abs2));
		}
		Vector3f displace1 = state[state.size()-1] - state[state.size()-2]; // this gives displacement relative to particle before, for final particle
		float abs1 = sqrt(pow(displace1.x(), 2) + pow(displace1.y(), 2) + pow(displace1.z(), 2));
		f.push_back(Vector3f(-springconstant * displace1.normalized()*abs1));
		for (int i = 1; i < f.size(); i++) {
			f[i] = f[i] + grav;
			f[i] = f[i] *  dragQueenConstant;
		}
	}
	else {
		f.push_back(Vector3f(0.0f, 0.0f, 0.0f)); //the first is not supposed to move.
		for (int i = 1; i < state.size(); i++) {
			Vector3f displace1 = state[i] - state[i - 1]; // this gives us the displacement relative to particle before.
			//we now take their normal values and multiply by their direction and the spring constant
			float abs1 = sqrt(pow(displace1.x(), 2) + pow(displace1.y(), 2) + pow(displace1.z(), 2));
			f.push_back(Vector3f(-springconstant * displace1.normalized() * abs1));
		}
		for (int i = 1; i < f.size(); i++) {
			f[i] = f[i] + grav;
			f[i] = f[i] * dragQueenConstant;
		}
		for (int i = 0; i < state.size(); i++) {
			if (f[i].x() + state[i].x() > 2) {
				f[i].x() = 0;
			}
			if (f[i].y() + state[i].y() > 2) {
				f[i].y() = 0;
			}
			if (f[i].z() + state[i].z() > 2) {
				f[i].z() = 0;
			}

		}
	}*/
	// YOUR CODE HERE

	return f;
}

// render the system (ie draw the particles)
void PendulumSystem::draw()
{
	cout << "stepped!" << endl;
	for (int i = 0; i < this->getState().size(); i=i+2) {
		
		this->getState()[i].print();
		Vector3f pos = this->getState()[i];//  position of particle i. YOUR CODE HERE
		glPushMatrix();
		glTranslatef(pos[0], pos[1], pos[2] );
		glutSolidSphere(0.075f,10.0f,10.0f);
		glPopMatrix();
	}
}
