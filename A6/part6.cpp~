// Put the names of all group members here 
//Su Wu         21762068
//Yixing Huang  21889201
//Xinyun Li     21884782
#include "helper.h"
#include "camera.h"
#include <glm/gtc/type_ptr.hpp>

/// The qt-headers required for the images:
#include <QtOpenGL/QGLWidget>
#include <QtGui/qimage.h>

using namespace std;

// some global variables:

bool wireframe_mode = false;

float t  = 0;  // the time parameter (incremented in the idle-function)
float speed = 0.1;  // rotation speed of the light source in degree/frame
GLuint earthTex,moonTex,saturnTex,backgroundTex; // use for the according textures


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
 * NEW: location_Texture
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
	

	/// part 6 relevant: &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
	GLint location_Texture;  
	/// &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

	void bindUniforms(glm::mat4& M, glm::mat4& V, glm::mat4& P, glm::vec4& LightSource, glm::vec4& Color,  GLuint TexID, float  t)
	{		
		location_Time					= glGetUniformLocation(Shader, "Time");
		location_MVP					= glGetUniformLocation(Shader, "MVP");
		location_MV						= glGetUniformLocation(Shader, "MV");
		location_NormalMatrix			= glGetUniformLocation(Shader, "NormalMatrix");
		location_LightSourceViewSpace	= glGetUniformLocation(Shader, "LightSource");
		location_Color					= glGetUniformLocation(Shader, "Color");

		location_Texture				= glGetUniformLocation(Shader, "DiffuseTexture");
 
		glm::mat4 MV			= V*M;
		glm::mat4 MVP			= P*MV;
		glm::mat3 NormalMatrix	= glm::transpose(glm::inverse(glm::mat3(MV)));


		glUniformMatrix4fv(location_MVP, 1, false, glm::value_ptr(MVP));
		glUniformMatrix4fv(location_MV, 1, false, glm::value_ptr(MV));
		glUniformMatrix3fv(location_NormalMatrix, 1, false, glm::value_ptr(NormalMatrix));
		glUniform4fv(location_LightSourceViewSpace, 1, glm::value_ptr(LightSource));
		glUniform4fv(location_Color, 1, glm::value_ptr(Color));
		
			
		//TODO: Bind texture TexID
		glUniform1i(location_Texture,TexID);
	
	 
		glUniform1f(location_Time, 10*t);
	}

};
/**
  * @brief SunShader, PhongShader:
  *  encapsulation of shaders
  */
ShaderUniforms SunShader, TexturePhongShader; // the shaders

cameraSystem cam;
int lastX;
int lastY;
bool isMoving;



/// TODO: augment the following two geometry-generating functions so that the vertices have 
/// appropriate texture coordinates 

