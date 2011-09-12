in vec3 normal, lightDir;
in vec2 texCoord;

uniform sampler2D image;
uniform sampler2D normMap;

void main() {
	vec4 tex = texture2D(image, texCoord);
	vec4 level = texture2D(normMap, texCoord);
	
	vec3 vNormal = 2.0f * (level.rgb - 0.5f);
	
	vec3 L = normalize(lightDir);
	
	gl_FragColor.rgb = gl_LightSource[0].diffuse.rgb * tex.rgb * clamp(dot(L,vNormal),0.0,1.0);
}