varying vec3 normal, lightDir, eyeVec;
varying vec2 texCoord;

void main() {
	texCoord = gl_MultiTexCoord0.xy;
	
	normal = gl_NormalMatrix * gl_Normal;
	vec3 vVert = vec3(gl_ModelViewMatrix * gl_Vertex);
	lightDir = vec3(gl_LightSource[0].position.xyz - vVert);
	eyeVec = -vVert;
	
	gl_Position = ftransform();
}
