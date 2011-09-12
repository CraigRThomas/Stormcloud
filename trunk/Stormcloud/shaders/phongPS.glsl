varying vec3 normal, lightDir, eyeVec;
varying vec2 texCoord;

uniform sampler2D image;

void main() {
	vec4 tex = texture2D(image, texCoord);
	vec4 colour = (gl_LightSource[0].ambient * gl_FrontMaterial.ambient * tex);
	
	vec3 N = normalize(normal);
	vec3 L = normalize(lightDir);
	
	float lambertTerm = dot(N,L);
	
	if (lambertTerm > 0.0){
		colour += gl_LightSource[0].diffuse * gl_FrontMaterial.diffuse * tex * lambertTerm;
		
		vec3 E = normalize(eyeVec);
		vec3 R = reflect(-L, N);
		
		float spec = 0.0;
		if (gl_FrontMaterial.shininess > 0.0 && gl_FrontMaterial.shininess <= 128) {
			spec = pow (max (dot (R, E), 0.0), gl_FrontMaterial.shininess);
		}
		colour += gl_LightSource[0].specular * tex * spec; 
	}
	
	gl_FragColor = colour;
}
