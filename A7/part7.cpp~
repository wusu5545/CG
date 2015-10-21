// Put the names of all group members here 
//Su Wu         21762068
//Yixing Huang  21889201
//Xinyun Li     21884782
/// This skeleton only contains a lit sphere. We recommend to use your stuff from the
/// the previous assignmnet as skeleton.

#include "camera.h"
#include "offLoader.h"

#include <QtOpenGL/QGLWidget>
#include <QtGui/qimage.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


using namespace std;


GLuint earthTex,moonTex,saturnTex,backgroundTex; 


float t  =0;  // the time parameter (incremented in the idle-function)
float speed = 0.1;  // rotation speed of the light source in degree/frame
bool wireframe_mode = false; 

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
	

	GLint location_Texture;  

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
		glUniform1i(location_Texture,TexID);
		
		glBindTexture(GL_TEXTURE_2D,TexID);

		glUniform1f(location_Time, 10*t);
	}

};

/**
 * @brief The ShaderUniforms struct:
 * every shader has its own uniforms,
 * binding of uniforms is done in bindUniforms()
 *
 */
ShaderUniforms SunShader, TexturePhongShader; // the shaders

cameraSystem cam;   // the camera
int lastX;
int lastY;
bool isMoving;
/**
 * @brief OffObject
 *  Object for loading/parsing the shuttle file
 *
 */
OffObject *objA;    // the shuttle
 

/**
 * @brief The GeometryData struct:
 * store the VertexArrayObject and number of vertices and indices
 */
struct GeometryData
{
	GLuint vao;
	unsigned int numVertices;
	unsigned int numIndices; 
};

GeometryData geometryShuttle, geometryCube, geometrySphere, geometryRings ;

/**
 * @brief The Vertex struct:
 * store vertices with according normals
 * and texture coordinates
 */
struct Vertex {

	Vertex(glm::vec3 p, glm::vec3 n )
	{
		position[0] = p.x;
		position[1] = p.y;
		position[2] = p.z;
		position[3] = 1;

		normal[0] = n.x;
		normal[1] = n.y;
		normal[2] = n.z;			 
		normal[3] = 1;
			 
	}; 
	Vertex(glm::vec3 p, glm::vec3 n, glm::vec2 t)	
	{
		position[0] = p.x;
		position[1] = p.y;
		position[2] = p.z;
		position[3] = 1;

		normal[0] = n.x;
		normal[1] = n.y;
		normal[2] = n.z;			 
		normal[3] = 1;

		texcoord[0] = t.x;
		texcoord[1] = t.y;
	};

	GLfloat position[4];
	GLfloat normal[4];
	GLfloat texcoord[2];
};

void bindVertexArrayObjects(GeometryData& geometry, const std::vector<Vertex> &vertexdata, const std::vector<unsigned short> &indices) 
{
	
	//*bind to GL
	glGenVertexArrays(1, &(geometry.vao));
    glBindVertexArray(geometry.vao);
	
	geometry.numVertices = vertexdata.size();
	geometry.numIndices = indices.size();

	// Create and bind a BO for vertex data
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, geometry.numVertices * sizeof(Vertex), vertexdata.data(), GL_STATIC_DRAW);
	

	// set up vertex attributes
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texcoord));

	// Create and bind a BO for index data
	GLuint ibo;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

	// copy data into the buffer object
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, geometry.numIndices * sizeof(unsigned short), indices.data(), GL_STATIC_DRAW);


	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

 void createRings() 
{ 
	glm::vec2 t;
	glm::vec3 n(0,1,0);    
	glm::vec3 p ;
	
	unsigned short index = 0;
	std::vector<Vertex> vertexdata ;
	std::vector<unsigned short> indices;
	

	 
	float r= 1;
	for (int i=0;i<100;i++)
	{
		float arg = M_PI*(float)i/50.0;
		p =  glm::vec3(r*cos(arg),0,r*sin(arg));	 
		vertexdata.push_back(Vertex (p,n,t));	
		indices.push_back(index++);  
	}
	 
	bindVertexArrayObjects(geometryRings,vertexdata,indices);
		 
}


