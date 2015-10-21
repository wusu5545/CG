// Put the names of all group members here 
//Su Wu         21762068
//Yixing Huang  21889201
//Xinyun Li     21884782

varying vec3 N; 
varying vec4 P;

uniform vec4 LightSource;  // updated each draw call 
uniform vec4 Color;  // updated each draw call
 
void main() {

	vec3 lightDir[2];
	//TODO add phong lighting
	vec3 D_LightDir = vec3(50,80,-160);
	vec3 cameraPos = vec3(50,80,-160);
	lightDir[0] = -D_LightDir;
	lightDir[1] = vec3(LightSource - P);
	vec4 Intensity = vec4(1,1,1,1.0);
	vec4 diffuse;
	vec4 spec;
	vec4 kdiff = Color;
	vec4 kspec = vec4(1,1,1,1);
	float specPow = 50.0;
	
	vec4 final_color;
	vec3 n = normalize(N);
	vec3 E = normalize(-P.xyz);	
	
	for (int i=0; i<2; ++i){
		vec3 L = normalize(lightDir[i]);
		vec3 R = normalize(reflect(-L,n));
		diffuse = Intensity*kdiff * max(dot(n,L),0.0);
		final_color += diffuse;
		if (i!=0){
			spec = Intensity*kspec * pow(max(dot(R,E),0.0),specPow);
			final_color += spec;
		}
	}
	
    gl_FragColor = final_color;

}