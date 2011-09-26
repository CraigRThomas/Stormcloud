uniform mat4 transform;
uniform vec4 lightPos;

attribute vec3 vPosition;
attribute vec3 vNormal;
attribute vec2 vTexCoord;

varying vec3 normal, lightDir, eyeVec;
varying vec2 texCoord;

void main() {
	texCoord = vTexCoord;
	
	vec4 pos = (transform * gl_ModelViewMatrix) * gl_Vertex;
	normal = gl_NormalMatrix * vNormal;
	lightDir = vec3(gl_ModelViewMatrix * lightPos) - vec3(gl_Vertex);
	eyeVec = -vec3(pos);
	
	gl_Position = pos;
}