void drawSphere(float r, int slices, int stacks) {

	float dTheta = 2.0*M_PI/(float)stacks;
	float dPhi = M_PI/(float)slices;
	float vx,vy;

	
	//TODO: Provide texture coordinates per vertex
	//glTexCoord2f(..,..); 

	//The Northpole:

	glBegin(GL_TRIANGLE_FAN);	
	//glTexCoord2f(..,..); 
	glTexCoord2f(0.0,1.0);
	
	glNormal3f(0,1,0);
	glVertex3f(0,r,0);
	for (int i = stacks; i>=0; i--) {
		//glTexCoord2f(..,..); 
		vx = 1.0-(float)i * (1.0/(float)(stacks));
		vy = 1.0 - 1.0/(float)(slices);
		glTexCoord2f(vx,vy);
		
		glNormal3f(sin(dPhi)*cos(i*dTheta), cos(dPhi), sin(dPhi)*sin(i*dTheta));
		glVertex3f(r*sin(dPhi)*cos(i*dTheta), r*cos(dPhi), r*sin(dPhi)*sin(i*dTheta));
	}  
	
	glEnd();



    // Middle Part

    for (int j=1; j<slices-1; j++) {
        glBegin(GL_QUAD_STRIP);
        for (int i = stacks; i>=0; i--) {
            //glTexCoord2f(..,..);
            vx = 1.0-(float)i * (1.0/(float)stacks);
	    vy = 1.0-(float)j * (1.0/((float)slices));
	    glTexCoord2f(vx,vy);
	    
            glNormal3f(sin(j*dPhi)*cos(i*dTheta), cos(j*dPhi), sin(j*dPhi)*sin(i*dTheta));
            glVertex3f(r*sin(j*dPhi)*cos(i*dTheta), r*cos(j*dPhi), r*sin(j*dPhi)*sin(i*dTheta));
            //glTexCoord2f(..,..);
	    vx = 1.0-(float)i * (1.0/(float)stacks);
	    vy = 1.0-(float)(j+1) * (1.0/((float)slices));
	    glTexCoord2f(vx,vy);
	    
            glNormal3f(sin((j+1)*dPhi)*cos(i*dTheta), cos((j+1)*dPhi), sin((j+1)*dPhi)*sin(i*dTheta));
            glVertex3f(r*sin((j+1)*dPhi)*cos(i*dTheta), r*cos((j+1)*dPhi), r*sin((j+1)*dPhi)*sin(i*dTheta));
        }
        glEnd();
	}     


	// South Pole:

	glBegin(GL_TRIANGLE_FAN);

	//glTexCoord2f(..,..); 
	glTexCoord2f(0.0,0.0);

	glNormal3f(0,-1,0);
	glVertex3f(0,-r,0);
	for (int i = 0; i<=stacks; i++) {
		//glTexCoord2f(..,..);
		vx = 1.0-(float)i * (1.0/(float)stacks);
		vy = 1.0-(float)(slices-1)*1.0/(float)(slices);
		glTexCoord2f(vx,vy);
		
		glNormal3f(sin((slices-1)*dPhi)*cos(i*dTheta), cos((slices-1)*dPhi), sin((slices-1)*dPhi)*sin(i*dTheta));
		glVertex3f(r*sin((slices-1)*dPhi)*cos(i*dTheta), r*cos((slices-1)*dPhi), r*sin((slices-1)*dPhi)*sin(i*dTheta));
	}  
	glEnd();
  
}

// This function draws a textured quad arround the scene.
// TODO

void DrawQuad(float a) {
    
	//TODO: Provide Texture Coordinates
	
  // -Z
  glBegin(GL_QUADS);
  glTexCoord2f(1.0,1.0);
  glVertex3f(a,a,-a);
  
  glTexCoord2f(0.0,1.0);
  glVertex3f(-a,a,-a);
  
  glTexCoord2f(0.0,0.0);
  glVertex3f(-a,-a,-a);
  
  glTexCoord2f(1.0,0.0);
  glVertex3f(a,-a,-a);
  
  // +Z
  
  glTexCoord2f(0.0,1.0);
  glVertex3f(a,a,a);
  
  glTexCoord2f(0.0,0.0);
  glVertex3f(a,-a,a);
  
  glTexCoord2f(1.0,0.0);
  glVertex3f(-a,-a,a);
  
  glTexCoord2f(1.0,1.0);
  glVertex3f(-a,a,a);
  
  
  // +X     
  glTexCoord2f(1.0,0.0);
  glVertex3f(a,a,-a);
  
  glTexCoord2f(0.0,0.0);
  glVertex3f(a,-a,-a);
  
  glTexCoord2f(0.0,1.0);
  glVertex3f(a,-a,a);
  
  glTexCoord2f(1.0,1.0);
  glVertex3f(a,a,a);
  
  // -X      
  glTexCoord2f(1.0,1.0);
  glVertex3f(-a,a,-a);
  
  glTexCoord2f(0.0,1.0);
  glVertex3f(-a,a,a);
  
  glTexCoord2f(0.0,0.0);
  glVertex3f(-a,-a,a);
  
  glTexCoord2f(1.0,0.0);
  glVertex3f(-a,-a,-a);
  
  // +Y
  glTexCoord2f(0.0,0.0);
  glVertex3f(-a,a,-a);
  
  glTexCoord2f(1.0,0.0);
  glVertex3f(a,a,-a);
  
  glTexCoord2f(1.0,1.0);
  glVertex3f(a,a,a);
  
  glTexCoord2f(0.0,1.0);
  glVertex3f(-a,a,a);
  
  // +Y
  glTexCoord2f(1.0,0.0);
  glVertex3f(-a,-a,-a);
  
  glTexCoord2f(1.0,1.0);
  glVertex3f(-a,-a,a);
  
  glTexCoord2f(0.0,1.0);
  glVertex3f(a,-a,a);
  
  glTexCoord2f(0.0,0.0);
  glVertex3f(a,-a,-a);
  
  glEnd();
  
  
}



