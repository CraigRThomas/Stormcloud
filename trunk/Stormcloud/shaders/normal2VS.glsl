attribute vec3 tangent;
attribute vec3 bitangent;
attribute vec3 vNormal;
varying vec3 lightVec, eyeVec;
varying vec2 texCoord;

void main() {
	gl_Position = ftransform();
	texCoord = gl_MultiTexCoord0.xy;
	
	vec3 n = normalize(vNormal);
	vec3 t = normalize(tangent);
	vec3 b = normalize(bitangent);
	
	vec3 vVertex = vec3(gl_ModelViewMatrix * gl_Vertex);
	vec3 tmpVec = vec3(gl_ModelViewMatrix * gl_LightSource[0].position) - vVertex;
	
	mat3 TBN = mat3(t.x,t.y,t.z, b.x,b.y,b.z, n.x,n.y,n.z);
	
	lightVec = TBN * tmpVec;
	
	tmpVec = -vVertex;
	eyeVec = TBN * tmpVec;
	
}