/// TODO
 void createSphere()
{
	float r=1;
	int slices = 25;
	int stacks = 50;
	
	float dTheta = 2.0*M_PI/(float)stacks;
	float dPhi = M_PI/(float)slices;  
	
	std::vector<Vertex> vertexdata ;
	std::vector<unsigned short> indices;

	// ***********************************************************************************
	// TODO: modify your drawing of the sphere
	// take care not to create duplicate vertices
	// having an equal amount of vertices and indices defeats the purpose of index buffers

	// because we want to have a single buffer, we will use TRIANGLES, e.g. each quad consists of 2 triangles
	
	// we suggest
	// 1. create top most vertex
	glm::vec2 t(0.5,1.0);
	glm::vec3 n(0,1,0);
	glm::vec3 p(0,r,0);
	vertexdata.push_back(Vertex(p,n,t));
	
	// 2a. create first ring/slice: add 1 Vertex per stack
	for (int i = stacks; i>=0; i--){
		glm::vec2 t(1.0-i*1.0/float(stacks),1.0-1.0/float(slices));
		glm::vec3 n(sin(dPhi)*cos(i*dTheta), cos(dPhi), sin(dPhi)*sin(i*dTheta));
		glm::vec3 p(r*sin(dPhi)*cos(i*dTheta), r*cos(dPhi), r*sin(dPhi)*sin(i*dTheta));
		vertexdata.push_back(Vertex(p,n,t));
	}
	// 2b. create 1 triangle per stack !NO VERTEX DUPLICATES!, close the ring with the first vertex you created for the stack
	for (int triangleID = 0; triangleID<stacks; triangleID++){
		indices.push_back(0);
		indices.push_back(triangleID);
		indices.push_back(triangleID+1);
	}
	indices.push_back(0);
	indices.push_back(stacks);
	indices.push_back(1);
	/// -> you should be able to see the hat of the sphere (maybe use wire frame mode to contol)
	
	// 3.  create middle part
	//	v0---v2
	//	|   /|
	//	|  / |
	//	| /  |
	//	v1---v3
	// 3a. create the remaing rings/slices: add 1 Vertex per stack
	for (int j=1; j<slices-1; j++){
		for (int i = stacks; i>=0; i--){
			t = glm::vec2(1.0-i*1.0/float(stacks),1.0-(j+1)/float(slices));
			n = glm::vec3(sin((j+1)*dPhi)*cos(i*dTheta), cos((j+1)*dPhi), sin((j+1)*dPhi)*sin(i*dTheta));
			p = glm::vec3(r*sin((j+1)*dPhi)*cos(i*dTheta), r*cos((j+1)*dPhi), r*sin((j+1)*dPhi)*sin(i*dTheta));
			vertexdata.push_back(Vertex(p,n,t));
	// 3b. create 2 triangles for every new vertex you add
			indices.push_back(vertexdata.size() - stacks - 2);//v0
			indices.push_back(vertexdata.size() - 1);//v1
			indices.push_back(vertexdata.size() - stacks - 1);//v2
			
			indices.push_back(vertexdata.size() - stacks - 1);//v2
			indices.push_back(vertexdata.size() - 1);//v1
			indices.push_back(vertexdata.size());//v3
		}
	}
	// 4. create bottmom most vertex
	// 4a. no additional vertices necessary
	// 4b. create 1.triangle per stack using the existing vertices of the lastly created slice/ring
	t = glm::vec2 (0.5,0);
	n = glm::vec3 (0,-1,0);
	p = glm::vec3 (0,-r,0);
	vertexdata.push_back(Vertex(p,n,t));
	int lastVertex = vertexdata.size() - 1;
	for (int triangleID = 0; triangleID <= stacks; triangleID++){
		indices.push_back(lastVertex);
		indices.push_back(vertexdata.size() - triangleID - 1);
		indices.push_back(vertexdata.size() - triangleID - 2);
	}
	// Note: Vertex positions/normals/texcoords are identical to your existing code
	// wireframe rendering can be helpful

	// ***********************************************************************************
 
	//glm::vec2 t(0.5,1.0);
	//glm::vec3 n(0,1,0);    
	//glm::vec3 p(0,r,0);
	//vertexdata.push_back(Vertex (p,n,t));
 
	bindVertexArrayObjects(geometrySphere,vertexdata,indices);

}

