#include "object.h"

Object::Object(){

}

Object::~Object(){

}

void Object::update(const float &dt){
	std::vector<Mesh*>::iterator i = meshes.begin();
	for (i;i<meshes.end();i++){
		(*i)->update(dt);
	}
}

void Object::draw(){
	std::vector<Mesh*>::iterator i = meshes.begin();
	for (i;i<meshes.end();i++){
		(*i)->draw();
	}
}

bool Object::loadFromFile(char *path){
	std::ifstream f(path, std::ios_base::in|std::ios_base::binary);

	std::string line;
	unsigned int count = 0, vOff = 0;
	Mesh* temp = 0;
	while(f.good()){
		getline(f,line);
		if (strstr(line.c_str(),"begin ")){
			temp = new Mesh();
			sscanf(line.c_str(),"%*s %s",temp->groupName);
			std::cout<<"Loading: "<<temp->groupName<<"\n";
		} else if (strstr(line.c_str(),"start")){
			count = 0;
		} else if (strstr(line.c_str(),"end ")){
			temp->allocateBuffers();
			if (!temp->vertShader){
				temp->vertShader = ShaderMgr.createShader(ShaderType::VERTEX, "shaders/phongVS.glsl");
			}
			if (!temp->fragShader){
				temp->fragShader = ShaderMgr.createShader(ShaderType::FRAGMENT, "shaders/phongPS.glsl");
			}
			meshes.push_back(temp);
		} else if (strstr(line.c_str(),"texpath ")){
			char buf[256];
			sscanf(line.c_str(),"%*s %s",buf);
			temp->texId = ilutGLLoadImage(buf);
		} else if (strstr(line.c_str(),"num_verts ")){
			count = 0;
		} else if (strstr(line.c_str(),"num_norms ")){
			count = 0;
		} else if (strstr(line.c_str(),"num_texcoords ")){
			count = 0;
			sscanf(line.c_str(),"%*s %u",&temp->num_verts);
			temp->texCoords = new GLfloat[temp->num_verts*2];
			temp->vertices = new GLfloat[temp->num_verts*3];
			temp->normals = new GLfloat[temp->num_verts*3];
		} else if (strstr(line.c_str(),"num_faces")){
			count = 0;
			sscanf(line.c_str(),"%*s %u",&temp->num_faces);
			temp->faces = new GLuint[temp->num_faces*3];
		} else if (line[0] == 'v'){
			unsigned int size;
			if (line[1] == ' '){
				sscanf(line.c_str(),"%*s %u %f %f %f",&size,&temp->vertices[count*3],&temp->vertices[count*3+1],&temp->vertices[count*3+2]);
				for (unsigned int i=1;i<size;i++){
					temp->vertices[(count+i)*3] = temp->vertices[count*3];
					temp->vertices[(count+i)*3+1] = temp->vertices[count*3+1];
					temp->vertices[(count+i)*3+2] = temp->vertices[count*3+2];
				}
				count += size-1;
			} else if (line[1] == 'n'){
				sscanf(line.c_str(),"%*s %u %f %f %f",&size,&temp->normals[count*3],&temp->normals[count*3+1],&temp->normals[count*3+2]);
				float len = sqrt(temp->normals[count*3]*temp->normals[count*3] + temp->normals[count*3+1]*temp->normals[count*3+1] + temp->normals[count*3+2]*temp->normals[count*3+2]);
				temp->normals[count*3] /= len;
				temp->normals[count*3+1] /= len;
				temp->normals[count*3+2] /= len;
				for (unsigned int i=1;i<size;i++){
					temp->normals[(count+i)*3] = temp->normals[count*3];
					temp->normals[(count+i)*3+1] = temp->normals[count*3+1];
					temp->normals[(count+i)*3+2] = temp->normals[count*3+2];
				}
				count += size-1;
			} else if (line[1] == 't'){
				sscanf(line.c_str(),"%*s %*u %f %f",&temp->texCoords[count*2],&temp->texCoords[count*2+1]);
			}
			count++;
		} else if (line[0] == 'f'){
			unsigned int size; 
			sscanf(line.c_str(),"%*s %u",&size);

			char* lineCopy = strdup(line.c_str());
			char *buf = new char[32];
			buf = strtok(lineCopy," ");
			buf = strtok(NULL," ");
			buf = strtok(NULL," ");

			unsigned int v;
			for (unsigned int i=0;i<size;i++){
				sscanf(buf,"%u",&v);
				if (i<3){
					temp->faces[count*3+i] = v - vOff;
				} else {
					count++;
					temp->faces[count*3]   = temp->faces[(count-1)*3];
					temp->faces[count*3+1] = temp->faces[(count-1)*3+2];
					temp->faces[count*3+2] = v - vOff;
				}
				buf = strtok(NULL," ");
			}
			count++;
			free(lineCopy);
		} else if (strstr(line.c_str(),"mat kd")){
			sscanf(line.c_str(),"%*s %*s %f %f %f",&temp->mat.diffuse[0],&temp->mat.diffuse[1],&temp->mat.diffuse[2]);
		} else if (strstr(line.c_str(),"mat ka")){
			sscanf(line.c_str(),"%*s %*s %f %f %f",&temp->mat.ambient[0],&temp->mat.ambient[1],&temp->mat.ambient[2]);
		} else if (strstr(line.c_str(),"mat ks")){
			sscanf(line.c_str(),"%*s %*s %f %f %f",&temp->mat.specular[0],&temp->mat.specular[1],&temp->mat.specular[2]);
		} else if (strstr(line.c_str(),"mat ns")){
			sscanf(line.c_str(),"%*s %*s %i",&temp->mat.shininess);
		}
	}
	
	temp = 0;
	free(temp);
	f.close();

	return true;
}