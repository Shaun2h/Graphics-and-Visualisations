#include "SkeletalModel.h"
#include <fstream>
#include <FL/Fl.H>

using namespace std;
void SkeletalModel::load(const char *skeletonFile, const char *meshFile, const char *attachmentsFile)
{
	loadSkeleton(skeletonFile);

	m_mesh.load(meshFile);
	m_mesh.loadAttachments(attachmentsFile, m_joints.size());

	computeBindWorldToJointTransforms();
	updateCurrentJointToWorldTransforms();
}

void SkeletalModel::draw(Matrix4f cameraMatrix, bool skeletonVisible)
{
	// draw() gets called whenever a redraw is required
	// (after an update() occurs, when the camera moves, the window is resized, etc)

	m_matrixStack.clear();
	m_matrixStack.push(cameraMatrix);

	if(  !skeletonVisible )
	{
		drawJoints();
       //####################################################################################################
	  //####################################################################################################
	  //####################################################################################################
	  //####################################################################################################
	  //####################################################################################################
	  //####################################################################################################
		drawSkeleton();
	}//####################################################################################################
	//####################################################################################################
	//####################################################################################################
	//####################################################################################################
	//####################################################################################################
	else
	{
		//drawJoints();
		//drawSkeleton();
		// Clear out any weird matrix we may have been using for drawing the bones and revert to the camera matrix.
		glLoadMatrixf(m_matrixStack.top());

		// Tell the mesh to draw itself.
		m_mesh.draw();
	}
}

void SkeletalModel::loadSkeleton( const char* filename )
{
	// Load the skeleton from file here.
	ifstream inp;
	inp.open(filename);
	if (!inp) {
		cerr << "file doesn't exist or something. anyway, "<< filename<< endl;
		cout << system("dir") << endl;
		exit(1);  //forcequit
	}
	string line;
	int counter = 0;
	string delimiter = " ";
	while (getline(inp, line)) {
		cout << line << endl;
		std::vector<std::string> result;
		std::istringstream iss(line);

		for (std::string line; iss >> line; ) {
			result.push_back(line); // split by spaces.
		}
		float x = ::atof(result[0].c_str()); 
		float y = ::atof(result[1].c_str());
		float z = ::atof(result[2].c_str());
		int parent  = ::atof(result[3].c_str());
		Joint* joint = new Joint;
		joint->transform = Matrix4f(
			1,0,0,x,
			0,1,0,y,
			0,0,1,z,
			0,0,0,1
			);
		m_joints.push_back(joint);
		if (parent != -1) { // if parent is not -1, i.e not root
			m_joints[parent]->children.push_back(joint);
		}
		else {
			m_rootJoint = joint; //it's the root joint.
			// WE DON'T PUSH IT INTO THE STACK AHHHH
		}
		counter += 1;
	}
	/*
	The first three fields are floating point numbers giving the joint’s translation relative to its parent joint. 
	The final field is the index of its parent (where a joint’s index is the zero-based order it occurs in the .skel file),
	*/

}

void SkeletalModel::drawJoints( )
{
	// Draw a sphere at each joint. You will need to add a recursive helper function to traverse the joint hierarchy.
	//
	// We recommend using glutSolidSphere( 0.025f, 12, 12 )
	// to draw a sphere of reasonable size.
	//
	recur_down(m_rootJoint, m_matrixStack); // if parsed correctly it should not require the actual m_joints class...

	// You are *not* permitted to use the OpenGL matrix stack commands
	// (glPushMatrix, glPopMatrix, glMultMatrix).
	// You should use your MatrixStack class
	// and use glLoadMatrix() before your drawing call.
}
void SkeletalModel::recur_down(Joint* inp,MatrixStack matty) {
	if (inp->children.size()>0) { //still has children...
		for (int i = 0; i < inp->children.size(); i++) { //look at each child...
		//since matty is outside, i don't need to return the matrix stack itself.
			matty.push(inp->transform);
			recur_down(inp->children[i], matty);
			//cout << "printed a ting" << endl;
			//matty.top().print();
			//matty.top().getRow(4).print();
			glLoadMatrixf(matty.top()); //push the appropriate thing.
			glutSolidSphere(0.025f, 12, 12);
			
			/*#################################################*/
			//Draw the box towards your child...
			Matrix4f target_location = inp->children[i]->transform; //looking at the child joint..
			Vector3f coords_of_childjoint = target_location.getCol(3).xyz();
			float distance = coords_of_childjoint.abs();
			//scale a->b
			//Vector3f a = Vector3f(-0.025f, -0.025f, 0);
			//Vector3f b = Vector3f(0.025f, 0.025f, distance);
			Matrix4f scaling = Matrix4f::scaling(0.025f, 0.025f, distance); //scaling matrix...
			Matrix4f translate = Matrix4f::translation(0.0f, 0.0f, 0.5f);
			Vector3f z = Vector3f(coords_of_childjoint.normalized()[0], coords_of_childjoint.normalized()[1], coords_of_childjoint.normalized()[2]);
			//so we begin by setting the child joint normalised
			Vector3f y = Vector3f::cross(Vector3f(0, 0, 1),z).normalized();
			//Vector3f x = Vector3f::cross(y, z).normalized();
			Matrix4f rotate = Matrix4f::rotation(y, acos(z[2]));
			matty.push(rotate*scaling*translate);
			// need to push the correct matrix..
			glLoadMatrixf(matty.top());
			glutSolidCube(1.0f);
			matty.pop();
			matty.pop();
		}
	}
	else {
		matty.push(inp->transform);
		glLoadMatrixf(matty.top()); //push the appropriate thing.
		glutSolidSphere(0.025f, 12, 12);
		//cout << "printed a ting" << endl;
		//matty.top().print();
		matty.pop();
	}
	
	
}

