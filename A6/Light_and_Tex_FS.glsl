// Put the names of all group members here 
//Su Wu         21762068
//Yixing Huang  21889201
//Xinyun Li     21884782
uniform vec4 Color;  // updated each draw call
uniform sampler2D Texture;
uniform int DiffuseTexture;

varying vec3 N; 
varying vec4 P;
varying vec2 TexCoord;

uniform vec4 LightSource;  // updated each draw call
 

void main() {

	// TODO: Implement phong shading
	// textures affect diffuse lighting only
	
	// only water should produce specular hightlights
	vec3 lightDir[2];
	//TODO add phong lighting
	vec3 D_LightDir = vec3(50,80,-160);
	lightDir[0] = -D_LightDir;
	lightDir[1] = vec3(LightSource - P);
	vec4 Intensity = vec4(1,1,1,1.0);
	vec4 diffuse;
	vec4 spec;
	vec4 Surface_Color = texture2D(Texture,TexCoord);
	if (DiffuseTexture == 5)
		Surface_Color = Color;
	vec4 ambient = 0.0 * Surface_Color;
	vec4 kdiff = Surface_Color;//Color;
	vec4 kspec = vec4(1,1,1,1);
	float specPow = 50.0;
	
	vec4 final_color=ambient;
	vec3 n = normalize(N);
	vec3 E = normalize(-P.xyz);	
	
	
//  	vec3 L = normalize(lightDir[1]);
//  	vec3 R = normalize(reflect(-L,n));
//  	diffuse = Intensity*kdiff * max(dot(n,L),0.0);
//  	final_color = ambient+diffuse;
//  	spec = Intensity*kspec * pow(max(dot(R,E),0.0),specPow);
//  	final_color += spec;
 	for (int i=0; i<2; ++i){
 		vec3 L = normalize(lightDir[i]);
 		vec3 R = normalize(reflect(-L,n));
 		diffuse = Intensity*kdiff * max(dot(n,L),0.0);
 		final_color += diffuse;
 		//apply the specular term only to reflective surfaces, e.g. water
 		if ((i!=0)&&((Surface_Color.b<1)&&(Surface_Color.b>0)&&(Surface_Color.r+Surface_Color.g<1))){
 			spec = Intensity*kspec * pow(max(dot(R,E),0.0),specPow);
 			final_color += spec;
		}
 	}
 	if ((DiffuseTexture == 1))//(gl_FrontFacing==1)&&
		gl_FragColor = Surface_Color;
	else 
		gl_FragColor = final_color;//texture2D(Texture,TexCoord);

}