/// creates a cube with length a
 void createCube(float a)
{	
	std::vector<Vertex> vertexdata ;
	std::vector<unsigned short> indices;

	glm::vec2 t;
	glm::vec3 n;
	glm::vec3 p;
	 
	unsigned short index = 0;
	t = glm::vec2(0,0);
	p = glm::vec3(a,a,-a);
	vertexdata.push_back(Vertex(p,n,t));
	indices.push_back(index++);
	t =glm::vec2(0,1);
	p =glm::vec3(-a,a,-a);
	vertexdata.push_back(Vertex(p,n,t));
	indices.push_back(index++);
	t =glm::vec2(1,1);
	p =glm::vec3(-a,-a,-a);
	vertexdata.push_back(Vertex(p,n,t));
	indices.push_back(index++);
	t =glm::vec2(1,0);
	p =glm::vec3(a,-a,-a);
	vertexdata.push_back(Vertex(p,n,t));
	indices.push_back(index++);

	// +Z
    
	t =glm::vec2(0,0);
	p =glm::vec3(a,a,a);
	vertexdata.push_back(Vertex(p,n,t));
	indices.push_back(index++);
	t =glm::vec2(0,1);
	p =glm::vec3(a,-a,a);
	vertexdata.push_back(Vertex(p,n,t));
	indices.push_back(index++);
	t =glm::vec2(1,1);
	p =glm::vec3(-a,-a,a);
	vertexdata.push_back(Vertex(p,n,t));
	indices.push_back(index++);
	t =glm::vec2(1,0);
	p =glm::vec3(-a,a,a);
	vertexdata.push_back(Vertex(p,n,t));
	indices.push_back(index++);
      
      
	// +X     
	t =glm::vec2(0,0);
	p =glm::vec3(a,a,-a);
	vertexdata.push_back(Vertex(p,n,t));
	indices.push_back(index++);
	t =glm::vec2(0,1);
	p =glm::vec3(a,-a,-a);
	vertexdata.push_back(Vertex(p,n,t));
	indices.push_back(index++);
	t =glm::vec2(1,1);
	p =glm::vec3(a,-a,a);
	vertexdata.push_back(Vertex(p,n,t));
	indices.push_back(index++);
	t =glm::vec2(1,0);
	p =glm::vec3(a,a,a);
	vertexdata.push_back(Vertex(p,n,t));
	indices.push_back(index++);
           
	// -X      
	t =glm::vec2(0,0);
	p =glm::vec3(-a,a,-a);
	vertexdata.push_back(Vertex(p,n,t));
	indices.push_back(index++);
	t =glm::vec2(0,1);
	p =glm::vec3(-a,a,a);
	vertexdata.push_back(Vertex(p,n,t));
	indices.push_back(index++);
	t =glm::vec2(1,1);
	p =glm::vec3(-a,-a,a);
	vertexdata.push_back(Vertex(p,n,t));
	indices.push_back(index++);
	t =glm::vec2(1,0);
	p =glm::vec3(-a,-a,-a);
	vertexdata.push_back(Vertex(p,n,t));
	indices.push_back(index++);
      
	// +Y
	t =glm::vec2(0,0);
	p =glm::vec3(-a,a,-a);
	vertexdata.push_back(Vertex(p,n,t));
	indices.push_back(index++);
	t =glm::vec2(0,1);
	p =glm::vec3(a,a,-a);
	vertexdata.push_back(Vertex(p,n,t));
	indices.push_back(index++);
	t =glm::vec2(1,1);
	p =glm::vec3(a,a,a);
	vertexdata.push_back(Vertex(p,n,t));
	indices.push_back(index++);
	t =glm::vec2(1,0);
	p =glm::vec3(-a,a,a);
	vertexdata.push_back(Vertex(p,n,t));
	indices.push_back(index++);
      
	// +Y
	t = glm::vec2(0,0);
	p = glm::vec3(-a,-a,-a);
	vertexdata.push_back(Vertex(p,n,t));
	indices.push_back(index++);
	t = glm::vec2(0,1);
	p = glm::vec3(-a,-a,a);
	vertexdata.push_back(Vertex(p,n,t));
	indices.push_back(index++);
	t = glm::vec2(1,1);
	p = glm::vec3(a,-a,a);
	vertexdata.push_back(Vertex(p,n,t));
	indices.push_back(index++);
	t = glm::vec2(1,0);
	p = glm::vec3(a,-a,-a);
	vertexdata.push_back(Vertex(p,n,t));
	indices.push_back(index++);
      

	 bindVertexArrayObjects(geometryCube,vertexdata,indices);


}
  
