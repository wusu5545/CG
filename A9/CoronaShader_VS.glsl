#version 330

 
layout(location = 0) in vec4 vPosition;
layout(location = 1) in vec4 vNormal;
layout(location = 2) in vec4 vTex;

layout(location = 4) in vec4 vColor; // vertex based Color (no uniform!)

uniform mat4 MVP;  // updated each draw call
 
out vec4 Color;



void main() 
{
	gl_Position =   MVP * vPosition;

	Color = vColor;
}