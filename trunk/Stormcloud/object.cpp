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

void Object::draw(Matrix &t){
	std::vector<Mesh*>::iterator i = meshes.begin();
	for (i;i<meshes.end();i++){
		(*i)->draw(t);
	}
}

bool Object::loadFromFile(char *path){
	ilInit();
	ilutInit();
	std::ifstream f(path, std::ios_base::in|std::ios_base::binary);
	std::string line;
	char* line_c;
	unsigned int count = 0;
	Mesh* newMesh;
	int c = 0;

	while(f.good()){
		getline(f,line);
		if (line.length() == 0) break;
		line_c = new char[line.length()];
		line_c = strdup(line.c_str());
		if (strstr(line_c,"begin ")){
			newMesh = new Mesh();
			sscanf(line_c,"%*s %s",newMesh->groupName);
			std::cout<<"Loading: "<<newMesh->groupName<<"\n";
		} else if (strstr(line_c,"start")){
			count = 0;
		} else if (strstr(line_c,"end ")){
			newMesh->allocateBuffers();
			if (!newMesh->vertShader){
				newMesh->vertShader = ShaderMgr.createShader(ShaderType::VERTEX, "shaders/phongVS.glsl");
			}
			if (!newMesh->fragShader){
				newMesh->fragShader = ShaderMgr.createShader(ShaderType::FRAGMENT, "shaders/phongPS.glsl");
			}
			meshes.push_back(newMesh);
		} else if (strstr(line_c,"texpath ")){
			char buf[256];
			sscanf(line_c,"%*s %s",buf);
			newMesh->texId = ilutGLLoadImage(buf);
		} else if (strstr(line_c,"normmappath ")){
			char buf[256];
			sscanf(line_c,"%*s %s",buf);
			newMesh->normMapId = ilutGLLoadImage(buf);
			newMesh->normMapped = true;
			newMesh->TBNs.clear();
			for (unsigned int i=0;i<newMesh->num_faces;i++){
				newMesh->TBNs.push_back(Matrix(3,3));
			}
			newMesh->vertShader = ShaderMgr.createShader(ShaderType::VERTEX, "shaders/normal2VS.glsl");
			newMesh->fragShader = ShaderMgr.createShader(ShaderType::FRAGMENT, "shaders/normal2PS.glsl");
			
			for (unsigned int i=0;i<newMesh->vertShader->attribs.size();i++){
				if (strcmp(newMesh->vertShader->attribs[i].name,"tangent")==0){
					newMesh->vertShader->attribs[i].values = new float[newMesh->num_faces*3];
				} else if (strcmp(newMesh->vertShader->attribs[i].name,"bitangent")==0){
					newMesh->vertShader->attribs[i].values = new float[newMesh->num_faces*3];
				} else if (strcmp(newMesh->vertShader->attribs[i].name,"vNormal")==0){
					newMesh->vertShader->attribs[i].values = new float[newMesh->num_faces*3];
				} 
			}

		} else if (strstr(line_c,"num_verts ")){
		} else if (strstr(line_c,"num_norms ")){
		} else if (strstr(line_c,"num_texcoords ")){
			count = 0;
			sscanf(line_c,"%*s %u",&newMesh->num_verts);
			newMesh->texCoords = new GLfloat[newMesh->num_verts*2];
			newMesh->vertices = new GLfloat[newMesh->num_verts*3];
			newMesh->normals = new GLfloat[newMesh->num_verts*3];
		} else if (strstr(line_c,"num_faces")){
			count = 0;
			sscanf(line_c,"%*s %u",&newMesh->num_faces);
			newMesh->faces = new GLuint[newMesh->num_faces*3];
		} else if (line[0] == 'v'){
			unsigned int size;
			if (line[1] == ' '){
				sscanf(line_c,"%*s %u %f %f %f",&size,&newMesh->vertices[count*3],&newMesh->vertices[count*3+1],&newMesh->vertices[count*3+2]);
				for (unsigned int i=1;i<size;i++){
					newMesh->vertices[(count+i)*3] = newMesh->vertices[count*3];
					newMesh->vertices[(count+i)*3+1] = newMesh->vertices[count*3+1];
					newMesh->vertices[(count+i)*3+2] = newMesh->vertices[count*3+2];
				}
				count += size-1;
			} else if (line[1] == 'n'){
				sscanf(line_c,"%*s %u %f %f %f",&size,&newMesh->normals[count*3],&newMesh->normals[count*3+1],&newMesh->normals[count*3+2]);
				float len = sqrt(newMesh->normals[count*3]*newMesh->normals[count*3] + newMesh->normals[count*3+1]*newMesh->normals[count*3+1] + newMesh->normals[count*3+2]*newMesh->normals[count*3+2]);
				newMesh->normals[count*3] /= len;
				newMesh->normals[count*3+1] /= len;
				newMesh->normals[count*3+2] /= len;
				for (unsigned int i=1;i<size;i++){
					newMesh->normals[(count+i)*3] = newMesh->normals[count*3];
					newMesh->normals[(count+i)*3+1] = newMesh->normals[count*3+1];
					newMesh->normals[(count+i)*3+2] = newMesh->normals[count*3+2];
				}
				count += size-1;
			} else if (line[1] == 't'){
				sscanf(line_c,"%*s %*u %f %f",&newMesh->texCoords[count*2],&newMesh->texCoords[count*2+1]);
			}
			count++;
		} else if (line[0] == 'f'){
			unsigned int size; 
			sscanf(line_c,"%*s %u",&size);

			char *buf = new char[32];
			buf = strtok(line_c," ");
			buf = strtok(NULL," ");
			buf = strtok(NULL," ");

			unsigned int v;
			for (unsigned int i=0;i<size;i++){
				sscanf(buf,"%u",&v);
				if (i<3){
					newMesh->faces[count*3+i] = v;
				} else {
					count++;
					newMesh->faces[count*3]   = newMesh->faces[(count-1)*3];
					newMesh->faces[count*3+1] = newMesh->faces[(count-1)*3+2];
					newMesh->faces[count*3+2] = v;
				}
				buf = strtok(NULL," ");
			}
			count++;
		} else if (strstr(line_c,"mat kd")){
			sscanf(line_c,"%*s %*s %f %f %f",&newMesh->mat.diffuse[0],&newMesh->mat.diffuse[1],&newMesh->mat.diffuse[2]);
		} else if (strstr(line_c,"mat ka")){
			sscanf(line_c,"%*s %*s %f %f %f",&newMesh->mat.ambient[0],&newMesh->mat.ambient[1],&newMesh->mat.ambient[2]);
		} else if (strstr(line_c,"mat ks")){
			sscanf(line_c,"%*s %*s %f %f %f",&newMesh->mat.specular[0],&newMesh->mat.specular[1],&newMesh->mat.specular[2]);
		} else if (strstr(line_c,"mat ns")){
			sscanf(line_c,"%*s %*s %i",&newMesh->mat.shininess);
		}
	}
	f.close();
	delete [] line_c;
	line_c = NULL;
	newMesh = NULL;

	ilShutDown();
	return true;
}