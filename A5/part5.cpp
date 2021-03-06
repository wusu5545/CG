// Put the names of all group members here 
//Su Wu         21762068
//Yixing Huang  21889201
//Xinyun Li     21884782

#include "camera.h"
#include "helper.h"
#include <math.h>

//glm stuff
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



using namespace std;

// some global variables:
bool wireframe_mode = false;



/**
 * @brief cameraSystem:
 *  global camera variable
 * use and edit
 */
 cameraSystem cam;
 //last mouse position
 int lastX;
 int lastY;
 bool isMoving;

float t  = 0;  // the time parameter (incremented in the idle-function)
float speed = 0.1;  // rotation speed of the light source in degree/frame



//We need to keep track of matrices ourselves

/**
 * @brief P,V,M:
 * your matrices for setting the scene, no matrix stack anymore
 */
glm::mat4 P,V,M;

/**
 * @brief The ShaderUniforms struct:
 * every shader has its own uniforms,
 * binding of uniforms is done in bindUniforms()
 *
 */
struct ShaderUniforms
{
    GLuint Shader;
    GLint location_MVP;
    GLint location_MV;
    GLint location_NormalMatrix ;
    GLint location_Time;
    GLint location_LightSourceViewSpace;
    GLint location_Color;

    void bindUniforms(glm::mat4& M, glm::mat4& V, glm::mat4& P, glm::vec4& LightSource, glm::vec4& Color,float  t)
    {
        location_Time					= glGetUniformLocation(Shader, "Time");
        location_MVP					= glGetUniformLocation(Shader, "MVP");
        location_MV						= glGetUniformLocation(Shader, "MV");
        location_NormalMatrix			= glGetUniformLocation(Shader, "NormalMatrix");
        location_LightSourceViewSpace	= glGetUniformLocation(Shader, "LightSource");
        location_Color					= glGetUniformLocation(Shader, "Color");


        // TODO create the matrices MV,MVP and NormalMatrix


        glm::mat4 MV			= V*M;
        glm::mat4 MVP			= P*MV;
        glm::mat3 NormalMatrix	= glm::transpose(glm::inverse(glm::mat3(MV)));


        glUniformMatrix4fv(location_MVP, 1, false, glm::value_ptr(MVP));
        glUniformMatrix4fv(location_MV, 1, false, glm::value_ptr(MV));
        glUniformMatrix3fv(location_NormalMatrix, 1, false, glm::value_ptr(NormalMatrix));
        glUniform4fv(location_LightSourceViewSpace, 1, glm::value_ptr(LightSource));
        glUniform4fv(location_Color, 1, glm::value_ptr(Color));
        glUniform1f(location_Time, 10*t);
    }

};
/**
  * @brief SunShader, PhongShader:
  *  encapsulation of shaders
  */
 ShaderUniforms SunShader, PhongShader;
 
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

void displaySun(glm::vec4& LightSource){
	
	glm::vec4 Color (0.9,0.9,0, 1); // set the color to yellow
	glUseProgram(SunShader.Shader);
	SunShader.bindUniforms(M,V,P,LightSource,Color,t);
	glutSolidSphere(25,150,80);
	
}

void displayEarth(glm::vec4& LightSource){
	
	glm::vec4 Color=glm::vec4(0,0,1,1.0);
	M = glm::rotate(glm::mat4(1.0),t+5.0f,glm::vec3(0.0,1.0,0.0));
	M = glm::translate(M,glm::vec3(50.0,0.0,0.0));
	glUseProgram(PhongShader.Shader);
	PhongShader.bindUniforms(M,V,P,LightSource,Color,t);
	drawSphere(12,30,50);
}

void displayMoon(glm::vec4& LightSource){

	glm::vec4 Color =  glm::vec4(0.5,0.5,0.5,1.0);
	M = glm::rotate(M,t*10,glm::vec3(1.0,0.0,0.0));
	M = glm::translate(M,glm::vec3(0.0,20.0,0.0));
	glUseProgram(PhongShader.Shader);
	PhongShader.bindUniforms(M,V,P,LightSource,Color,t);
	drawSphere(6,30,50); 

}

void drawRing(float r) {
  
  glBegin(GL_LINE_LOOP);
  glNormal3f(0,1,0);
  for (int i=0;i<100;i++) {
    float arg = M_PI*(float)i/50.0;
    glVertex3f(r*cos(arg),0,r*sin(arg));
  }
  
  glEnd();
  
}