/// this funcion does not require texture coordinates
void drawRing(float r) {
  
  glBegin(GL_LINE_LOOP);
  glNormal3f(0,1,0);
  for (int i=0;i<100;i++) {
    float arg = M_PI*(float)i/50.0;
    glVertex3f(r*cos(arg),0,r*sin(arg));
  }
  
  glEnd();
  
}


/// Generating and initializing an openGL texture:

void initTexture(GLint texture_name, GLint w, GLint h, GLubyte *data) {
  
  glBindTexture(GL_TEXTURE_2D, texture_name);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);	
  glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA, w,h,0,GL_RGBA,GL_UNSIGNED_BYTE, data);
}

void initGL() {
  
  glClearColor(0,0,0,0);   // set the clear color to black
  glEnable(GL_DEPTH_TEST); // turn on the depth test
  glEnable(GL_CULL_FACE);  // turn on backface culling
 // set the camera:

    /*glm::vec3 eye = glm::vec3( cam.position);
	glm::vec3 center= glm::vec3( cam.position + cam.viewDir);
	glm::vec3 up= glm::vec3( cam.upDir);
	V = glm::lookAt(eye,center,up);
    */

    glm::vec3 eye(50,80,-160);
    glm::vec3 center(0,0,0);
    glm::vec3 up(0,1,0);
    V = glm::lookAt(eye,center,up);

   // create the shaders (the functions are defined in helper.h)
    createProgram_VF("sun_VS.glsl","sun_FS.glsl",&SunShader.Shader);
	createProgram_VF("Light_and_Tex_VS.glsl","Light_and_Tex_FS.glsl",&TexturePhongShader.Shader);  

  
  /// TODO Texture generation: &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
  
  QImage t_earthTex,t_moonTex,t_saturnTex,t_backgroundTex;
  QImage b_earthTex,b_moonTex,b_saturnTex,b_backgroundTex;
  
  if (b_backgroundTex.load("data/background.jpg"))
  {
     t_backgroundTex = QGLWidget::convertToGLFormat(b_backgroundTex);
     glGenTextures(1,&backgroundTex);
     initTexture(backgroundTex,t_backgroundTex.width(),t_backgroundTex.height(),t_backgroundTex.bits());
  }else
     qWarning("ERROR LOADING IMAGE: background.jpg");
   
  if (b_earthTex.load("data/earth.jpg"))
  {
     t_earthTex = QGLWidget::convertToGLFormat(b_earthTex);
     glGenTextures(1,&earthTex);
     initTexture(earthTex,t_earthTex.width(),t_earthTex.height(),t_earthTex.bits());
  }else
     qWarning("ERROR LOADING IMAGE: earth.jpg");
  
  if(b_moonTex.load("data/moon.jpg"))
  {
     t_moonTex = QGLWidget::convertToGLFormat(b_moonTex);
     glGenTextures(1,&moonTex);
     initTexture(moonTex,t_moonTex.width(),t_moonTex.height(),t_moonTex.bits());
  }else
     qWarning("ERROR LOADING IMAGE: moon.jpg");
  
  if (b_saturnTex.load("data/saturn.jpg"))
  {
     t_saturnTex = QGLWidget::convertToGLFormat(b_saturnTex);
     glGenTextures(1,&saturnTex);
     initTexture(saturnTex,t_saturnTex.width(),t_saturnTex.height(),t_saturnTex.bits());
  }else
     qWarning("ERROR LOADING IMAGE: saturnTex.jpg");
  /// &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
  
  
}

