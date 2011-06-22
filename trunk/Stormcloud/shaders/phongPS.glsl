in vec3 normal, lightDir, eyeVec;
in vec2 texCoord;

uniform sampler2D image;

void main() {
	vec4 tex = texture2D(image, texCoord);
	vec4 colour = (gl_FrontLightModelProduct.sceneColor * tex) + (gl_LightSource[0].ambient * tex);
	
	vec3 N = normalize(normal);
	vec3 L = normalize(lightDir);
	
	float lambertTerm = dot(N,L);
	
	if (lambertTerm > 0.0){
		colour += gl_LightSource[0].diffuse * tex * lambertTerm;
		
		vec3 E = normalize(eyeVec);
		vec3 R = reflect(-L, N);
		float spec = pow (max (dot (R, E), 0.0), gl_FrontMaterial.shininess);
		colour += gl_LightSource[0].specular * tex * spec; 
		//colour += gl_LightSource[0].specular * gl_FrontMaterial.specular * spec; 
	}
	
	gl_FragColor = colour;
}
