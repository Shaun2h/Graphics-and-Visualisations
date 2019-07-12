#include "ClothSystem.h"
#include "iostream"
//TODO: Initialize here
int cloth_length = 0;
ClothSystem::ClothSystem(int numParticles,int input_length) :ParticleSystem(numParticles)
{
	Vector3f basedisplace = Vector3f(-0.5, 0, 0);
	cloth_length = input_length;
	if (cloth_length > numParticles / 2 || pow(cloth_length,2)!=numParticles) { //lets... limit it to a square cloth for now.
		cout << "It is nonsense." << endl;
		cout << "Cloth Length: " << cloth_length << endl;
		cout << "Num Particles: " << numParticles << endl;
		cout << "current clothlength**2: " <<pow(cloth_length,2)<< endl;
		cout << "Essentially, we require at least cloth_length**2 == numParticles. else it's a chain." << endl;
		exit(0);
	}
	m_numParticles = numParticles;
	vector<Vector3f> state;
	// fill in code for initializing the state based on the number of particles
	int internalcount = 0;
	float row = 0; //an internal counter for the row number
	float row2 = 0;
	for (int i = 0; i < m_numParticles; i++) {
		state.push_back(Vector3f(3, row, row2) + basedisplace * internalcount); //push back pos
		state.push_back(Vector3f(0, 0, 0)); //push back velo. here we don't want to add any speed however..
		internalcount += 1;
		if (internalcount% cloth_length == 0){
			internalcount = 0;
			row -= 0.5;
			row2 += 0.5;
		}
		// for this system, we care about the position and the velocity

	}
	this->setState(state);
}


