// Put the names of all group members here 
//Su Wu         21762068
//Yixing Huang  21889201
//Xinyun Li     21884782
uniform mat4 MVP;  // updated each draw call
uniform mat4 MV;  // updated each draw call 
uniform mat3 NormalMatrix;  // updated each draw call
 
uniform vec4 LightSource;  // updated each draw call
 


varying vec3 N;
varying vec4 P;
varying vec2 TexCoord;


void main() { 

	// TODO:
	// reuse your phong vs code
	// interpolate tex coordinates over the surface using the provided varying

	P = (MV * gl_Vertex);
	N = normalize(NormalMatrix*gl_Normal);
	TexCoord = gl_MultiTexCoord0;
	gl_Position = MVP * gl_Vertex;

}