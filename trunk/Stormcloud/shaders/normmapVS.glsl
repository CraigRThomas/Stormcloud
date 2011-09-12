attribute vec3 tangent;
attribute vec3 bitangent;
attribute vec3 vNormal;
out vec3 normal, lightDir;
out vec2 texCoord;

void main() {
	texCoord = gl_MultiTexCoord0.xy;
	
	vec3 vVert = vec3(gl_ModelViewMatrix * gl_Vertex);
	lightDir = vec3(gl_LightSource[0].position.xyz - gl_Vertex);
	mat3 TBN = mat3(tangent.x,-tangent.y,tangent.z, -bitangent.x,bitangent.y,-bitangent.z, normal.x,-normal.y,normal.z);
	lightDir = TBN * lightDir;
	
	gl_Position = ftransform();
}
