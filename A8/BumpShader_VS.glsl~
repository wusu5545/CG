// Put the names of all group members here 
//Su Wu         21762068
//Yixing Huang  21889201
//Xinyun Li     21884782
#version 330

 
layout(location = 0) in vec4 vPosition;
layout(location = 1) in vec4 vNormal;
layout(location = 2) in vec2 vTex;
layout(location = 3) in vec4 vTangent;

uniform mat4 MVP;  // updated each draw call
uniform mat4 MV;  // updated each draw call 
uniform mat3 NormalMatrix;  // updated each draw call
 
uniform vec4 LightSource;  // updated each draw call


out vec3 V;
out vec3 L;
out vec2 TexCoord;


void main() {

	vec3 p = vec3(MV * vPosition);
	V = normalize(-p);
	vec3 n = normalize(NormalMatrix * vec3(vNormal));
	vec3 t = normalize(NormalMatrix * vec3(vTangent));
	
	vec3 b = cross(n,t);
	mat3 MT = mat3(t,b,n);
	MT = transpose(MT);
	
	V = normalize(MT * V);
	L = normalize(vec3(LightSource) - p);
	L = normalize(MT * L);
	
	TexCoord = vTex;
	gl_Position = MVP * vPosition;

}