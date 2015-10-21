// Put the names of all group members here 
//Su Wu         21762068
//Yixing Huang  21889201
//Xinyun Li     21884782
//

uniform vec3 D_LightDir;
uniform vec3 P_LightPos;
uniform vec3 cameraPos;

varying vec3 N;
varying vec3 v;
varying vec3 eyeVec;
varying vec3 lightDir[2];
varying vec4 color;

void main() {

	
	N = normalize(gl_Normal);
	v = vec3(gl_Vertex);
	lightDir[0] = -D_LightDir;
	lightDir[1] = P_LightPos - v;
	
	eyeVec = cameraPos - v;
	
	color = gl_Color;
	
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;//ftransform();
	

}