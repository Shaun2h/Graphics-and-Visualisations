#include "curve.h"
#include "extra.h"
#ifdef WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
using namespace std;

namespace
{
    // Approximately equal to.  We don't want to use == because of
    // precision issues with floating point.
    inline bool approx( const Vector3f& lhs, const Vector3f& rhs )
    {
        const float eps = 1e-8f;
        return ( lhs - rhs ).absSquared() < eps;
    }

    
}
Vector3f last_Binormal;
bool midgen = FALSE;
Curve evalBezier( const vector< Vector3f >& P, unsigned steps )
{
    // Check
    if( P.size() < 4 || P.size() % 3 != 1 )
    {
        cerr << "evalBezier must be called with 3n+1 control points." << endl;
        exit( 0 );
    }
	cout << "evalBezier triggered." << endl;
    // TODO:
    // You should implement this function so that it returns a Curve
    // (e.g., a vector< CurvePoint >).  The variable "steps" tells you
    // the number of points to generate on each piece of the spline.
    // At least, that's how the sample solution is implemented and how
    // the SWP files are written.  But you are free to interpret this
    // variable however you want, so long as you can control the
    // "resolution" of the discretized spline curve with it.
	for (int i = 0; i < P.size(); ++i)
		std::cout << P[i] << ' ';
	Curve curve(steps+1);
	Vector3f tempy_N, tempy_B;
	Matrix4f basemat = Matrix4f(
		1, -3, 3, -1,
		0, 3, -6, 3,
		0, 0, 3, -3,
		0, 0, 0, 1);
	Matrix4f diff_mat = Matrix4f(
		-3, 6, -3, 0,
		3, -12, 9, 0,// we have to manually math this out.
		0, 6, -9, 0,
		0, 0, 3, 0);
	
	Matrix4f controls;
	Vector4f canon, temp, tangent;
	for (unsigned i = 0; i < P.size() - 3; i++) {
		controls = Matrix4f(Vector4f(P[i], 0), Vector4f(P[i + 1], 0), Vector4f(P[i + 2], 0), Vector4f(P[i + 3], 0)); // i honestly have no understanding as to the w, and what it represents. we aren't using xyzw coords so it's a lil confusing?
		// make the control points matrix.
		for (unsigned k = 0; k <= steps; ++k) {
			float tval = (float)1 / steps * k;
			canon = Vector4f(1, tval, pow(tval, 2), pow(tval, 3));
			temp = controls * basemat * canon;//now use the formula
			curve[k].V = temp.xyz(); //only take the xyz...

			tangent = controls * diff_mat * canon;
			tangent.normalize();

			curve[k].T = tangent.xyz();

			if (k == 0 && i==0 && !midgen) { // is this the first of many?
				//curve[k].B = Vector3f(0, 0, -1); //arbitrary. just not tangent.
				curve[k].B = Vector3f(0, 0, -1);
				//tempy_B = last_Binormal;
				//tempy_N=
				curve[k].N = Vector3f::cross(curve[k].B, curve[k].T).normalized();//normalise
				last_Binormal = curve[k].B;
			}
			else {
				curve[k].N = Vector3f::cross(last_Binormal, curve[k].T).normalized();
				curve[k].B= Vector3f::cross(curve[k].T, curve[k].N).normalized();
				last_Binormal = curve[k].B;
				//last_Binormal = tempy_B;

			}
			//curve[k].B= tempy_B.normalized();
			//curve[k].N = tempy_N.normalized();
			
		}

	}
	//https://www.cs.helsinki.fi/group/goa/mallinnus/curves/curves.html
	/*for (unsigned i = 0; i < P.size() - 3; i++) {
		for (unsigned k = 0; k < steps; ++k) { //for each of the steps
			cout << "bezier" << endl;
			cout << k << endl;
			curve[k].B.print();
			curve[k].V.print();
			curve[k].N.print();
			curve[k].T.print();
		}
	}*/
	/*
	for (unsigned k = 0; k < steps; ++k) { //for each of the steps
		//since steps appear to contribute to the curve via defining the amount of definition that is required,
		float tval = 1 / steps * k; // calculate the current t value..
		ret[k].V = -3 * pow(1 - tval, 2) * P[0] + 3 * (pow(1 - tval, 2) - 6 * tval * (1 - tval)) * P[1] + (6 * tval * (1 - tval) - 3 * pow(tval, 2)) * P[2] + pow(tval, 2) * P[3];
		ret[k].T = 6 * (1 - tval) * P[0] + P[1] * (-6 * (1 - tval) - 6 + 12 * tval) + P[2] * (6 - 12 * tval - 6 * tval) + P[3] * 2 * tval;
		ret[k].T = ret[k].T.normalized(); // normalise it.
		if (k == 0) {
			ret[k].N.cross(ret[k].T + Vector3f(1, 1, 1),ret[k].T).normalized();  // add an arbitrary amount to T to ensure it's never parallel
			ret[k].B = ret[k].T + Vector3f(1, 1, 1);//set B as it was...
			//This is a recursive update so... this statement MUST only be applied if it's 0.
		}
		else {
			ret[k].N.cross(ret[k-1].T,ret[k].T).normalized();  // add an arbitrary amount to T to ensure it's never parallel
			ret[k].B.cross(ret[k].T, ret[k].N).normalized();// we assume B is chosen arbitrarily.
		}
		
		// it seems that only the first
		
	}
	*/
	//cerr << "\t>>> Bezier Steps (type steps): " << steps << endl;
	//cerr << "\t>>> Returning empty Bezier curve." << endl;
	//so that bit didn't work. no idea why since i got the math right.. maybe i forgot a loop?

	return curve;





    // Make sure that this function computes all the appropriate
    // Vector3fs for each CurvePoint: V,T,N,B.
    // [NBT] should be unit and orthogonal.

    // Also note that you may assume that all Bezier curves that you
    // receive have G1 continuity.  Otherwise, the TNB will not be
    // be defined at points where this does not hold.

    //cerr << "\t>>> evalBezier has been called with the following input:" << endl;

    //cerr << "\t>>> Control points (type vector< Vector3f >): "<< endl;
    /*for( unsigned i = 0; i < P.size(); ++i )
    {
        cerr << "\t>>> " << P[i] << endl;
		
    }*/

    //cerr << "\t>>> Steps (type steps): " << steps << endl;
    //cerr << "\t>>> Returning empty curve." << endl;

    // Right now this will just return this empty curve.
    //return Curve();
}

