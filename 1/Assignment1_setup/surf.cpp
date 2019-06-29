#include "surf.h"
#include "extra.h"

#include <math.h> // you need this for pi...
using namespace std;

namespace
{
    
    // We're only implenting swept surfaces where the profile curve is
    // flat on the xy-plane.  This is a check function.
    static bool checkFlat(const Curve &profile)
    {
        for (unsigned i=0; i<profile.size(); i++)
            if (profile[i].V[2] != 0.0 ||
                profile[i].T[2] != 0.0 ||
                profile[i].N[2] != 0.0)
                return false;
    
        return true;
    }
}

Surface makeSurfRev(const Curve& profile, unsigned steps)
{
	Surface surface;
	struct Surface
	{
		std::vector< Vector3f > VV;
		std::vector< Vector3f > VN;
		std::vector< Tup3u > VF;
	};

	if (!checkFlat(profile))
	{
		cerr << "surfRev profile curve must be flat on xy plane." << endl;
		//exit(0);
	}
	cout << "Surface incoming." << endl;
	/*for (unsigned k = 0; k < profile.size(); k++) {
		cout << "ting"<< k << endl;
		profile[k].V.print();
		profile[k].T.print();
		profile[k].N.print();
		profile[k].B.print();
	}*/
	//again, steps will help to define the amount of definition you append to the relevant 
    // TODO: Here you should build the surface.  See surf.h for details.
	for (unsigned i = 0; i < steps; i++) { //steps is sweep definition. As a result, this doesn't matter for the tup3u
		float latest_angle = (float)2 * M_PI / float(steps) * (float)i; //i'm not sure if i'm sweeping it the right direction though..
		Matrix4f rotationfirst; 
		rotationfirst = rotationfirst.rotateY(latest_angle);
		Matrix3f rotationsecond = rotationfirst.getSubmatrix3x3(0, 0);
		rotationsecond = rotationsecond.inverse(); //Apparently need to write out the shapes... so 3 and 4 here for match.
		rotationsecond.transposed(); //transpose it.
		for (unsigned k = 0; k < profile.size(); k++) { // this part varies the points accordingly...
			CurvePoint latest = profile[k];

			Vector3f vertex = (rotationfirst * Vector4f(latest.V, 1)).xyz(); // multiply a vertex by the first rotation matrix.
			Vector3f normal = (rotationsecond * latest.N * (-1)).normalized(); //multiply the normal by the second rotation
			/*
			cout << "V" << endl;
			latest.V.print();
			cout << "N" << endl;
			latest.N.print();
			*/
			/*cout<< "rotation first"<<endl;
			rotationfirst.print();
			cout << "rotation second" << endl;
			rotationsecond.print();*/
			/*
			cout << "vertex" << endl;
			vertex.print();
			cout << "normal" << endl;
			normal.print();
			*/
			surface.VV.push_back(vertex);
			surface.VN.push_back(normal); //push the normals and vertexes in
			if (k < profile.size() - 1) {
				//unsigned int start = (k + steps * k)%(profile.size()*steps);
				//unsigned int next = (start + profile.size()) % (profile.size()*steps);
				unsigned int ref = k + profile.size() * i; //reference point
				unsigned int next = (ref + profile.size()) % (steps * profile.size()); //points to opposite side of the line.
				Tup3u a(ref, ref +1,next);
				Tup3u b(ref +1,next+1,next);
				surface.VF.push_back(a);
				surface.VF.push_back(b);

			}
		}

	}
    //cerr << "\t>>> makeSurfRev called (but not implemented).\n\t>>> Returning empty surface." << endl;
	cout << "did the ting! ting!" << endl;
    return surface;
}

