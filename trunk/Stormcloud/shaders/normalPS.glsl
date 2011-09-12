in vec3 norm;
in vec3 lightDir;
in vec2 texCoord;

uniform sampler2D image;

void main(){
	vec4 col = vec4(0.3,0.3,0.3,1.0);//texture2D(image, texCoord);
	
	float shininess = 1500.f;
	if (gl_FrontMaterial.shininess != 128 && gl_FrontMaterial.shininess != 0){
		shininess = gl_FrontMaterial.shininess;
	}
	
	float diffuse = max(dot(lightDir, norm), 0.0);
	float specular = pow(clamp(dot(reflect(-lightDir, norm), vec3(0.0, 0.0, 1.0)), 0.0, 1.0), shininess);
	
	col.xyz *= gl_LightSource[0].ambient.xyz;
	col.xyz *= gl_LightSource[0].diffuse.xyz * diffuse;
	col.xyz += gl_LightSource[0].specular.xyz * specular;
	
	gl_FragColor = col;
}
