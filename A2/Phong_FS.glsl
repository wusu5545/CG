// Put the names of all group members here 
//Su Wu         21762068
//Yixing Huang  21889201
//Xinyun Li     21884782
//

varying vec3 N;
varying vec3 v;
varying vec3 eyeVec;
varying vec3 lightDir[2];
varying vec4 color;

void main() {

	vec4 diffuse;
	vec4 spec;

	vec4 LightSource =vec4(1,1,1,1);
	vec4 kdiff = color;//vec4(0.5,0.5,0.5,1);
	
	vec4 kspec = vec4(1,1,1,1);
	float specPow = 50.0;	
	
	vec3 n = normalize(N);

	vec4 final_color; //ambient = 0;
		
	vec3 E = normalize(eyeVec);
	
	
	for (int i=0; i<2; ++i){
		vec3 L = normalize(lightDir[i]);
		vec3 R = normalize(reflect(-L,n));
		diffuse = LightSource* kdiff * max(dot(n,L),0.0);
		final_color += diffuse;
		if (i!=0){
			spec = LightSource* kspec * pow(max(dot(R,E),0.0),specPow);
			final_color += spec;
		}
	}
	
	gl_FragColor = final_color;

}