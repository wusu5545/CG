//Su Wu         21762068
//Yixing Huang  21889201
//Xinyun Li     21884782

varying vec3 N;
varying vec3 Veye;
varying float phase;

void main() {

	float Cyellow = 0.4;
	
	float alph = 1.0 - max(0.0,dot(N,-Veye));
	float Cbase= 0.5*(phase+1.0);

	vec4 Cfinal = vec4(1,Cbase+alph*Cyellow,0,0);
	
	gl_FragColor = Cfinal;
}