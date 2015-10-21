// Put the names of all group members here 
//Su Wu         21762068
//Yixing Huang  21889201
//Xinyun Li     21884782
#version 330

 
layout(location = 0) in vec4 vPosition;
layout(location = 1) in vec4 vNormal;
layout(location = 2) in vec2 vTex;

uniform mat4 MVP;  // updated each draw call
uniform mat4 MV;  // updated each draw call 
uniform mat3 NormalMatrix;  // updated each draw call
 
uniform vec4 LightSource;  // updated each draw call
 


out vec3 N;
out vec4 P;
out vec2 TexCoord;


void main() { 

	// TODO:
	// reuse your phong vs code
	// interpolate tex coordinates over the surface using the provided varying
	P = (MV * vPosition);
	N = normalize(NormalMatrix * vec3(vNormal));
	TexCoord = vTex;
	gl_Position = MVP * vPosition;

}