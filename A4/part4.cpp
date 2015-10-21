// Put the names of all group members here 
//Su Wu         21762068
//Yixing Huang  21889201
//Xinyun Li     21884782

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


        glm::mat4 MV;
        MV = V * M;
        glm::mat4 MVP;
        MVP = P * V * M;//projection * view * model
        glm::mat3 NormalMatrix;
        NormalMatrix = glm::transpose(glm::inverse(glm::mat3(MV)));


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

void initGL() {
 
    glClearColor(0,0,0,0);   // set the clear color to black
    glEnable(GL_DEPTH_TEST); // turn on the depth test
    glEnable(GL_CULL_FACE);  // turn on backface culling


    // set the camera:

    glm::vec3 eye(50,80,-160);
    glm::vec3 center(0,0,0);
    glm::vec3 up(0,1,0);


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

	//TODO  Implement the drawing of the scene here.

	// Note that the matrices and other uniforms have to be updated/bound
    // whenever you issue a new draw command with different values

	// Create a modelmatrix M for the positioning of  planets
  	M = glm::mat4(1.0);
  	glm::vec4 LightSource = glm::vec4(0.0);
  	glm::vec4 Color = glm::vec4(0.0,0.0,0.0,0.0);
  	
  	//draw a yellow sun
	glUseProgram(SunShader.Shader);
	SunShader.bindUniforms(M,V,P,LightSource,Color,t);
	glutSolidSphere(25,150,80);
	//draw a blue earth
	
	Color = glm::vec4(0.0,0.0,1.0,1.0);
	M = glm::rotate(glm::mat4(1.0),t+5.0f,glm::vec3(0.0,1.0,0.0));
	M = glm::translate(M,glm::vec3(50.0,0.0,0.0));
	LightSource = glm::mat4(1.0)*glm::vec4(0,0,0,1.0);
	glUseProgram(PhongShader.Shader);
	PhongShader.bindUniforms(M,V,P,LightSource,Color,t);
	drawSphere(12,30,50);
	// draw the grey earth moon
	
	Color =  glm::vec4(0.5,0.5,0.5,1.0);
	M = glm::rotate(M,t*10,glm::vec3(1.0,0.0,0.0));
	M = glm::translate(M,glm::vec3(0.0,20.0,0.0));
	LightSource = glm::mat4(1.0)*glm::vec4(0,0,0,1.0);
	glUseProgram(PhongShader.Shader);
	PhongShader.bindUniforms(M,V,P,LightSource,Color,t);
	drawSphere(6,30,50); 
	// remember that the transformation of the earth also affects toe moon
	
	//draw saturn with its rings
	
	Color = glm::vec4(1.0, 0.55, 0.0, 1.0);
	M = glm::rotate(glm::mat4(1.0),t/2+5.0f,glm::vec3(0.0,1.0,0.0));
	M = glm::translate(M,glm::vec3(100.0,0.0,0.0));
	LightSource = glm::mat4(1.0)*glm::vec4(0,0,0,1.0);
	glUseProgram(PhongShader.Shader);
	PhongShader.bindUniforms(M,V,P,LightSource,Color,t);
	drawSphere(16,30,50);
	//rings
	glm::mat4 M_saturn = M;
	for( float radius = 20.0f;radius < 31.0f; radius+=1.0f){
		Color = glm::vec4(1,0.84,0,0);
		M = glm::rotate(M_saturn,30.0f,glm::vec3(0,0,1));
		LightSource = glm::mat4(1.0)*glm::vec4(0,0,0,1.0);
		glUseProgram(PhongShader.Shader);
		PhongShader.bindUniforms(M,V,P,LightSource,Color,t);
		
		glBegin(GL_LINE_LOOP);
		glNormal3f(0,1,0);// must do this! or will not be lighting
		for(int i = 0;i<=100; i++)
			glVertex3f(sin((i*2*M_PI)/100)*radius,0,cos((i*2*M_PI)/100)*radius);
		glEnd();
	}
    // Hint: before you draw geometry make sure the shader is bound, as well as the uniforms



	glutSwapBuffers();
}


// the keyboard handler:

void keyboard(unsigned char key, int x, int y) {
  
  switch(key) {
    
    case 'p': // toggle polygon mode:	
      wireframe_mode = !wireframe_mode;
      if (wireframe_mode) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
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
  glutReshapeFunc(reshape);
  glutIdleFunc(onIdle);
	
  initGL();
  
  
  
  glutMainLoop();
}
