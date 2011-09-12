#include "skeleton.h"

Skeleton::Skeleton(void){
	numJoints = 0;
	rotation = 0;
	hasRun = false;
	hasUpdated = false;
}

Skeleton::~Skeleton(void){

}

void Skeleton::load(char *filePath, char* meshName){
	char path[256];
	strcpy(path, filePath);
	strcat(path, ".joints");
	std::ifstream f(path, std::ios_base::in|std::ios_base::binary);
	std::string line;
	char* line_c;
	numJoints = 0;
	rotation = 0;

	while(f.good()){
		getline(f,line);
		line_c = new char[line.length()];
		line_c = strdup(line.c_str());
		char* c = new char[256];
		strcpy(c, "begin ");
		c = strcat(c,meshName);
		if (strncmp(line_c,c,line.length())==0 && line.length() > 0){
			getline(f,line);
			line_c = new char[line.length()];
			line_c = strdup(line.c_str());
			sscanf(line_c,"%s %f %f %f",root.name, &root.pos.x, &root.pos.y, &root.pos.z);
			root.index = 0;
			root.parent = 0;

			Joint* cur = &root;
			Joint j;
			numJoints = 1;
			while(f.good()){
				getline(f,line);
				if (strstr(line_c,"end")) break;
				if (line.length()>0){
					line_c = new char[line.length()];
					line_c = strdup(line.c_str());
					if (strstr(line_c,"}")){
						cur = cur->parent;
					} else {
						sscanf(line_c,"%s %f %f %f",j.name,&j.pos.x,&j.pos.y,&j.pos.z);
						j.parent = cur;
						j.index = numJoints;
						j.rotation = Quaternion(1,0,0,0);
						numJoints++;
						cur->children.push_back(j);
						cur = &cur->children.back();
					}
				}
			}
			cur = 0;
			free(cur);
		}
	}
	f.close();

	strcpy(path,filePath);
	strcat(path,".weights");
	std::ifstream wf(path,std::ios_base::in|std::ios_base::binary);

	char* buf = new char[128];
	int count = 0;
	getline(wf,line);
	line_c = new char[line.length()];
	line_c = strdup(line.c_str());
	buf = strtok(line_c,":");
	while(buf){
		count++;
		buf = strtok(NULL,":");
	}
	
	while(wf.good()){
		getline(wf,line);
		line_c = new char[line.length()];
		line_c = strdup(line.c_str());
		if (strstr(line_c,":")){
			weights.push_back(new float[count]);
			buf = strtok(line_c,":");
			buf = strtok(NULL,":");
			for (int i=0;i<count;i++){
				sscanf(buf,"%f",&weights.back()[i]);
				buf = strtok(NULL,":");
				if (buf==NULL) break;
			}
		}
	}
	wf.close();

	Quaternion q(1,0,0,0);
	for (unsigned int i=0;i<weights.size();i++){
		jointTransforms.push_back(q);
		vertexTransforms.push_back(q);
	}
}

Quaternion Skeleton::transformByIndex(const unsigned int &index){
	if (vertexTransforms.size() >= index){
		return vertexTransforms[index];
	} else {
		return Quaternion(1,0,0,0);
	}
}

void Skeleton::updateTransforms(Quaternion q, Joint *j){
	if (j->index == 1){
		q *= normalize(Quaternion(cos(rotation*3.1415/360.f),0,sin(rotation*3.1415/360.f),0));
		q.normalize();
	}

	jointTransforms[j->index] = q;
	//q *= animData
	for (unsigned int i=0;i<j->children.size();i++){
		updateTransforms(Quaternion(1,0,0,0), &j->children[i]);
	}
}

void Skeleton::update(void){
	if (numJoints && !hasUpdated){
		Quaternion q(1,0,0,0);
		updateTransforms(q, &root);

		for (unsigned int i=0;i<vertexTransforms.size();i++){
			vertexTransforms[i] = q;
			for (unsigned int j=0;j<numJoints;j++){
				if (weights[i][j] == 1){
					vertexTransforms[i] = jointTransforms[j];
				} else if (weights[i][j]!=0){
					//if (jointTransforms[j].w != 1) std::cout<<"test\n";
					if (j == 0){
						vertexTransforms[i] = normalize(jointTransforms[j] * weights[i][j]);
					} else {
						vertexTransforms[i] = normalize(normalize(jointTransforms[j] * weights[i][j]) * vertexTransforms[i]);
					}
				}
			}
		}
		
		hasRun = true;
		hasUpdated = true;
	}
}