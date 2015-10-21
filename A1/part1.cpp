// Put the names of all group members here 
//Su Wu         21762068
//Yixing Huang  21889201
//Xinyun Li     21884782
// all function which require editing are marked TODO


#include <GL/glut.h>
#include <math.h>
#include <iostream>


using namespace std;

// some global variables and their initial values:
bool wireframe_mode = false;
int number_of_slices = 20;
int number_of_stacks = 30;


//!TODO 
void drawSphere(float r, int slices, int stacks) {
//  create vertices and faces for a sphere using the
// openGL immediate mode   
    GLfloat dalpha=2*M_PI/stacks;//X_Y plane
    GLfloat dtheta=M_PI/slices;//in Z direction
    GLfloat theta=0,x,y,z;
//    glutSolidSphere((GLdouble)r,(GLint)slices,(GLint)stacks);

    glBegin(GL_TRIANGLE_FAN);//the area arround the poles
    glVertex3f(0,r,0);
    theta=1*dtheta;
    for (int j=1;j<=stacks+1;j++){
        x=r*sin(theta)*cos(j*dalpha);
        y=r*cos(theta);
        z=r*sin(theta)*sin(j*dalpha);
        glVertex3f(x,y,z);
    }
    glEnd();

    glBegin(GL_QUAD_STRIP);//quad strips
    for (int i=1;i<=slices-2;i++){
        theta=i*dtheta;
        for(int j=1;j<=stacks+1;j++){
            x=r*sin(theta)*cos(j*dalpha);
            y=r*cos(theta);
            z=r*sin(theta)*sin(j*dalpha);
            glVertex3f(x,y,z);
            x=r*sin(theta+dtheta)*cos(j*dalpha);
            y=r*cos(theta+dtheta);
            z=r*sin(theta+dtheta)*sin(j*dalpha);
            glVertex3f(x,y,z);
        }
    }
    glEnd();

    glBegin(GL_TRIANGLE_FAN);//the area arround the poles
    glVertex3f(0,-r,0);
    theta=(slices-1)*dtheta;
    for (int j=1;j<=stacks+1;j++){
        x=r*sin(theta)*cos(j*dalpha);
        y=r*cos(theta);
        z=r*sin(theta)*sin(j*dalpha);
        glVertex3f(x,y,z);
    }
    glEnd();

}

//void drawCube(GLdouble a){
// draw Cube edge length is a
    //glBegin(GL_QUAD_STRIP);//cube
    //glVertex3f();
    //glEnd();
//}


//!TODO
void initGL() {
 
// In this function, you should implement the initial
// setting of the modelview matrix 
// This is also the place for initializing other opengl
// state variables (e.g the clear color...)
    //glMatrixMode(GL_PROJECTION);
    //glLoadIdentity();
    //glFrustum(-1,1,-1,1,5,100);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(50,50,50,-50,-50,-50,-1,2,-1);

}
//!TODO
void reshape(int w, int h)
{
 // set the viewport and the projection matrix.here
    glViewport(0,0,(GLsizei) w,(GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float aspect=(float)w/(float)h;
    gluPerspective(70.0f,(GLfloat) aspect,0.1f,1000.0f);
}

//________________________________________________
// no need to edit below this line, but feel free
// to do so and to try different things.

void display()
{
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  
glColor3f(0.9,0.9,0); // set the color to yellow
//glTranslatef(0.0, 1.0, 0.0);
//glRotatef(90,0,0,1);
//glClear(GL_COLOR_BUFFER_BIT);
//glBegin(GL_TRIANGLES);
//glColor3f(0,0,1);
//glVertex3f(-1,-1,-2);
//glVertex3f( 1,-1,-2);
//glVertex3f( 0, 1,-2);
//glEnd();

drawSphere(25,number_of_slices,number_of_stacks);
glFlush();
}


// the keyboard handler:
// p: toggle wireframe mode
// 1: decrease number of slices
// 2: increase number of slices
// 3: decrease number of stacks
// 4: increase number of stacks

void keyboard(unsigned char key, int x, int y) {
  
  switch(key) {
    
    case 'p': // toggle polygon mode:	
      wireframe_mode = !wireframe_mode;
      if (wireframe_mode) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      break;
    
    case '1': // decrease number of slices:
      if (number_of_slices > 2) {
	number_of_slices--;
	cout << "slices: " << number_of_slices << endl;
      }
      else cout << "slices already minimum!\n";
      break;
      
    case '2': // increase number of slices:
      number_of_slices++;
      cout << "slices: " << number_of_slices << endl;
      break;
    
    case '3': // decrease number of stacks:
      if (number_of_stacks > 3) {
	number_of_stacks--;
	cout << "stacks: " << number_of_stacks << endl;
      }
      else cout << "stacks already at minimum!\n";
      break;
    
    case '4': // increase number of stacks:
      number_of_stacks++;
      cout << "stacks: " << number_of_stacks << endl;
      break;
  }
  
  glutPostRedisplay();	
}


int main(int argc, char **argv)
{
  
  glutInit(&argc, argv);
  glutInitWindowSize(1042, 1024);
  glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH);
	
  glutCreateWindow("Planet System");
	
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutReshapeFunc(reshape);
	
  initGL();
	
  glutMainLoop();
}
