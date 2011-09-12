varying vec3 lightVec, eyeVec;
varying vec2 texCoord;

uniform sampler2D colourMap;
uniform sampler2D normalMap;

void main() {
	float distSqr = dot(lightVec, lightVec);
	float att =  clamp(1.0 - 0.1 * sqrt(distSqr), 0.0, 1.0);
	if (att < 0.0) att *= -1.0;
	vec3 lVec = lightVec * inversesqrt(distSqr);
	
	vec3 vVec = normalize(eyeVec);
	vec4 base = texture2D(colourMap, texCoord);
	vec3 bump = normalize(texture2D(normalMap, texCoord).xyz * 2.0 - 1.0);
	
	vec4 vAmbient = gl_LightSource[0].ambient * gl_FrontMaterial.ambient;
	vec4 colour = vAmbient*base;
	
	float diffuse = max( dot(normalize(lVec), bump),0.0);
	if (diffuse > 0.0){
		vec4 vDiffuse = gl_LightSource[0].diffuse * gl_FrontMaterial.diffuse * diffuse;
		colour += vDiffuse*base;
		
		float spec = 0.0;
		if (gl_FrontMaterial.shininess > 0.0 && gl_FrontMaterial.shininess <= 128.0) {
			spec = pow(clamp(dot(reflect(-lVec,bump),vVec),0.0,1.0),gl_FrontMaterial.shininess);
		}
		vec4 vSpec = gl_LightSource[0].specular * gl_FrontMaterial.specular * spec;
		colour += vSpec;
	}
	
	colour *= att;
	clamp(colour.a,0.0,1.0);
	gl_FragColor = colour;
}
