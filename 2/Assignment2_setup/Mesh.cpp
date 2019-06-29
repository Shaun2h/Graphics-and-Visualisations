#include "Mesh.h"

using namespace std;

void Mesh::load( const char* filename )
{
	// 2.1.1. load() should populate bindVertices, currentVertices, and faces
	cout << "FILE" << endl;
	cout << filename << endl;
	//string file_name = filename
	string file_name = filename;
	string indicator;
	string line;
	string ting;
	ifstream file(filename);
	if (file.is_open()) {
		while(getline(file, line)) {
			stringstream ss(line);
			ss >> ting;
			if (ting == "f") {
				ss >> indicator;
				unsigned int x = ::atof(indicator.c_str());
				ss >> indicator;
				unsigned int y = ::atof(indicator.c_str());
				ss >> indicator;
				unsigned int z = ::atof(indicator.c_str());
				faces.push_back(Tuple3u(x, y, z));
			}
			else if (ting == "v") {
				ss >> indicator;
				float x = ::atof(indicator.c_str());
				ss >> indicator;
				float y = ::atof(indicator.c_str());
				ss >> indicator;
				float z = ::atof(indicator.c_str());
				bindVertices.push_back(Vector3f(x, y, z));
			}
			else {
				cout << "Unable to open file";
			}
		}
		file.close();
		/*std::istream in(&file);
		while (in >> indicator) { //still have stuff
			if (indicator == "f") {
				in >> indicator;
				unsigned int x = ::atof(indicator.c_str());
				in >> indicator;
				unsigned int y = ::atof(indicator.c_str());
				in >> indicator;
				unsigned int z = ::atof(indicator.c_str());
				faces.push_back(Tuple3u(x, y, z));
			}
			if (indicator == "v") {
				in >> indicator;
				float x = ::atof(indicator.c_str());
				in >> indicator;
				float y = ::atof(indicator.c_str());
				in >> indicator;
				float z = ::atof(indicator.c_str());
				bindVertices.push_back(Vector3f(x, y, z));
			}
		}*/
	}
	
	//faces.push_back();
	// Add your code here.
	// make a copy of the bind vertices as the current vertices
	currentVertices = bindVertices;
}

void Mesh::draw()
{
	// Since these meshes don't have normals
	// be sure to generate a normal per triangle.
	// Notice that since we have per-triangle normals
	// rather than the analytical normals from
	// assignment 1, the appearance is "faceted".

	cout << "Total Faces: " << faces.size()<<endl;
	int counter = 0;
	Tuple3u latest_face;
	Vector3f norm1;
	Vector3f norm2;
	Vector3f norm3;
	Vector3f a;
	Vector3f b;
	Vector3f c;

	for (int i = 0; i < faces.size(); i++) {
		//cout << "new" << endl;
		latest_face = faces[i];
		a = currentVertices[latest_face[0]-1];
		b = currentVertices[latest_face[1]-1]; //look at the current face values. just like the old code, look at face to determine which vertice to use.
		c = currentVertices[latest_face[2]-1];
		norm1 = Vector3f::cross(b - a, c - a); //get norms.
		norm1.normalize();
		//norm1.print();
		norm2 = Vector3f::cross(c - b, a - b);
		norm2.normalize();
		//norm2.print();
		norm3 = Vector3f::cross(a - c, b - c);
		norm3.normalize(); // all are directional vectors. and normied.
		//norm3.print();

		glBegin(GL_TRIANGLES);
		glNormal3d(norm1[0], norm1[1], norm1[2]); //the first vertex and normal
		glVertex3d(a[0], a[1], a[2]);


		glNormal3d(norm2[0], norm2[1], norm2[2]);
		glVertex3d(b[0], b[1], b[2]);


		glNormal3d(norm3[0], norm3[1], norm3[2]);
		glVertex3d(c[0], c[1], c[2]);
		glEnd();
		counter += 1;
		if (counter % 1000 == 0) {
			cout << counter << endl;
	}
	
		
		
	}

	cout << "done" << endl;
}



void Mesh::loadAttachments(const char* filename, int numJoints)
{
	ifstream file(filename);
	string line;
	string ting; //latest thing
	cout << filename << endl;
	ifstream filez(filename);

	if (file.is_open()) {
		while (getline(filez, line)) {
			stringstream ss(line);
			vector<float> latest;
			while (ss >> ting) {//we still got stuff right
				float temp = ::atof(ting.c_str());
				latest.push_back(temp);
			}
			//cout << latest.size() << endl;
			attachments.push_back(latest);
			// 2.2. Implement this method to load the per-vertex attachment weights
			// this method should update m_mesh.attachments
		}
	}
}
