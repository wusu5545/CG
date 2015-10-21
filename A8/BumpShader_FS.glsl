// Put the names of all group members here 
//Su Wu         21762068
//Yixing Huang  21889201
//Xinyun Li     21884782
uniform vec4 Color;  // updated each draw call
uniform sampler2D Texture;
uniform sampler2D Texture_Night;
uniform sampler2D Texture_Normal;
 

in vec3 V;
in vec3 L; 
in vec2 TexCoord;


uniform vec4 LightSource;  // updated each draw call
 
void main() {
      vec4 Color_day = texture2D(Texture,TexCoord);
      vec4 Color_night = texture2D(Texture_Night,TexCoord);
      vec3 N = texture2D(Texture_Normal,TexCoord).rgb * 2.0 - 1.0;
      
      //L = normalize(L);
      //V = normalize(V);
      float Day_or_Night = max(0.0,dot(N,L));
      vec4 Surface_Color = Color_day * Day_or_Night + Color_night * (1.0 - Day_or_Night);
      
      vec4 Intensity = vec4(1.0,1.0,1.0,1.0);
      vec4 ambient = 0.50 * Surface_Color;
      vec4 final_color = ambient;
      
      vec4 kdiff = Surface_Color;
      vec4 kspec = vec4(1.0,1.0,1.0,1.0);
      float specPow = 50.0;
      
      vec4 diffuse = Intensity * kdiff ;//* max(dot(n,L),0.0);
      final_color = ambient + diffuse;
      vec3 R = normalize(reflect(-L,N));
      vec4 spec = Intensity * kspec * pow(max(dot(R,V),0.0),specPow);
      final_color += spec;
      
      gl_FragColor = final_color;

}