vector<vector<Vector3f>>ClothSystem:: get_relevant(vector<Vector3f> state, int actual_index)
{
	// We use zero indexing here. hoser.
	bool isright = false;
	bool isleft = false;
	bool isbot = false;
	bool istop = false;
	bool hasleftflex = true;
	bool hasrightflex = true;
	bool hastopflex = true;
	bool hasbotflex = true;
	vector<Vector3f> structural; 
	vector<Vector3f> flexion;
	vector<Vector3f> shear;
		
	int particle_num = actual_index; //this will return the actual particle number in the cloth
	if (particle_num%cloth_length==0) { //it's on the left side of the cloth since it's equal to cloth_length. 
		isleft = true;
	}
	//else if (particle_num % cloth_length != cloth_length -1) { // it's in the middle of the cloth. 
		//the particle is not on the left, and not on the right.
	//}
	else if (particle_num % cloth_length == cloth_length - 1) { // it's on the right side of the cloth
		//particle is on the right side of the cloth, taking up cloth len.
		isright = true; 
	}
	
	if (actual_index % (cloth_length)> 2) {//has a left flexion
		flexion.push_back(state[actual_index - 2]);
	}
	if (actual_index % (cloth_length) < cloth_length - 2 && actual_index<state.size()) { //has a right flexion.
		flexion.push_back(state[actual_index + 2]);
	}
	if ((actual_index - (cloth_length * 2) > 0)) { //has a top flexion
		flexion.push_back(state[actual_index - (cloth_length*2)]);
	}
	if ((actual_index + (cloth_length * 2) < state.size())) { //has a bot flexion
		flexion.push_back(state[actual_index + (cloth_length*2)]);
	}
	
	
	int quotient = particle_num / cloth_length; // check if it's 0 here.
	if (quotient == 0) {// it's on the top of the cloth.
		istop = true;
	}
	else if (quotient == cloth_length - 1) {// it's at the bottom of the cloth.
		isbot = true;
	}
	/*else {
		// it's in the middle of the cloth.
		isupdown_mid = true;
	}*/

	if (istop) {
		if (isright) {
			// top right particle...
			
			//structural springs
			structural.push_back(state[actual_index - 1]);//push back the position of that on the left.
			//structural.push_back(state[actual_index - 1]); //push back the velocity of that on the left.
			structural.push_back(state[actual_index + cloth_length]);//push back the position of that on the directly below.
			//structural.push_back(state[actual_index + cloth_length*2 +1 ]); //push back the velocity of that directly below.
			
			//shear springs
			shear.push_back(state[actual_index + cloth_length- 1]);//push back position of the bottom diagonal left one
			
			//flexion
			//cout << "top right" << endl;
			// we pin this..
		}
		else if (isleft) {
			//top left particle..

			//structural springs
			structural.push_back(state[actual_index + 1]);//push back the position of that on the right.
			//structural.push_back(state[actual_index + 3]); //push back the velocity of that on the right.
			structural.push_back(state[actual_index + cloth_length]);//push back the position of that on the directly below.
			//structural.push_back(state[actual_index + cloth_length * 2 + 1]); //push back the velocity of that directly below.


			//shear springs
			shear.push_back(state[actual_index + cloth_length + 1]);//push back position of the bottom diagonal right one


			//we don't do anything for this particle...
			//cout << "top left" << endl;



		}
		else {
			// only along the top border

			//structural springs
			structural.push_back(state[actual_index - 1]);//push back the position of that on the left.
			//structural.push_back(state[actual_index - 1]); //push back the velocity of that on the left.
			structural.push_back(state[actual_index + 1]);//push back the position of that on the right.
			//structural.push_back(state[actual_index + 3]); //push back the velocity of that on the right.
			structural.push_back(state[actual_index + cloth_length]);//push back the position of that on the directly below.
			//structural.push_back(state[actual_index + cloth_length * 2 + 1]); //push back the velocity of that directly below.
			//cout << "top border" << endl;

			//shear springs
			shear.push_back(state[actual_index + cloth_length - 1]);//push back position of the bottom diagonal left one
			shear.push_back(state[actual_index + cloth_length + 1]);//push back position of the bottom diagonal right one

		}
	}
	else if (isbot) {
		if (isright) {
			// bottom right particle.

			//structural springs
			structural.push_back(state[actual_index - 1]);//push back the position of the particle on the left
			//structural.push_back(state[actual_index - 1]);//push back the velocity of the particle on the left
			structural.push_back(state[actual_index - cloth_length]);//push back the position of that directly above.
			//structural.push_back(state[actual_index - cloth_length * 2 + 1]); //push back the velocity of that directly above.
			//cout << "bot right" << endl;

			//shear springs
			shear.push_back(state[actual_index - cloth_length- 1]);//push back position of the top diagonal left one

		}
		else if (isleft) {
			//bottom left particle.

			//structural springs
			structural.push_back(state[actual_index + 1]);//push back the position of the particle on the right
			//structural.push_back(state[actual_index + 3]);//push back the velocity of the particle on the right
			structural.push_back(state[actual_index - cloth_length * 1]);//push back the position of that directly above.
			//structural.push_back(state[actual_index - cloth_length * 2 + 1]); //push back the velocity of that directly above.
			//cout << "bot left" << endl;

			//shear springs
			shear.push_back(state[actual_index - cloth_length + 1]);//push back position of the top diagonal right one
		}
		else {
			// only along the bottom border.

			//structural springs
			structural.push_back(state[actual_index + 1]);//push back the position of the particle on the right
			//structural.push_back(state[actual_index + 3]);//push back the velocity of the particle on the right
			structural.push_back(state[actual_index - 1]);//push back the position of the particle on the left
			//structural.push_back(state[actual_index + 3]);//push back the velocity of the particle on the left
			structural.push_back(state[actual_index - cloth_length]);//push back the position of that directly above.
			//structural.push_back(state[actual_index - cloth_length * 2 + 1]); //push back the velocity of that directly above.
			//cout << "bot border" << endl;

			//shear springs
			shear.push_back(state[actual_index - cloth_length - 1]);//push back position of the top diagonal left one
			shear.push_back(state[actual_index - cloth_length + 1]);//push back position of the top diagonal right one
		}
		//perform with knowledge of along bottom border
	}
	else {
		if (isright) {
			//just along right border.

			//structural springs
			structural.push_back(state[actual_index - 1]);//push back the position of the particle on the left
			//structural.push_back(state[actual_index + 3]);//push back the velocity of the particle on the left
			structural.push_back(state[actual_index - cloth_length]);//push back the position of that directly above.
			//structural.push_back(state[actual_index - cloth_length * 2 + 1]); //push back the velocity of that directly above.
			structural.push_back(state[actual_index + cloth_length]);//push back the position of that on the directly below.
			//structural.push_back(state[actual_index + cloth_length * 2 + 1]); //push back the velocity of that directly below.
			//cout << "just right border" << endl;

			//shear springs
			shear.push_back(state[actual_index - cloth_length - 1]);//push back position of the top diagonal left one
			shear.push_back(state[actual_index + cloth_length - 1]);//push back position of the bot diagonal left one
		}
		else if (isleft) {
			//just along left border

			//structural springs
			structural.push_back(state[actual_index + 1]);//push back the position of the particle on the right
			//structural.push_back(state[actual_index + 3]);//push back the velocity of the particle on the right
			structural.push_back(state[actual_index - cloth_length]);//push back the position of that directly above.
			//structural.push_back(state[actual_index - cloth_length * 2 + 1]); //push back the velocity of that directly above.
			structural.push_back(state[actual_index + cloth_length]);//push back the position of that on the directly below.
			//structural.push_back(state[actual_index + cloth_length * 2 + 1]); //push back the velocity of that directly below.
			//cout << "just left border" << endl;
			//shear springs
			shear.push_back(state[actual_index - cloth_length + 1]);//push back position of the top diagonal right one
			shear.push_back(state[actual_index + cloth_length + 1]);//push back position of the bot diagonal right one
		}
		else {
			//someone who's in the middle, along NO border whatsoever.

			//structural springs
			structural.push_back(state[actual_index + 1]);//push back the position of the particle on the right
			//structural.push_back(state[actual_index + 3]);//push back the velocity of the particle on the right
			structural.push_back(state[actual_index - 1]);//push back the position of the particle on the left
			//structural.push_back(state[actual_index + 3]);//push back the velocity of the particle on the left
			structural.push_back(state[actual_index - cloth_length]);//push back the position of that directly above.
			//structural.push_back(state[actual_index - cloth_length * 2 + 1]); //push back the velocity of that directly above.
			structural.push_back(state[actual_index + cloth_length]);//push back the position of that on the directly below.
			//structural.push_back(state[actual_index + cloth_length * 2 + 1]); //push back the velocity of that directly below.
			//cout << "middling" << endl;

			//shear springs
			shear.push_back(state[actual_index - cloth_length + 1]);//push back position of the top diagonal right one
			shear.push_back(state[actual_index + cloth_length  + 1]);//push back position of the bot diagonal right one
			shear.push_back(state[actual_index - cloth_length - 1]);//push back position of the top diagonal left one
			shear.push_back(state[actual_index + cloth_length - 1]);//push back position of the bot diagonal left one
		}
		//else no border condition.
	}
	vector<vector<Vector3f>> out;
	out.push_back(structural);
	out.push_back(shear);
	out.push_back(flexion);
	return out;
}