Surface makeGenCyl(const Curve &profile, const Curve &sweep )
{
    Surface surface;
	for (unsigned i = 0; i < profile.size(); i++) {// essentially for all the points where we want to perform the cylinder.
		for (unsigned k = 0; k < sweep.size(); k++) {// for each of the points within the sweep...
			Matrix4f sweepcoordinates = Matrix4f(
				sweep[k].N[0], sweep[k].B[0], sweep[k].T[0], sweep[k].V[0],
				sweep[k].N[1], sweep[k].B[1], sweep[k].T[1], sweep[k].V[1],
				sweep[k].N[2], sweep[k].B[2], sweep[k].T[2], sweep[k].V[2],
				0, 0, 0, 1
			);
			Matrix3f norms = sweepcoordinates.getSubmatrix3x3(0, 0).transposed().inverse(); //you obtain the norms...?
			Vector4f profile_coords(profile[i].V.xyz(), 1); //you obtain the xyz of the profile coordinates.
			Vector3f vertex = (sweepcoordinates * profile_coords).xyz();
			Vector3f normie = (norms * profile[i].N.xyz()).normalized();
			surface.VV.push_back(vertex);
			surface.VN.push_back(normie);
		}
		
		
	}
	for (unsigned int k = 0; k < surface.VV.size() - (sweep.size()); k++) {

		Tup3u a;
		Tup3u b;

		/*if ((k + 1) % (sweep.size()) != 0) {
			a = Tup3u(k + 1, k, k + sweep.size()); // begin by looking at each piece, it's neighbour, and then the one directly in the next line.
			b = Tup3u(k + 1, k + sweep.size(), k + 1 + sweep.size()); //repeat but in the opposing direction for the other triangle.
		}// this works due to sweep being for a point on each line.
		else {*/
			a = Tup3u((k + 1)%surface.VV.size(), k, (k) % surface.VV.size() + sweep.size()); // begin by looking at each piece, it's neighbour, and then the one directly in the next line.
			b = Tup3u((k + 1) % surface.VV.size(), (k) % surface.VV.size() + sweep.size(), (k + 1 + sweep.size()) % surface.VV.size()); //repeat but in the opposing direction for the other triangle.
		//}
		surface.VF.push_back(a);
		surface.VF.push_back(b);

	}

    if (!checkFlat(profile))
    {
        cerr << "genCyl profile curve must be flat on xy plane." << endl;
        //exit(0);
    }

    // TODO: Here you should build the surface.  See surf.h for details.

    //cerr << "\t>>> makeGenCyl called (but not implemented).\n\t>>> Returning empty surface." <<endl;
	Surface surface1;
    return surface;
}
	
void drawSurface(const Surface &surface, bool shaded)
{
    // Save current state of OpenGL
    glPushAttrib(GL_ALL_ATTRIB_BITS);

    if (shaded)
    {
        // This will use the current material color and light
        // positions.  Just set these in drawScene();
        glEnable(GL_LIGHTING);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        // This tells openGL to *not* draw backwards-facing triangles.
        // This is more efficient, and in addition it will help you
        // make sure that your triangles are drawn in the right order.
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
    }
    else
    {        
        glDisable(GL_LIGHTING);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        
        glColor4f(0.4f,0.4f,0.4f,1.f);
        glLineWidth(1);
    }

    glBegin(GL_TRIANGLES);
    for (unsigned i=0; i<surface.VF.size(); i++)
    {
        glNormal(surface.VN[surface.VF[i][0]]);
        glVertex(surface.VV[surface.VF[i][0]]);
        glNormal(surface.VN[surface.VF[i][1]]);
        glVertex(surface.VV[surface.VF[i][1]]);
        glNormal(surface.VN[surface.VF[i][2]]);
        glVertex(surface.VV[surface.VF[i][2]]);
    }
    glEnd();

    glPopAttrib();
}

void drawNormals(const Surface &surface, float len)
{
    // Save current state of OpenGL
    glPushAttrib(GL_ALL_ATTRIB_BITS);

    glDisable(GL_LIGHTING);
    glColor4f(0,1,1,1);
    glLineWidth(1);

    glBegin(GL_LINES);
    for (unsigned i=0; i<surface.VV.size(); i++)
    {
        glVertex(surface.VV[i]);
        glVertex(surface.VV[i] + surface.VN[i] * len);
    }
    glEnd();

    glPopAttrib();
}

void outputObjFile(ostream &out, const Surface &surface)
{
    
    for (unsigned i=0; i<surface.VV.size(); i++)
        out << "v  "
            << surface.VV[i][0] << " "
            << surface.VV[i][1] << " "
            << surface.VV[i][2] << endl;

    for (unsigned i=0; i<surface.VN.size(); i++)
        out << "vn "
            << surface.VN[i][0] << " "
            << surface.VN[i][1] << " "
            << surface.VN[i][2] << endl;

    out << "vt  0 0 0" << endl;
    
    for (unsigned i=0; i<surface.VF.size(); i++)
    {
        out << "f  ";
        for (unsigned j=0; j<3; j++)
        {
            unsigned a = surface.VF[i][j]+1;
            out << a << "/" << "1" << "/" << a << " ";
        }
        out << endl;
    }
}
