// Draw an Icosahedron
// ECE4893/8893 Project 4
// Jake Ashmore

#include <iostream>
#include <math.h>
#include <GL/glut.h>
#include <GL/glext.h>
#include <GL/gl.h>
#include <GL/glu.h>

using namespace std;

#define NFACE 20
#define NVERTEX 12

#define X .525731112119133606 
#define Z .850650808352039932


// These are the 12 vertices for the icosahedron
static GLfloat vdata[NVERTEX][3] = {    
   {-X, 0.0, Z}, {X, 0.0, Z}, {-X, 0.0, -Z}, {X, 0.0, -Z},    
   {0.0, Z, X}, {0.0, Z, -X}, {0.0, -Z, X}, {0.0, -Z, -X},    
   {Z, X, 0.0}, {-Z, X, 0.0}, {Z, -X, 0.0}, {-Z, -X, 0.0} 
};

// These are the 20 faces.  Each of the three entries for each 
// vertex gives the 3 vertices that make the face.
static GLint tindices[NFACE][3] = { 
   {0,4,1}, {0,9,4}, {9,5,4}, {4,5,8}, {4,8,1},    
   {8,10,1}, {8,3,10}, {5,3,8}, {5,2,3}, {2,7,3},    
   {7,10,3}, {7,6,10}, {7,11,6}, {11,0,6}, {0,1,6}, 
   {6,1,10}, {9,0,11}, {9,11,2}, {9,2,5}, {7,2,11} };

int testNumber; // Global variable indicating which test number is desired
int depth=1;		// How many depth levels you want
static float updateRate=10.0;

void drawTriangles(GLfloat* v1, GLfloat* v2, GLfloat* v3) {

	glBegin(GL_TRIANGLES);
	
	  glColor3f(0.0,0.0,1.0);		//Draw the triangle faces blue
	
	  glNormal3fv(v1);			//Specifies a pointer to an array of three elements: the x, y, and z coordinates of the new current normal.
	  glVertex3fv(v1);			//Specify x, y, and z coordinates of a vertex
	
	  glNormal3fv(v2);
	  glVertex3fv(v2);
	
	  glNormal3fv(v3);
	  glVertex3fv(v3);
	
	glEnd();
	
	glBegin(GL_LINE_LOOP);
	
	  glColor3f(1.0,1.0,1.0);		// Draw triangle lines as white
	
	  glNormal3fv(v1);
	  glVertex3fv(v1);
	
	  glNormal3fv(v2);
	  glVertex3fv(v2);
	
	  glNormal3fv(v3);
	  glVertex3fv(v3);
	
	glEnd();
			
}

void normalize(GLfloat* v) {
	GLfloat d = sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]);
	
	if (d!=0.0) {
		v[0] /= d;
		v[1] /= d;
		v[2] /= d;
	} else {
		return;
	}
}

void subdivide(GLfloat* v1, GLfloat* v2, GLfloat* v3, int d) {
	//Recursively draw more triangles from the existing ones
	GLfloat v12[3]; GLfloat v23[3]; GLfloat v31[3];
	
	if(d==0){
		drawTriangles(v1, v2, v3);
		return;
	}
	
	for(size_t i = 0; i < 3; i++){
		v12[i] = v1[i] + v2[i];
		v23[i] = v2[i] + v3[i];
		v31[i] = v3[i] + v1[i];
	}
	
	normalize(v12); normalize(v23); normalize(v31);
	
	//Draw triangles
	subdivide(v1, v12, v31, d-1);
	subdivide(v2, v23, v12, d-1);
	subdivide(v3, v31, v23, d-1);
	subdivide(v12, v23, v31, d-1);
}


void add_rotation() {
	//Take the icosahedron and rotate one degree around the x and y axis
	static GLfloat rotX = 0.0;
	static GLfloat rotY = 0.0;
	glRotatef(rotX, 1.0, 0.0, 0.0);		//Causes rotation around X axis
	glRotatef(rotY, 0.0, 1.0, 0.0);		//Causes rotation around Y axis
	rotX += 1.0;
	rotY -= 1.0;
	
	//If we have reached max roation, reset
	if(rotX == 360.0) {rotX = (GLfloat) 0.0;}
	if(rotY == 360.0) {rotY = (GLfloat) 0.0;}
}

// Test cases.  Fill in your code for each test case
void Test1()
{
	//Draw the Icosahedron
	for(size_t i=0; i < NFACE; i++) {
		
		drawTriangles(&vdata[tindices[i][0]][0], &vdata[tindices[i][1]][0], &vdata[tindices[i][2]][0]);
	}
}

void Test2()
{
	//Draw the Icosahedron, add a 10hz update, and rotate 1 degree at each update 
	Test1();
	add_rotation();
}

void Test3()
{
	/*
	 * Start with the Icosahedron and find the center point of each edge of each of the 20 triangles. 
	 * Next create a vector from the 0,0,0 point to the midpoint of each edge. 
	 * These midpoints will NOT be one unit from the enter.
	 * Extend these vectors to be exactly one unit from the origin
	 * Then subdivide the original triangle into four smaller triangles, with the vertices all being one unit from the origin.
	 */
	for(size_t i=0; i < NFACE; i++) {
		subdivide(&vdata[tindices[i][0]][0], &vdata[tindices[i][1]][0], &vdata[tindices[i][2]][0], depth);
	}
}


void Test4()
{
	//Identical to test 3 but adding rotation as in test 2.
	Test3();
	add_rotation();

}

void Test5(int depth)
{
	//Add in a variable level of depth specified by the second command line argument not to exceed depth=5
	for(size_t i=0; i < NFACE; i++) {		
		subdivide(&vdata[tindices[i][0]][0], &vdata[tindices[i][1]][0], &vdata[tindices[i][2]][0], depth);
	}
}

void Test6(int depth)
{
	//Identical to test 5 but adding rotation 
	Test5(depth);
	add_rotation();
}



void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glLineWidth(.5);
	
	if(testNumber==1) Test1();
	if(testNumber==2) Test2();
	if(testNumber==3) Test3();
	if(testNumber==4) Test4();
	if(testNumber==5) Test5(depth);
	if(testNumber==6) Test6(depth);
	else cout << "Invalid Test Number";
	
	glutSwapBuffers();		//for double buffering 
}

void reshape(int w, int h) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glViewport(0, 0, w, h); //(GLsizei)
}

void timer(int) {
	glutPostRedisplay();
	glutTimerFunc(1000.0/updateRate, timer,0);
}

int main(int argc, char** argv)
{
  if (argc < 2)
    {
      std::cout << "Usage: icosahedron testnumber" << endl;
      exit(1);
    }
  
  // Set the global test number
  testNumber = atol(argv[1]);
  
  if (testNumber >= 5) {
	  if (argc < 2) {
		  cout << "Error: Please specify the depth level" << endl;
		  exit(EXIT_FAILURE);
	  }
	  depth = atoi(argv[2]);
	  if(depth > 5) {
		  cout << "Error: Max depth level allowed is 5" << endl;
		  return 0;
	  }
  }
  
  // Initialize glut  and create your window here
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
  glutInitWindowSize(500,500);
  glutInitWindowPosition(500,200);
  glutCreateWindow("Icosahedron");
  
  //initialize values
  glClearColor(0.0, 0.0, 0.0, 0.0); 
  glShadeModel(GL_FLAT);
  glEnable(GL_LINE_SMOOTH);
  glLoadIdentity(); 
  gluLookAt(0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
  
  // Set your glut callbacks here
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutTimerFunc(1000.0/updateRate, timer,0);

  // Enter the glut main loop here
  glutMainLoop();
  return 0;
}