// TODO: implement evalF
// for a given state, evaluate f(X,t)
vector<Vector3f> ClothSystem::evalF(vector<Vector3f> state)
{
	Vector3f force;
	Vector3f displacement;
	vector<Vector3f> output;
	vector<Vector3f> positionals;
	for (int i = 0; i < state.size(); i = i + 2) {
		positionals.push_back(state[i]);
	}
	//cout << "*****************************************************" << endl;
	for (int i=0; i<positionals.size();i++){//accessing all relevant positional vectors
		float structspringRestLength = 0.5;
		float shearrestlength = structspringRestLength * sqrt(2.0);//pow(pow(structspringRestLength, 2)*2,0.5);
		//float shearrestlength = 0.5 * pow(2.0, 0.5);
		float flexrestlength = 1;
		float dragQueenConstant = 0.7; //drag affects all. this is the typical drag value for something like a plate perpendicular to flow in 3d.
		float structspringconst = 100;
		float sheark = 70;
		float flexk=  70;
		float displacement_val;
		float mass = 0.3;
		vector<vector<Vector3f>> out = get_relevant(positionals, i);
		vector<Vector3f> structural = out[0];
		vector<Vector3f> shear = out[1];
		vector<Vector3f> flexion= out[2];
		
		// you now have all relevant points. As usual, even - position, odd - velocity
		force = Vector3f(0, 0, 0);
		Vector3f grav = Vector3f(0, -9.81, 0) * mass;
		//Vector3f grav = Vector3f(0, 0, 0);
		Vector3f drag = dragQueenConstant * state[i*2 + 1]; //velo is after only.
		//account for struct
		//cout <<"State"<< endl;
		//positionals[i].print();
		//cout << structural.size() << endl;
		for (int k = 0; k<structural.size(); k++) {
			//cout << "struct" << endl;
			//structural[k].print();
			//cout << "state" << endl;
			//state[i].print();
			displacement = Vector3f(structural[k].x() - positionals[i].x(), structural[k].y() - positionals[i].y(), structural[k].z() - positionals[i].z());
			displacement_val = displacement.abs(); //get magnitude
			displacement.normalize(); //get direction...
			//cout << "displacement direction" << endl;
			//displacement.print();
			
			//cout << "displacement val" << endl;
			//cout << displacement_val << endl;
			displacement = displacement.normalized() * ((displacement_val - structspringRestLength) * structspringconst);
			force = Vector3f(force.x()+displacement.x(), force.y() + displacement.y(), force.z() + displacement.z());
			//cout << "____________________________________" << endl;
		}
		// account for shear...
		
		for (int k = 0; k < shear.size(); k++) {
			displacement = Vector3f(shear[k].x() - positionals[i].x(), shear[k].y() - positionals[i].y(), shear[k].z() - positionals[i].z());
			displacement_val = displacement.abs(); //get magnitude
			displacement = displacement.normalized() * ((displacement_val - shearrestlength) * sheark);
			force = Vector3f(force.x() + displacement.x(), force.y() + displacement.y(), force.z() + displacement.z());
		}
		

		//account for flexion.
		
		for (int k = 0; k < flexion.size(); k++) {
			displacement = Vector3f(flexion[k].x() - positionals[i].x(), flexion[k].y() - positionals[i].y(), flexion[k].z() - positionals[i].z());
			displacement_val = displacement.abs(); //get magnitude
			displacement.normalize();
			displacement = displacement.normalized() * ((displacement_val - flexrestlength) * flexk);
			force = Vector3f(force.x() + displacement.x(), force.y() + displacement.y(), force.z() + displacement.z());
		}
		

		//cout << "force" << endl;
		//force.print();
		//cout << "********************************************" << endl;
		//force is now the sum for this particular point...
		// you now have all relevant points,
		//we again keep in mind the top left and top right particle receives nothing.
		
		
		output.push_back(positionals[i]+ state[i*2+1]); //i.e new position push!
		output.push_back(state[i*2+1]+(force + grav - drag )/mass); // i.e new velocity push!
		//output.push_back(Vector3f(0, 0, 0));
		//output.push_back(Vector3f(0, 0, 0));
	}
	output[0] = Vector3f(0,0,0); //overwrite new pos derivative for top left
	output[1] = Vector3f(0, 0, 0); //overwrite new velo derivative for top left
	output[(cloth_length - 1) * 2]=Vector3f(0,0,0); //overwrite new pos derivative for top right
	output[(cloth_length - 1) * 2+1] = Vector3f(0, 0, 0); //overwrite new velo derivative for top right

	//since these are derivatives, we should just set them as zero.

	//output[0].print();
	//output[1].print();
	//output[(cloth_length - 1) * 2].print();
	//output[(cloth_length - 1) * 2 + 1].print();
	//cout << "called it." << endl;
	//cout << "*****************************************************" << endl;
	return output;
}

///TODO: render the system (ie draw the particles)
void ClothSystem::draw()
{
	vector<Vector3f> state = this->getState();
	for (int i = 0; i < state.size(); i = i + 2) {//accessing all relevant positional vectors
		//this->getState()[i].print();
		Vector3f pos = this->getState()[i];//  position of particle i. YOUR CODE HERE
		glPushMatrix();
		glTranslatef(pos[0], pos[1], pos[2]);
		glutSolidSphere(0.075f, 5.0f, 5.0f);
		glPopMatrix();
		
	}
}

