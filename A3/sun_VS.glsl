//Su Wu         21762068
//Yixing Huang  21889201
//Xinyun Li     21884782

uniform float Time; 

varying vec3 N;			//Normal vector
varying vec3 Veye;		//viewing direction
varying float phase;	//current phase

float rand(vec2 n){		//random function
    return fract(sin(dot(n.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main()
{
	
	const float pi = 3.1415926535;
	
	float Amplitude;	//Amplitude of sine wave
	float Phase;		//Phase shift of sine wave
	
	N = normalize(gl_NormalMatrix*gl_Normal);
	vec4 thisPos = gl_Vertex;
	Veye = normalize(gl_ModelViewMatrix*gl_Vertex).xyz;
	vec3 P0 = thisPos.xyz;
	
	Amplitude = 4.0 * rand(gl_Vertex.xz+gl_Normal.yz);
	Phase = 2.0 * pi * rand(gl_Vertex.xy+gl_Vertex.yz);
	
	vec3 P =P0 + sin(Time*pi/100+Phase)*Amplitude*gl_Normal;
	
	phase = sin(Time*pi/100+Phase);
	
	thisPos.xyz = P;
	gl_Position = gl_ModelViewProjectionMatrix * thisPos;
 	
}
