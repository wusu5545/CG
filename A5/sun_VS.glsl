// Put the names of all group members here 
//Su Wu         21762068
//Yixing Huang  21889201
//Xinyun Li     21884782

uniform float Time;  // updated each frame by the application

uniform mat4 MVP;  // updated each draw call
uniform mat4 MV;  // updated each draw call
uniform mat3 NormalMatrix;  // updated each draw call

varying float height; // handed to the fragment shader
varying vec3 v_normal;
varying vec3 v_viewingVec;

float rand(vec2 n){		//random function
    return fract(sin(dot(n.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main()
{
	/**
		HINT: You can use your own sun-shaders from previous tasks!
	*/
	const float pi = 3.1415926535;

	float Amplitude;	//Amplitude of sine wave
	float Phase;		//Phase shift of sine wave
	
	v_normal = normalize(NormalMatrix*gl_Normal);
	vec4 thisPos = gl_Vertex;
	v_viewingVec = normalize(MV*gl_Vertex).xyz;//normalize(MV*vec4(vec3(50,80,-160)-gl_Vertex.xyz,0.0)).xyz;
	vec3 P0 = thisPos.xyz;
	
	Amplitude = 5.0 * rand(gl_Vertex.xz+gl_Normal.yz);
	Phase = 2.0 * pi * rand(gl_Vertex.xy+gl_Vertex.yz);
	
	vec3 P =P0 + sin(Time*pi/180+Phase)*Amplitude*gl_Normal;
	
	height = 0.5*(sin(Time*pi/180+Phase)+1.0);
	
	thisPos.xyz = P;
 
    gl_Position = MVP * thisPos;
 
}
