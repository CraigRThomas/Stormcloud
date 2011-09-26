uniform sampler2D image;
uniform vec4 matAmbient;
uniform vec4 matDiffuse;
uniform vec4 matSpecular;
uniform int matShininess;
uniform vec4 lightAmbient;
uniform vec4 lightDiffuse;
uniform vec4 lightSpecular;

varying vec3 normal, lightDir, eyeVec;
varying vec2 texCoord;

void main() {
	vec4 tex = texture2D(image, texCoord);
	vec4 colour = lightAmbient * matAmbient * tex;
	
	vec3 N = normalize(normal);
	vec3 L = normalize(lightDir);
	
	float lambertTerm = -dot(N,L);
	
	if (lambertTerm > 0.0){
		colour += lightDiffuse * matDiffuse *tex* lambertTerm;
		
		vec3 E = normalize(eyeVec);
		vec3 R = reflect(-L, N);
		
		float spec = 0.0;
		if (matShininess > 0.0 && matShininess <= 128.0) {
			spec = pow (max (dot (R, E), 0.0), matShininess);
			colour += lightSpecular * matSpecular * tex * spec; 
		}
		
	} 
	
	gl_FragColor = colour;
}
