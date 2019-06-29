#include "GL/freeglut.h"
#include <cmath>
#include <iostream>
#include <sstream>
#include <vector>
#include "vecmath.h"
using namespace std;

// Globals
const int MAX_BUFFER_SIZE = 300;
char buffer[MAX_BUFFER_SIZE];
// This is the list of points (3D vectors)
vector<Vector3f> vecv;
vector<float[3]> a;
bool nothing = TRUE;
bool dorotate = FALSE;
bool rotational = FALSE;
float angle = 0;
float currentX = 5;
float currentY = 0; //intial values.
float increment = 0.1;
// This is the list of normals (also 3D vectors)
vector<Vector3f> vecn;

// This is the list of faces (indices into vecv and vecn)
vector<vector<unsigned> > vecf;
int toggler = 0; // used to toggle between the 4 possible indexes she has..
int lightmovex = 0;
int lightmovey = 0;
// You will need more global variables to implement color and position changes


// These are convenience functions which allow us to call OpenGL 
// methods on Vec3d objects
inline void glVertex(const Vector3f &a) 
{ glVertex3fv(a); }

inline void glNormal(const Vector3f &a) 
{ glNormal3fv(a); }


// This function is called whenever a "Normal" key press is received.
void keyboardFunc( unsigned char key, int x, int y )
{
    switch ( key )
    {
    case 27: // Escape key
        exit(0);
        break;
    case 'c':
        // add code to change color here
		toggler += 1;
		toggler = toggler % 4;
		cout << "Toggled color." << endl; 
        break;
	case 'r':
		dorotate = !dorotate;
		cout << "toggled rotate" << endl;
		break;
    default:
        cout << "Unhandled key press " << key << "." << endl;        
    }

	// this will refresh the screen so that the user sees the color change
    glutPostRedisplay();
}

// This function is called whenever a "Special" key press is received.
// Right now, it's handling the arrow keys.
void specialFunc( int key, int x, int y )
{
    switch ( key )
    {
    case GLUT_KEY_UP:
        // add code to change light position
		lightmovey += 1;
		cout << "The light moved up" << endl;
		break;
    case GLUT_KEY_DOWN:
		lightmovey -= 1;
        // add code to change light position
		cout << "The light moved down" << endl;
		break;
    case GLUT_KEY_LEFT:
		lightmovex -= 1;
        // add code to change light position
		cout << "The light moved left" << endl;
		break;
    case GLUT_KEY_RIGHT:
		lightmovex += 1;
        // add code to change light position
		cout << "The light moved right" << endl;
		break;
    }

	// this will refresh the screen so that the user sees the light position
    glutPostRedisplay();
}

// This function is responsible for displaying the object.
void drawScene(void){
    // Clear the rendering window
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (dorotate){
		angle += 3;	
		gluLookAt(0.0, 0.0, 5.0,
			0.0, 0.0, 0.0,
			0.0, 1.0, 0.0);
	}
	else {
		gluLookAt(0.0, 0.0, 5.0,
			0.0, 0.0, 0.0,
			0.0, 1.0, 0.0);
	}
    // Rotate the image
    glMatrixMode( GL_MODELVIEW );  // Current matrix affects objects positions
    glLoadIdentity();              // Initialize to the identity

    // Position the camera at [0,0,5], looking at [0,0,0],
    // with [0,1,0] as the up direction.
	/*if (!dorotate){
		gluLookAt(0.0, 0.0, 5.0,
		0.0, 0.0, 0.0,
		0.0, 1.0, 0.0);
	}
	else {
		cout << "A rotate was called" << endl;
		gluLookAt(currentX, 0.0, currentY,
			0.0, 0.0, 0.0,
			0.0, 1.0, 0.0);
		cout << currentX << "  " << currentY << endl;
		if (abs(currentX)>5) {
			rotational = !rotational; // flip out the rotationals..
		}
		if (rotational) {
			currentX += increment/2;
			currentY = -sqrt(pow(5, 2) - pow(currentX, 2));
		}
		else {
			currentX -= increment/2;
			currentY = sqrt(pow(5, 2) - pow(currentX, 2));
		}
		
		
		cout << currentX << "  " << currentY << endl;

	}*/
	

    // Set material properties of object

	// Here are some colors you might use - feel free to add more
    GLfloat diffColors[4][4] = { {0.5, 0.5, 0.9, 1.0},
                                 {0.9, 0.5, 0.5, 1.0},
                                 {0.5, 0.9, 0.3, 1.0},
                                 {0.3, 0.8, 0.9, 1.0} };
    
	// Here we use the first color entry as the diffuse color
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, diffColors[toggler]);

	// Define specular color and shininess
    GLfloat specColor[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat shininess[] = {100.0};

	// Note that the specular color and shininess can stay constant
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specColor);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
  
    // Set light properties
    // Light color (RGBA)
    GLfloat Lt0diff[] = {1.0,1.0,1.0,1.0};
    // Light position
	GLfloat Lt0pos[] = {1.0f+lightmovex, 1.0f + lightmovey, 5.0f, 1.0f};

    glLightfv(GL_LIGHT0, GL_DIFFUSE, Lt0diff);
    glLightfv(GL_LIGHT0, GL_POSITION, Lt0pos);
	glRotatef(angle, 0, 1.0, 0);
	// This GLUT method draws a teapot.  You should replace
	// it with code which draws the object you loaded.
	for (int i = 0; i < vecf.size(); i++) {
		glBegin(GL_TRIANGLES);
		//since it's v1,t1,n1, v2,t2,n2, v3,t3,n3
		// v positions are 0, 3, 6
		// n positions are 2, 5, 8


		glNormal3d(vecn[vecf[i][2] - 1][0], vecn[vecf[i][2] - 1][1], vecn[vecf[i][2] - 1][2]);
		//looking at the 2nd in the list of 9 things, and then each of the values in that vertex separately...
		glVertex3d(vecv[vecf[i][0] - 1][0], vecv[vecf[i][0] - 1][1], vecv[vecf[i][0] - 1][2]);

		glNormal3d(vecn[vecf[i][5] - 1][0], vecn[vecf[i][5] - 1][1], vecn[vecf[i][5] - 1][2]);
		glVertex3d(vecv[vecf[i][3] - 1][0], vecv[vecf[i][3] - 1][1], vecv[vecf[i][3] - 1][2]);


		glNormal3d(vecn[vecf[i][8] - 1][0], vecn[vecf[i][8] - 1][1], vecn[vecf[i][8] - 1][2]);
		glVertex3d(vecv[vecf[i][6] - 1][0], vecv[vecf[i][6] - 1][1], vecv[vecf[i][6] - 1][2]);

		glEnd();
	}
    
    // Dump the image to the screen.
    glutSwapBuffers();
	glutPostRedisplay();
	if (angle == 360.0) {
		angle = 0;
	}
}

