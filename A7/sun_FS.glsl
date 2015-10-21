// Put the names of all group members here 
//Su Wu         21762068
//Yixing Huang  21889201
//Xinyun Li     21884782

varying float height;
varying vec3 v_normal;
varying vec3 v_viewingVec;

void main() {

 	/**
		HINT: You can use your own sun-shaders from previous tasks!
	*/
	float Cyellow = 0.4;
	
	float alph = 1.0 - max(0.0,dot(v_normal,-v_viewingVec));
	float Cbase= height;

	vec4 Cfinal = vec4(1.0,Cbase+alph*Cyellow,0,0);
	
	gl_FragColor = Cfinal;
}
