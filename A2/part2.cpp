#include "helper.h"  // the shader-handling is implemented here
#include <math.h>


using namespace std;

// some global variables:

bool wireframe_mode = false;
int number_of_slices = 30;
int number_of_stacks = 50;
int shader_mode = 1;  

float D_light_direction[3]; // Direction of the directional Light Source D
float P_light_position[3];  // Position of the point Light P

float t  = 0;  // the time parameter (incremented in the idle-function)
float speed = 0.01;  // rotation speed of the light source in degree/frame

GLuint GouraudShader , PhongShader; // the shaders

// this function creates a shpere including normals
void drawSphere(float r, int slices, int stacks) {
  
  float dTheta = 2.0*M_PI/(float)stacks;
  float dPhi = M_PI/(float)slices;  
  
  //The Northpole:
  glBegin(GL_TRIANGLE_FAN);
  glNormal3f(0,1,0);
  glVertex3f(0,r,0);
  for (int i = stacks; i>=0; i--) {
    glNormal3f(sin(dPhi)*cos(i*dTheta), cos(dPhi), sin(dPhi)*sin(i*dTheta));
    glVertex3f(r*sin(dPhi)*cos(i*dTheta), r*cos(dPhi), r*sin(dPhi)*sin(i*dTheta));
  }  
  glEnd();
  
  // Middle Part
  
  for (int j=1; j<slices-1; j++) {
    glBegin(GL_QUAD_STRIP);
    for (int i = stacks; i>=0; i--) {
      
      glNormal3f(sin(j*dPhi)*cos(i*dTheta), cos(j*dPhi), sin(j*dPhi)*sin(i*dTheta));
      glVertex3f(r*sin(j*dPhi)*cos(i*dTheta), r*cos(j*dPhi), r*sin(j*dPhi)*sin(i*dTheta));
      
      glNormal3f(sin((j+1)*dPhi)*cos(i*dTheta), cos((j+1)*dPhi), sin((j+1)*dPhi)*sin(i*dTheta));
      glVertex3f(r*sin((j+1)*dPhi)*cos(i*dTheta), r*cos((j+1)*dPhi), r*sin((j+1)*dPhi)*sin(i*dTheta));
    }  
    glEnd();
  }     
  // South Pole:
  glBegin(GL_TRIANGLE_FAN);
  glNormal3f(0,-1,0);
  glVertex3f(0,-r,0);
  for (int i = 0; i<=stacks; i++) {
    glNormal3f(sin((slices-1)*dPhi)*cos(i*dTheta), cos((slices-1)*dPhi), sin((slices-1)*dPhi)*sin(i*dTheta));
    glVertex3f(r*sin((slices-1)*dPhi)*cos(i*dTheta), r*cos((slices-1)*dPhi), r*sin((slices-1)*dPhi)*sin(i*dTheta));
  }  
  glEnd();
  
}


void initGL() {
  
  glClearColor(0,0,0,0);   // set the clear color to black
  glEnable(GL_DEPTH_TEST); // turn on the depth test
  glEnable(GL_CULL_FACE);  // turn on backface culling
  
  // Direction of the directional light source D: 
  D_light_direction[0] = 0;
  D_light_direction[1] = 0;
  D_light_direction[2] = 1;
  
  // Inital poition of the point light P:
  P_light_position[0] = 45;
  P_light_position[1] = 0;
  P_light_position[2] = 0;
  
  
  // set the camera:
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(50,50,-50,-50,-50,50,0,1,0);
  
  // create the shaders (the functions are defined in helper.h)
  createProgram_V("Gouraud.glsl",&GouraudShader);
  createProgram_VF("Phong_VS.glsl","Phong_FS.glsl",&PhongShader);
  
  
  // the values which are constant (direction of light D and the camera position)
  // have to be handed to the respective uniform shader variables only once, that's
  // why this code is in the init function:
  
  GLint location;
  glUseProgram(GouraudShader);
  location = glGetUniformLocation(GouraudShader, "D_LightDir");
  glUniform3fv(location,1,D_light_direction);
  location = glGetUniformLocation(GouraudShader, "cameraPos");
  glUniform3f(location, 50.0,50.0,-50.0);
  
  glUseProgram(PhongShader);
  location = glGetUniformLocation(PhongShader, "D_LightDir");
  glUniform3fv(location,1,D_light_direction);
  location = glGetUniformLocation(PhongShader, "cameraPos");
  glUniform3f(location, 50.0,50.0,-50.0);
  
}

void reshape(int w, int h)
{
  glViewport(0,0,(GLsizei) w, (GLsizei) h);
  
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(70.0, (GLfloat) w/ (GLfloat) h, 10, 200.0);  
}


// this function is responsible for the animation. See ducumentations of glutIdleFunc 
// for further details.

void onIdle() {
  
  t+= speed;  // increase the time parameter
  
  // update the x and z position of the Point light P (y remains constant):
  P_light_position[0] = 45.0*cos(t*M_PI/180.0);
  P_light_position[2] = 45.0*sin(t*M_PI/180.0);
  
  // hand the position of P to the shaders:
  glUseProgram(GouraudShader);
  GLint location;
  location = glGetUniformLocation(GouraudShader, "P_LightPos");
  glUniform3fv(location,1,P_light_position);
  
  glUseProgram(PhongShader);
  location = glGetUniformLocation(PhongShader, "P_LightPos");
  glUniform3fv(location,1,P_light_position);  
  
  glutPostRedisplay();
}


void display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  
  glColor3f(0.9,0.9,0); // set the color to yellow
  
  if (shader_mode == 1)  	    glUseProgram(0);   // use fixed function pipeline (No lighting)    
  else if (shader_mode == 2)  glUseProgram(GouraudShader); // use the Gouraud Shader
  else if (shader_mode == 3)  glUseProgram(PhongShader); // use the Phong shader
  else cout << "unknown shader mode\n";
      
  drawSphere(25,number_of_slices,number_of_stacks);
    
    
    // draw a point at the current point light position:
  glUseProgram(0); // use fixed function pipeline to draw the 
  glPointSize(5);
  glBegin(GL_POINTS);
  glVertex3fv(P_light_position);
  glEnd();
  
  glutSwapBuffers();
}


// the keyboard handler:
// p: toggle wireframe mode
// 1: decrease number of slices
// 2: increase number of slices
// 3: decrease number of stacks
// 4: increase number of stacks
// +/- : increase/decrease the speed of the light source

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

void Specialkeyboard(int key, int x, int y)
{
  
  switch(key) {
    
    case GLUT_KEY_F1:
      shader_mode = 1;
      cout << "Using no shader\n";
      break;     
    case GLUT_KEY_F2:
      shader_mode=2;
      cout << "using Gouraud shader\n";
      break;    
    case GLUT_KEY_F3:
      shader_mode=3;
      cout << "using Phong Shader\n";
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
    // cheap on on-chip graphic hardware might not support extensions.
    // you'll get an error here if thats the case. 
    fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
    exit(1);
  }
	
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutSpecialFunc(Specialkeyboard);
  glutReshapeFunc(reshape);
  glutIdleFunc(onIdle); 
	
  initGL();
	
  glutMainLoop();
}