void SkeletalModel::drawSkeleton( )
{
	
	// Draw boxes between the joints. You will need to add a recursive helper function to traverse the joint hierarchy.
	// i guess i can ignore this and do it all in  draw joints.. right?
	
	//I DID IT IN DRAW JOINTS. DA POLIHIS HAS NOTHING ON ME
}

void SkeletalModel::setJointTransform(int jointIndex, float rX, float rY, float rZ)
{
	Joint* target_joint = m_joints[jointIndex];
	Matrix4f rotate = Matrix4f::rotateZ(rZ) * Matrix4f::rotateY(rY) * Matrix4f::rotateX(rX); //get rotation matrices...
	Matrix4f transformmat = target_joint->transform;
	transformmat.setSubmatrix3x3(0, 0, rotate.getSubmatrix3x3(0, 0)); //overwrite the submatrix at the top left since we mainly set the right col to our desired
	// original transform...
	// Set the rotation part of the joint's transformation matrix based on the passed in Euler angles.
	target_joint->transform = transformmat;
}


void SkeletalModel::computeBindWorldToJointTransforms()
{
	m_matrixStack.clear();
	// 2.3.1. Implement this method to compute a per-joint transform from
	// world-space to joint space in the BIND POSE.
	//
	// Note that this needs to be computed only once since there is only
	// a single bind pose.
	//
	// This method should update each joint's bindWorldToJointTransform.
	// You will need to add a recursive helper function to traverse the joint hierarchy.
	bindworldtojointrecur(m_rootJoint, m_matrixStack);
}

void SkeletalModel::bindworldtojointrecur(Joint* targetjoint, MatrixStack matrixStack) {
	//Update the currentJointToWorldTransform matrix of each Joint
	matrixStack.push(targetjoint->transform); // push parent's transform
	for (int i = 0; i < targetjoint->children.size(); i++) {
		bindworldtojointrecur(targetjoint->children[i], matrixStack); //go into each child and... do.
	}
	targetjoint->bindWorldToJointTransform = matrixStack.top(); //do self. after children are done...
	matrixStack.pop(); 
	cout << "bind" << endl;
}

void SkeletalModel::Jointtoworldrecurupdate(Joint* joint, MatrixStack matty) {
	matty.push(joint->transform);
	for (int i = 0; i < joint->children.size(); i++) {
		Jointtoworldrecurupdate(joint->children[i], matty);
	}
	joint->currentJointToWorldTransform = matty.top();
	matty.pop();
}

void SkeletalModel::updateCurrentJointToWorldTransforms()
{
	m_matrixStack.clear();
	// 2.3.2. Implement this method to compute a per-joint transform from
	// joint space to world space in the CURRENT POSE.
	//
	// The current pose is defined by the rotations you've applied to the
	// joints and hence needs to be *updated* every time the joint angles change.
	//
	// This method should update each joint's bindWorldToJointTransform.
	// You will need to add a recursive helper function to traverse the joint hierarchy.	
	Jointtoworldrecurupdate(m_rootJoint, m_matrixStack);
}

void SkeletalModel::updateMesh()
{
	// 2.3.2. This is the core of SSD.
	// Implement this method to update the vertices of the mesh
	// given the current state of the skeleton.
	// You will need both the bind pose world --> joint transforms.
	// and the current joint --> world transforms.
	float currentWeight;
	
	Vector4f current_item;
	Vector4f new_vector;
	Vector4f transformationUpdate;
	vector<float> vertexweights;

	for (int i = 0; i < m_mesh.currentVertices.size(); i++)
	{
		vertexweights = m_mesh.attachments[i];
		current_item = Vector4f(m_mesh.bindVertices[i][0], m_mesh.bindVertices[i][1], m_mesh.bindVertices[i][2], 1);
		// so we obtain the current vertex
		new_vector = Vector4f(0, 0, 0, 0);
		for (int j = 0; j < vertexweights.size(); j++) { //for each within the vertex weights, which should be for all vertexes
			//new_vector = current_sum + actual weights <access via ij> and then, follow that formula... need to slowly work out.. 
			//m_joints[j]->bindWorldToJointTransform.print();
			new_vector = new_vector + m_mesh.attachments[i][j] *
				((m_joints[j]->currentJointToWorldTransform) * (m_joints[j]->bindWorldToJointTransform.inverse()) * current_item);
		}
		m_mesh.currentVertices[i] = new_vector.xyz(); //place the summed version...
		//new_vector.print();
		
	}

} 