Curve evalBspline( const vector< Vector3f >& P, unsigned steps )
{
    // Check
    if( P.size() < 4 )
    {
        //cerr << "evalBspline must be called with 4 or more control points." << endl;
        exit( 0 );
    }
	cout << "evalBspline triggered." << endl;
    // TODO:
    // It is suggested that you implement this function by changing
    // basis from B-spline to Bezier.  That way, you can just call
    // your evalBezier function.

    //cerr << "\t>>> evalBSpline has been called with the following input:" << endl;

    //cerr << "\t>>> Control points (type vector< Vector3f >): "<< endl;
	Curve ret = Curve((P.size()-3)*(steps+1)); //  master curve. you can only put up to -3 of max.
    for( unsigned i = 0; i < P.size(); ++i )
    {
        cerr << "\t>>> " << P[i] << endl;
    }

	Matrix4f beziermat = Matrix4f(
		1, -3, 3, -1,
		0, 3, -6, 3,
		0, 0, 3, -3,
		0, 0, 0, 1);

	float coeff = (float)1 / (float)6;
	Matrix4f splinemat = Matrix4f(
		1* coeff, -3 * coeff, 3 * coeff, -1 * coeff,
		4 * coeff, 0 * coeff, -6 * coeff, 3 * coeff,
		1 * coeff, 3 * coeff, 3 * coeff, -3 * coeff,
		0 * coeff, 0 * coeff, 0 * coeff, 1 * coeff);

	int finaloutputcounter = 0;

	for (unsigned i = 0; i < P.size() - 3; i += 1) {
		Matrix4f controlpoints = Matrix4f(Vector4f(P[i],0), Vector4f(P[i+1], 0), Vector4f(P[i+2], 0), Vector4f(P[i+3], 0));
		Matrix4f converted = controlpoints * splinemat * beziermat.inverse(); //convert over to a bezier type
		vector<Vector3f> newpoints; //store the points in here.
		for (unsigned k = 0; k < 4; k++) { // push all 4 back..
			newpoints.push_back(converted.getCol(k).xyz());
		}
		Curve bezierout = evalBezier(newpoints, steps);
		midgen = TRUE;
		for (int k = 0; k < bezierout.size(); k++) {
			ret[finaloutputcounter].V = bezierout[k].V;
			ret[finaloutputcounter].T = bezierout[k].T;
			ret[finaloutputcounter].N = bezierout[k].N;
			ret[finaloutputcounter].B = bezierout[k].B;
			finaloutputcounter++;
		}
	}
	midgen = FALSE;



	/*
	for (unsigned i = 0; i < P.size()-3; i++) { // -3 here because we can only go up to -3 of the end...
		vector<Vector3f> segment(P.begin()+i,P.begin()+i+4);  //a segment of the spline
		// no way to see what's going on...
		for (unsigned k = 0; k < steps; ++k) {
			float tval = (float)1 / steps * (float)k; // calculate the current t value of your thing
			ret[k].V = (float)1 / (float)6 * pow(1 - tval, 3) * segment[0] +
				(float)1 / (float)6 * ((float)3 * pow(tval, 3) - (float)6 * pow(tval, 2) + (float)4) * segment[1] +
					(float)1 / (float)6 * ((float)-3 * pow(tval, 3) + (float)3 * pow(tval, 2) + (float)3 * (float)tval + (float)1) * segment[2] +
					(float)1 / (float)6 * pow(tval, 3) * segment[3];
			ret[k].T = -(float)1 / (float)2 * pow(1 - tval, 2) * segment[0] +
				((float)9 / (float)6 * pow(tval, 2) - (float)2 * tval) * segment[1] +
				((float)-9 / (float)6 * (pow(tval, 2))+tval+0.5f) * segment[2] +
					(float)1 / (float)2 * pow(tval, 2) * segment[3];
			//cout << "T" << endl;
			//ret[k].T.print();
			if (k == 0) {
				Vector3f zzz = ret[k].T + Vector3f(0, 0, 1);
				cout << "trig trig" << endl;
				cout << "T" << endl;
				ret[k].T.print();
				cout << "zzz" << endl;
				zzz.print();
				Vector3f kzkz;
				kzkz = zzz* ret[k].T;
				cout << "kzkz" << endl;
				kzkz.print();
				ret[k].N = (ret[k].T + Vector3f(0, 0, 1)) * ret[k].T; // again the normal is the randomised bit here.
				//ret[k].N.cross(ret[k].T + Vector3f(0, 0, 1), ret[k].T);
				ret[k].N.normalize();
				//cout << "N" << endl;
				//ret[k].N.print();
			}
			else {
				ret[k].N = (ret[k - 1].B * ret[k].T); //else calculate based off the old version.
				//ret[k].N.cross(ret[k - 1].B,ret[k].T);
				ret[k].N.normalize();
				//cout << "N" << endl;
				//ret[k].N.print();
			}
			//cout << "B" << endl;
			ret[k].B = ret[k].T * ret[k].N;
			//ret[k].B.print();
			//ret[k].B.cross(ret[k].T, ret[k].N).normalize();
			ret[k].B.normalize();
		}
	}
	*/
    //cerr << "\t>>> Steps (type steps): " << steps << endl;
    //cerr << "\t>>> Returning empty curve." << endl;
	
	// didn't work either.. do i HAVE to matrix?
	// i fixed it. i forgot floats UwU
	// and since i UwU i shall OwO

	/*for (unsigned i = 0; i < P.size() - 3; i++) {
		for (unsigned k = 0; k < steps; ++k) { //for each of the steps
			cout << k << endl;
			ret[k].B.print();
			ret[k].V.print();
			ret[k].N.print();
			ret[k].T.print();
		}
	}*/


    // Return an empty curve right now.
    return ret;
}

