// Put the names of all group members here 
//Su Wu         21762068
//Yixing Huang  21889201
//Xinyun Li     21884782
//


uniform vec3 D_LightDir;
uniform vec3 P_LightPos;
uniform vec3 cameraPos;

vec3 N;
vec3 Vcam;

#define NUM_LIGHTS 2

vec3 lightDir[NUM_LIGHTS];

void main() {
	
	vec4 diffuse;
	vec4 spec;

	N = normalize(gl_Normal);//
	Vcam = normalize(cameraPos - gl_Vertex.xyz);
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;//ftransform();

	vec4 LightSource =vec4(1,1,1,1);
	vec4 kdiff = gl_Color;
	
	vec4 kspec = vec4(1,1,1,1);
	float specPow = 50.0;	
	
	lightDir[0] = -D_LightDir;//gl_Vertex.xyz;
	lightDir[1] = P_LightPos - gl_Vertex.xyz; 

	vec4 final_color; //ambient = 0;
		
	vec3 E = Vcam;
	
	
	for (int i=0; i<NUM_LIGHTS; ++i){
		vec3 L = normalize(lightDir[i]);
		vec3 R = normalize(reflect(-L,N));
		diffuse = LightSource* kdiff * max(dot(N,L),0.0);
		final_color += diffuse;
		if (i!=0){
			spec = LightSource* kspec * pow(max(dot(R,E),0.0),specPow);
			final_color += spec;
		}
	}
	
	gl_FrontColor = final_color;

}