// Initialize OpenGL's rendering modes
void initRendering()
{
    glEnable(GL_DEPTH_TEST);   // Depth testing must be turned on
    glEnable(GL_LIGHTING);     // Enable lighting calculations
    glEnable(GL_LIGHT0);       // Turn on light #0.
}

// Called when the window is resized
// w, h - width and height of the window in pixels.
void reshapeFunc(int w, int h)
{
    // Always use the largest square viewport possible
    if (w > h) {
        glViewport((w - h) / 2, 0, h, h);
    } else {
        glViewport(0, (h - w) / 2, w, w);
    }

    // Set up a perspective view, with square aspect ratio
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // 50 degree fov, uniform aspect ratio, near = 1, far = 100
    gluPerspective(50.0, 1.0, 1.0, 100.0);
}

void loadInput(char** target)
{
	// load the OBJ file here
	int counter = 0;
	while (cin.getline(buffer, MAX_BUFFER_SIZE)) { //somehow this triggers even if it's empty. but it should return false and break...?
		stringstream ss(buffer);
		//cout << buffer << endl;
		string s;
		Vector3f latest;
		ss >> s;
		if (s == "v") {
			ss >> latest[0] >> latest[1] >> latest[2];
			vecv.push_back(latest);
		}
		else if (s == "vn") {
			ss >> latest[0] >> latest[1] >> latest[2];
			vecn.push_back(latest);
		}
		else if (s == "f") {
			string entireline;
			while (getline(ss, entireline)) { //take out line by line
				//cout << "LINE LINE LINE " << endl;
				vector<unsigned> target; //declare the final vector.
				//cout << entireline << endl;
				stringstream midline(entireline);
				string partionedlines;
				while(getline(midline, partionedlines, ' ')) { //loop through each kind of partition. in this case it's space
					int latest;
					stringstream singlenum(partionedlines);
					string strnumber;
					while (getline(singlenum, strnumber, '/')) { //now /
						int number = atoi(strnumber.c_str()); //convert this into a number
						target.push_back(number);
					}	
				}
				//you have the whole number here
				//cout << a << " " << b << " " << c << endl;
				//ss >> temp[0] >> temp[1] >> temp[2];
				//vecf.push_back(temp);
				vecf.push_back((target));
			}
		}
	}
	cout << "done." << endl;
}

// Main routine.
// Set up OpenGL, define the callbacks and start the main loop
int main( int argc, char** argv )
{
	cout << "begin loadinput" << endl;
	loadInput(argv);
	cout << "finish loadinput" << endl;
    glutInit(&argc,argv);
    // We're going to animate it, so double buffer 
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );

    // Initial parameters for window position and size
    glutInitWindowPosition( 60, 60 );
    glutInitWindowSize( 360, 360 );
    glutCreateWindow("Assignment 0");

    // Initialize OpenGL parameters.
    initRendering();

    // Set up callback functions for key presses
    glutKeyboardFunc(keyboardFunc); // Handles "normal" ascii symbols
    glutSpecialFunc(specialFunc);   // Handles "special" keyboard keys

     // Set up the callback function for resizing windows
    glutReshapeFunc( reshapeFunc );

    // Call this whenever window needs redrawing
    glutDisplayFunc( drawScene );

    // Start the main loop.  glutMainLoop never returns.
    glutMainLoop( );

    return 0;	// This line is never reached.
}