Curve evalCircle( float radius, unsigned steps )
{
    // This is a sample function on how to properly initialize a Curve
    // (which is a vector< CurvePoint >).
    
    // Preallocate a curve with steps+1 CurvePoints
    Curve R( steps+1 );

    // Fill it in counterclockwise
    for( unsigned i = 0; i <= steps; ++i )
    {
        // step from 0 to 2pi
        float t = 2.0f * M_PI * float( i ) / steps;

        // Initialize position
        // We're pivoting counterclockwise around the y-axis
        R[i].V = radius * Vector3f( cos(t), sin(t), 0 );
        
        // Tangent vector is first derivative
        R[i].T = Vector3f( -sin(t), cos(t), 0 );
        
        // Normal vector is second derivative
        R[i].N = Vector3f( -cos(t), -sin(t), 0 );

        // Finally, binormal is facing up.
        R[i].B = Vector3f( 0, 0, 1 );
    }

    return R;
}

void drawCurve( const Curve& curve, float framesize )
{
    // Save current state of OpenGL
    glPushAttrib( GL_ALL_ATTRIB_BITS );

    // Setup for line drawing
    glDisable( GL_LIGHTING ); 
    glColor4f( 1, 1, 1, 1 );
    glLineWidth( 1 );
    
    // Draw curve
    glBegin( GL_LINE_STRIP );
    for( unsigned i = 0; i < curve.size(); ++i )
    {
        glVertex( curve[ i ].V );
    }
    glEnd();

    glLineWidth( 1 );

    // Draw coordinate frames if framesize nonzero
    if( framesize != 0.0f )
    {
        Matrix4f M;

        for( unsigned i = 0; i < curve.size(); ++i )
        {
            M.setCol( 0, Vector4f( curve[i].N, 0 ) );
            M.setCol( 1, Vector4f( curve[i].B, 0 ) );
            M.setCol( 2, Vector4f( curve[i].T, 0 ) );
            M.setCol( 3, Vector4f( curve[i].V, 1 ) );

            glPushMatrix();
            glMultMatrixf( M );
            glScaled( framesize, framesize, framesize );
            glBegin( GL_LINES );
            glColor3f( 1, 0, 0 ); glVertex3d( 0, 0, 0 ); glVertex3d( 1, 0, 0 );
            glColor3f( 0, 1, 0 ); glVertex3d( 0, 0, 0 ); glVertex3d( 0, 1, 0 );
            glColor3f( 0, 0, 1 ); glVertex3d( 0, 0, 0 ); glVertex3d( 0, 0, 1 );
            glEnd();
            glPopMatrix();
        }
    }
    
    // Pop state
    glPopAttrib();
}

