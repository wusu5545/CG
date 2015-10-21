#include "helper.h"  // the shader-handling is implemented here
#include <math.h>

using namespace std;

// some global variables:

bool wireframe_mode = false;
int number_of_slices = 150;
int number_of_stacks = 80;
int shader_mode = 1;  

float t  = 0;  // the time parameter (incremented in the idle-function)
float speed = 0.05;  // initial rotation speed of the animation (can be changed by pressing +/-)

GLuint SunShader; // the shader

void initGL() {
  
  glClearColor(0,0,0,0);   // set the clear color to black
  glEnable(GL_DEPTH_TEST); // turn on the depth test
  glEnable(GL_CULL_FACE);  // turn on backface culling
  
  
  // set the camera:
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(50,50,-50,-50,-50,50,0,1,0);
  
  // create the shaders (the functions are defined in helper.h)
  createProgram_VF("sun_VS.glsl","sun_FS.glsl",&SunShader);
  

}

void reshape(int w, int h)
{
  glViewport(0,0,(GLsizei) w, (GLsizei) h);
  
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(70.0, (GLfloat) w/ (GLfloat) h, 10, 200.0);  
}


// this function is responsible for the animation.
void onIdle() {
  
  t+= speed;  // increase the time parameter
  
  // hand the Time Parameter to the shader:
  GLint location;
  glUseProgram(SunShader);
  location = glGetUniformLocation(SunShader, "Time");
  glUniform1f(location,t);
  
  glutPostRedisplay();
}


void display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  
  
  glUseProgram(SunShader);
  glutSolidSphere(25,number_of_slices,number_of_stacks);
  
  glutSwapBuffers();
}


// the keyboard handler:
// p: toggle wireframe mode
// 1: decrease number of slices
// 2: increase number of slices
// 3: decrease number of stacks
// 4: increase number of stacks
// +/- : increase/decrease the speed of the animation

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
      
      // increase / decrease the speed of the point light P:     
    case '+':
      speed += 0.01;
      break;
    case '-':
      speed -= 0.01;
      break;
  }
  
  
  glutPostRedisplay();	
}

int main(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutInitWindowSize(1042, 1024);
  glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	
  glutCreateWindow("Planet System");
  
   GLenum err = glewInit();  
  if (GLEW_OK != err)
  {
    // cheap or on-chip graphic hardware might not support extensions.
    // you'll get an error here if thats the case. 
    fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
    exit(1);
  }
	
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutReshapeFunc(reshape);
  glutIdleFunc(onIdle); 
	
  initGL();
	
  glutMainLoop();
}