void displaySaturn(glm::vec4& LightSource){

	glm::vec4 Color = glm::vec4(1.0, 0.55, 0.0, 1.0);
	M = glm::rotate(glm::mat4(1.0),t/2+5.0f,glm::vec3(0.0,1.0,0.0));
	M = glm::translate(M,glm::vec3(100.0,0.0,0.0));
	glUseProgram(PhongShader.Shader);
	PhongShader.bindUniforms(M,V,P,LightSource,Color,t);
	drawSphere(16,30,50);
	//rings
	glm::mat4 M_saturn = M;
	Color = glm::vec4(1,0.84,0,0);
	M = glm::rotate(M_saturn,30.0f,glm::vec3(0,0,1));
	for (float i=0;i<=10;i+=0.5f){
		glUseProgram(PhongShader.Shader);
		if (glm::mod(i,1.5f) == 1.0f)
			Color = glm::vec4(1,0.84,0.6,0);
		if (glm::mod(i,1.5f) == 0.5f)
			Color = glm::vec4(1,0.84,0.3,0);//change color to make rings layering
		if (glm::mod(i,1.5f) == 0)
			Color = glm::vec4(1,0.84,0,0);
		PhongShader.bindUniforms(M,V,P,LightSource,Color,t);
		drawRing(20+i);
	}

}


void initGL() {

    glClearColor(0,0,0,0);   // set the clear color to black
    glEnable(GL_DEPTH_TEST); // turn on the depth test
    glEnable(GL_CULL_FACE);  // turn on backface culling


    // set the camera:
    glm::vec3 eye = glm::vec3( cam.position);
    glm::vec3 center= glm::vec3( cam.position + cam.viewDir);
    glm::vec3 up= glm::vec3( cam.upDir);
    V = glm::lookAt(eye,center,up);



    // enable line smoothing:
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);



    createProgram_VF("sun_VS.glsl",		"sun_FS.glsl",&SunShader.Shader);
    createProgram_VF("phong_VS.glsl",	"phong_FS.glsl",&PhongShader.Shader);


}

void reshape(int w, int h)
{
    glViewport(0,0,(GLsizei) w, (GLsizei) h);

    P = glm::perspective(70.0f, (GLfloat) w/ (GLfloat) h, 10.0f, 400.0f);

}

void onIdle() {

  t+= speed;  // increase the time parameter

  glutPostRedisplay();
}


void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // A single sphere, put in your code for more action

    // create Matrices

    // bind Shader & bind Uniforms

// Create a modelmatrix M for the positioning of  planets
  	M = glm::rotate(50.f,0.f,1.f,0.f);
    glm::vec4 LightSource = V * glm::vec4(0,0,0,1);
  	
  	//draw a yellow sun
	displaySun(LightSource);
	//draw a blue earth
	displayEarth(LightSource);
	// draw the grey earth moon
	displayMoon(LightSource);
	// remember that the transformation of the earth also affects toe moon
	
	//draw saturn with its rings
	displaySaturn(LightSource);

    glutSwapBuffers();
}

void updateV(){
	glm::vec3 eye = glm::vec3( cam.position);
    glm::vec3 center= glm::vec3( cam.position + cam.viewDir);
    glm::vec3 up= glm::vec3( cam.upDir);
    V = glm::lookAt(eye,center,up);
}

/// the mouse handling is done in the next two functions (you additionally need some global variables):

void onMouseDown(int button, int state, int x, int y) {
    //TODO: start movement
    if (state == GLUT_DOWN){
    	lastX = x;
    	lastY = y;
    	isMoving = true;
    }else
    	isMoving = false;
}

void onMouseMove(int x, int y) {
    //TODO: update camera
    float angle = 0.0;
    if (isMoving){
    	if (x!=lastX){
    		if(x>lastX)
    			angle = -1.0;
    		else
    			angle = 1.0;
    		cam.yaw(angle);
    	}
    	if (y!=lastY){
    		if(y>lastY)
    			angle = -1.0;
    		else
    			angle = 1.0;
    		cam.pitch(angle);
    	}
    	updateV();
    	lastX = x;
    	lastY = y;
    }
}

/// keyboard handler:

void keyboard(unsigned char key, int x, int y) {

    //TODO: update camera
  switch(key) {
  	
    case 'p': // toggle polygon mode:	
      wireframe_mode = !wireframe_mode;
      if (wireframe_mode) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      break;
    
    case 'w'://move forward
      cam.moveForward(1.0);
      updateV();
      break;    
    case 's'://move backward
      cam.moveBackward(1.0);
      updateV();
      break;
    case 'a'://roll left
      cam.roll(1.0);
      updateV();
      break;
    case 'd':
      cam.roll(-1.0);
      updateV();
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
    fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
  }

  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutMotionFunc(onMouseMove);
  glutMouseFunc(onMouseDown);
  glutReshapeFunc(reshape);
  glutIdleFunc(onIdle);

  initGL();



  glutMainLoop();
}