/// TODO
/// loads shuttle data from off-file and fills according GeometryData
 void createShuttle()
{
	objA = new OffObject("data/shuttle.off");
   
   
	std::vector<Vertex> vertexdata ;        // store the vertices of the shuttle here
	std::vector<unsigned short> indices;    // store the according indices here


	geometryShuttle.numVertices = objA->noOfVertices;
	geometryShuttle.numIndices = objA->noOfFaces*3;


	// TODO: Fill vertexdata
	glm::vec2 t;
	glm::vec3 n;
	glm::vec3 p;
	for (unsigned int i=0; i< geometryShuttle.numVertices; i++) 
	{	 
		p = objA->vertexList[i];
		n = objA->normalsList[i];
		//printf("%f, %f, %f \n",n.x,n.y,n.z);
		vertexdata.push_back(Vertex(p,n,t));
	}

	// TODO: Fill indexData

	for (int i=0; i< objA->noOfFaces; i++) 
	{ 
		indices.push_back(objA->faceList[i].A);
		indices.push_back(objA->faceList[i].B);
		indices.push_back(objA->faceList[i].C);
	}

	bindVertexArrayObjects(geometryShuttle,vertexdata,indices);

}

void initTexture(GLint name, GLint w, GLint h, GLubyte *data) {
	  
	glBindTexture(GL_TEXTURE_2D, name);
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
   
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
   
  
	// set the camera:

	glm::vec3 eye = glm::vec3( cam.position);
	glm::vec3 center= glm::vec3( cam.position + cam.viewDir);
	glm::vec3 up= glm::vec3( cam.upDir);
	V = glm::lookAt(eye,center,up);


	// create the shaders (the functions are defined in helper.h)

	
    // create the shaders (the functions are defined in helper.h)
	createProgram_VF("sun_VS.glsl","sun_FS.glsl",&SunShader.Shader);
	createProgram_VF("Light_and_Tex_VS.glsl","Light_and_Tex_FS.glsl",&TexturePhongShader.Shader);

  
// ***********************************************************************************
// You can add texture creation from previous assignment
  
	
	//QImage earth, moon, saturn, back;
    // Use the code from your previous solutions!
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
// ***********************************************************************************
   
   
   // the space shuttle & other geometry:
	createShuttle();
	createSphere();
	createRings();
	createCube(300);
	printf("Init done!");
}

