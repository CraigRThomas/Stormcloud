attribute vec3 in_position;

void main(void){
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