void reshape(int w, int h)
{
  glViewport(0,0,(GLsizei) w, (GLsizei) h);
  
	P = glm::perspective(70.0f, (GLfloat) w/ (GLfloat) h, 10.0f, 1200.0f);  


}

void onIdle() {

  t+= speed;  // increase the time parameter

  glutPostRedisplay();
}

/// TODO: Bind the approproate texture prior to each draw call

void display()
{ 
  
  
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  
 
	glUseProgram(SunShader.Shader);

	// create Matrices
	M = glm::rotate(50.f,0.f,1.f,0.f);  
 
	glm::vec4 LightSource = V * glm::vec4(0,0,0,1)  ;
	glm::vec4 Color (0.9,0.9,0, 1); // set the color to yellow

	// bind Shader & bind Uniforms
	SunShader.bindUniforms(M,V,P, LightSource, Color,0, t);
	 
	glutSolidSphere(25,150,80);


	//Texturing from now on

		
	glUseProgram(TexturePhongShader.Shader);

	//TODO: Set Color for each body 
	
	
	//TODO: Set Texture for each body
	
	
	
	//background	
	
	M = glm::mat4();
	
	glBindTexture(GL_TEXTURE_2D,backgroundTex);
	TexturePhongShader.bindUniforms(M,V,P, LightSource, Color,1,t);
	//TODO: Draw geometry for background
	DrawQuad(300);
	
	// the earth:

	Color = glm::vec4(0.0,0.0,0.9,1); // Set an appropriate color ;)

	M = glm::rotate(t,0.f,1.f,0.f);
	M = glm::translate(M, 50.f,0.f,0.f);
	 
	glBindTexture(GL_TEXTURE_2D,earthTex);
	TexturePhongShader.bindUniforms(M,V,P, LightSource, Color,2,t);
	drawSphere(7, 20,40);

	
	// the moon
 
	M = glm::rotate(M,t*2.f,1.f,0.f,0.f); 	
	M  = glm::translate(M ,0.f,10.f,0.f); 
	glBindTexture(GL_TEXTURE_2D,moonTex);
	TexturePhongShader.bindUniforms(M,V,P, LightSource, Color ,3,t); 

	drawSphere(1, 15,30);

	
	//Saturn


	M = glm::rotate(t/2.0f,0.f,1.f,0.f);
	M = glm::translate(M, 100.f,0.f,0.f);
	  
	glBindTexture(GL_TEXTURE_2D,saturnTex);
	TexturePhongShader.bindUniforms(M,V,P, LightSource,Color, 4 ,t);
	 
	drawSphere(12, 25,50);

	//Rings
	
	glLineWidth(1);

	M = glm::rotate(M, 30.f,0.f,0.f,1.f);

	  
	
	for (int i=0;i<30; i++) {
		if (glm::mod(i,4) == 0)
		    Color = glm::vec4(0.6,0.6,0.6,0);
		if (glm::mod(i,4) == 1)
		    Color = 0.75*glm::vec4(0.6,0.6,0.6,0);
		if (glm::mod(i,4) == 2)
		    Color = 0.5*glm::vec4(0.6,0.6,0.6,0);
		if (glm::mod(i,4) == 3)
		    Color = 0.25*glm::vec4(0.6,0.6,0.6,0);

		TexturePhongShader.bindUniforms(M,V,P, LightSource,Color,5 ,t);
		drawRing(16+i/4.0);
	}





	glUseProgram(0);

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

void cleanUp() {
  
  glDeleteTextures(1,&earthTex);
  glDeleteTextures(1,&moonTex);
  glDeleteTextures(1,&saturnTex);   
  glDeleteTextures(1,&backgroundTex);
  
  
  glDeleteProgram(SunShader.Shader);
  glDeleteProgram(TexturePhongShader.Shader);
  
}


void keyboard(unsigned char key, int x, int y) {
  
  switch(key) {
    
    case 27:
      cleanUp();
      exit(1);
      break;
      
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
  
  //TODO: you may need these 

  glutIdleFunc(onIdle);
	
  initGL();
  
  
  
  glutMainLoop();
}
