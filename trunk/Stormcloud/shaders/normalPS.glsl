in vec3 norm;
in vec3 lightDir;
in vec2 texCoord;

uniform sampler2D image;
uniform float shininess;

void main(){
	vec4 col = texture2D(image, texCoord);
	
	float diffuse = max(dot(lightDir, norm), 0.0);
	float specular = pow(clamp(dot(reflect(-lightDir, norm), vec3(0.0, 0.0, 1.0)), 0.0, 1.0), shininess);//gl_FrontMaterial.shininess);
	
	col.xyz *= gl_LightSource[0].ambient.xyz;
	col.xyz *= gl_LightSource[0].diffuse.xyz * diffuse;
	col.xyz += gl_LightSource[0].specular.xyz * specular;
	
	gl_FragColor = col;
}
