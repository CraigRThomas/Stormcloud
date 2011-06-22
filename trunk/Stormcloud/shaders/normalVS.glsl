out vec3 norm;
out vec3 lightDir;
out vec2 texCoord;

void main() {
	norm = normalize(gl_NormalMatrix * gl_Normal);
	lightDir = normalize(gl_LightSource[0].position.xyz);
	gl_Position = (gl_ModelViewProjectionMatrix * gl_Vertex);
	texCoord = gl_MultiTexCoord0.xy;
}
