#version 120
void main() {
	gl_Position = vec4(sign(gl_Vertex.xy), 0.0, 1.0);
}