void reshape(int w, int h)
{
	glViewport(0,0,(GLsizei) w, (GLsizei) h);
 
  	P = glm::perspective(70.0f, (GLfloat) w/ (GLfloat) h, 10.0f, 800.0f);  

}


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
	

	glUseProgram(TexturePhongShader.Shader);

  
	// ***********************************************************************************
	// Add rest of solar system  from previous assignments

	//// TODO: Use perviously created geometry
	// use vertex array objects instead of immediate mode
	//--------------------background:GL_QUADS------------------------
	M = glm::mat4();
	TexturePhongShader.bindUniforms(M,V,P, LightSource, Color,backgroundTex,t);
	glBindVertexArray(geometryCube.vao);
	glDrawElements(GL_QUADS, geometryCube.numIndices, GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);
	// TODO: redraw the same sphere geometry, modify the planets size via its model matrix
	//--------------------earth:GL_TRIANGLES--------------------------
	M = glm::rotate(t,0.f,1.f,0.f);
	M = glm::translate(M,50.f,0.f,0.f);
	glm::mat4 M_earth = M;
	//M = glm::rotate(M,12*t,0.f,-1.f,0.f);
	M = glm::scale(M,7.f,7.f,7.f);
	Color = glm::vec4(0,0,0.8,0);
	TexturePhongShader.bindUniforms(M,V,P, LightSource, Color,earthTex,t);
	glBindVertexArray(geometrySphere.vao);
	glDrawElements(GL_TRIANGLES, geometrySphere.numIndices, GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);
	//-------------------moon:GL_TRIANGLES----------------------------
	M = glm::rotate(M_earth,t*2.f,1.f,0.f,0.f); 	
	M = glm::translate(M ,0.f,10.f,0.f); 
	M = glm::scale(M,1.f,1.f,1.f);
	TexturePhongShader.bindUniforms(M,V,P, LightSource, Color,moonTex,t);
	glBindVertexArray(geometrySphere.vao);
	glDrawElements(GL_TRIANGLES, geometrySphere.numIndices, GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);
	//------------------saturn:GL_TRIANGLES---------------------------
	M = glm::rotate(t/2.0f,0.f,1.f,0.f);
	M = glm::translate(M, 100.f,0.f,0.f);
	glm::mat4 M_saturn = M;
	M = glm::scale(M,12.f,12.f,12.f);
	TexturePhongShader.bindUniforms(M,V,P, LightSource, Color,saturnTex,t);
	glBindVertexArray(geometrySphere.vao);
	glDrawElements(GL_TRIANGLES, geometrySphere.numIndices, GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);
	// TODO: @saturn rings; redraw the same ring, modify the rings diameter via its model matrix
	//------------------rings:GL_LINE_LOOP----------------------------
	glLineWidth(1);
	M = glm::rotate(M_saturn, 30.f,0.f,0.f,1.f);
	glm::mat4 M_ring = M;
	for (int i=0;i<30; i++) {
		if (glm::mod(i,4) == 0)
		    Color = glm::vec4(0.8039f,0.5216f,0.2471f,0);
		if (glm::mod(i,4) == 1)
		    Color = glm::vec4(0.8235f,0.7059f,0.5490f,0);
		if (glm::mod(i,4) == 2)
		    Color = glm::vec4(0.8706f,0.7216f,0.5294f,0);
		if (glm::mod(i,4) == 3)
		    Color = glm::vec4(0.9608f,0.8706f,0.7020f,0);
		float r = 16+i/4.0;
		M = glm::scale(M_ring,r,r,r);
		TexturePhongShader.bindUniforms(M,V,P, LightSource,Color,5,t);
		glBindVertexArray(geometryRings.vao);
		glDrawElements(GL_LINE_LOOP, geometryRings.numIndices, GL_UNSIGNED_SHORT, 0);
		glBindVertexArray(0);
	}
	// ***********************************************************************************
	//	glBindVertexArray(/*vao*/);
	//	glDrawElements(/*PRIMITIVE TYPE*/, /*NUMBER OF ELEMENTS*/, /*FORMAT of INDICES*/, (void*)0);	
	//  glBindVertexArray(0);

	// ***********************************************************************************
	// TODO: Add space shuttle
	// Bonus: Compute elliptical orbit
	//-----------------shuttle:GL_TRIANGLES---------------------------
	M = glm::mat4();
	float a = 100.0;
	float b = 50.0;
	float xtranslate = a * cos(t/2*M_PI/180.);
	float ytranslate = b * sin(t/2*M_PI/180.);
	//M = glm::rotate(-t/2.0f,0.f,0.f,1.f);//cycle
	//M = glm::translate(M,0.f,100.f,0.f);
	//M = glm::rotate(M,180.f,0.f,1.f,0.f);
	
	//ellipsoidal
	//M = glm::rotate(t/2.0f,0.f,0.f,1.f);
	M = glm::translate(M,-xtranslate,ytranslate,0.f);
	M = glm::rotate(M,-t/2,0.f,0.f,1.f);
	M = glm::rotate(M,90.f,0.f,0.f,1.f);
	M = glm::rotate(M,180.f,0.f,1.f,0.f);
	Color = glm::vec4(0.5f,0.5f,0.5f,0);
	TexturePhongShader.bindUniforms(M,V,P, LightSource, Color,6,t);
	glBindVertexArray(geometryShuttle.vao);
	glDrawElements(GL_TRIANGLES, geometryShuttle.numIndices, GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);
	// ***********************************************************************************
	glUseProgram(0);
	glutSwapBuffers();

}

void cleanUp() {
 
    glDeleteTextures(1,&earthTex);
    glDeleteTextures(1,&moonTex);
    glDeleteTextures(1,&saturnTex);
    glDeleteTextures(1,&backgroundTex);

    glDeleteProgram(SunShader.Shader);
    glDeleteProgram(TexturePhongShader.Shader);

    glDeleteVertexArrays(1, &geometryShuttle.vao);
    glDeleteVertexArrays(1, &geometryCube.vao);
    glDeleteVertexArrays(1, &geometrySphere.vao);
 
}

void onIdle() {

  t+= speed;  // increase the time parameter

  glutPostRedisplay();
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
 glutIdleFunc(onIdle);
 initGL();
 glutMainLoop();
}
