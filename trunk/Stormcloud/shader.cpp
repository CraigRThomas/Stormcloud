#include "shader.h"
#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>
PFNGLCREATEPROGRAMOBJECTARBPROC   Shader::glCreateProgramObjectARB  = NULL;
PFNGLCREATESHADEROBJECTARBPROC    Shader::glCreateShaderObjectARB   = NULL;
PFNGLLINKPROGRAMARBPROC           Shader::glLinkProgramARB          = NULL;
PFNGLCOMPILESHADERARBPROC         Shader::glCompileShaderARB        = NULL;
PFNGLGETINFOLOGARBPROC            Shader::glGetInfoLogARB           = NULL;
PFNGLDELETEOBJECTARBPROC          Shader::glDeleteObjectARB         = NULL;
PFNGLUSEPROGRAMOBJECTARBPROC      Shader::glUseProgramObjectARB     = NULL;
PFNGLSHADERSOURCEARBPROC          Shader::glShaderSourceARB         = NULL;
PFNGLATTACHOBJECTARBPROC          Shader::glAttachObjectARB         = NULL;
PFNGLDETACHOBJECTARBPROC          Shader::glDetachObjectARB         = NULL;
PFNGLGETOBJECTPARAMETERIVARBPROC  Shader::glGetObjectParameterivARB = NULL;
PFNGLGETUNIFORMLOCATIONARBPROC    Shader::glGetUniformLocationARB   = NULL;
PFNGLUNIFORM4FARBPROC             Shader::glUniform4fARB            = NULL;
PFNGLUNIFORM3FARBPROC             Shader::glUniform3fARB            = NULL;
PFNGLUNIFORM2FARBPROC             Shader::glUniform2fARB            = NULL;
PFNGLUNIFORM1FARBPROC             Shader::glUniform1fARB            = NULL;
PFNGLUNIFORM4IARBPROC             Shader::glUniform4iARB            = NULL;
PFNGLUNIFORM3IARBPROC             Shader::glUniform3iARB            = NULL;
PFNGLUNIFORM2IARBPROC             Shader::glUniform2iARB            = NULL;
PFNGLUNIFORM1IARBPROC             Shader::glUniform1iARB            = NULL;
PFNGLUNIFORM1FVARBPROC			  Shader::glUniform1fvARB			= NULL;
PFNGLUNIFORM2FVARBPROC			  Shader::glUniform2fvARB			= NULL;
PFNGLUNIFORM3FVARBPROC			  Shader::glUniform3fvARB			= NULL;
PFNGLUNIFORM4FVARBPROC			  Shader::glUniform4fvARB			= NULL;
PFNGLUNIFORM1IVARBPROC			  Shader::glUniform1ivARB			= NULL;
PFNGLUNIFORM2IVARBPROC			  Shader::glUniform2ivARB			= NULL;
PFNGLUNIFORM3IVARBPROC			  Shader::glUniform3ivARB			= NULL;
PFNGLUNIFORM4IVARBPROC			  Shader::glUniform4ivARB			= NULL;
PFNGLUNIFORMMATRIX2FVARBPROC	  Shader::glUniformMatrix2fvARB		= NULL;
PFNGLUNIFORMMATRIX3FVARBPROC	  Shader::glUniformMatrix3fvARB		= NULL;
PFNGLUNIFORMMATRIX4FVARBPROC	  Shader::glUniformMatrix4fvARB		= NULL;
PFNGLGETATTRIBLOCATIONARBPROC	  Shader::glGetAttribLocationARB	= NULL;
PFNGLVERTEXATTRIBPOINTERPROC	  Shader::glVertexAttribPointer		= NULL;
PFNGLBINDBUFFERARBPROC			  Shader::glBindBuffer				= NULL;
PFNGLENABLEVERTEXATTRIBARRAYPROC  Shader::glEnableVertexAttribArray = NULL;

bool Shader::init = false;

Shader::Shader(void){
	if(!Shader::init){
		Shader::initShaderProcs();
		init = true;
	}
	_priority = 0;
	_compileCode = 0;
}

Shader::Shader(ShaderType type, char* filepath, unsigned int priority){
	if(!Shader::init){
		Shader::initShaderProcs();
		init = true;
	}
	_type = type;
	_priority = priority;
	load(filepath);
}

Shader::~Shader(void){
}

void Shader::load(char* filepath){
	strcpy_s(_filepath,sizeof(_filepath),filepath);

	std::ifstream f(_filepath, std::ios_base::in|std::ios_base::binary);
	
	f.seekg(0, std::ios_base::beg);
	std::ifstream::pos_type begin_pos = f.tellg();
	f.seekg(0, std::ios_base::end);
	int shaderSourceLength = static_cast<int>(f.tellg() - begin_pos);
	char* shaderSource = new char[shaderSourceLength];
	f.seekg(0, std::ios_base::beg);
	std::string str;
	char* str_c;
	
	if(f.good()){
		f.getline(shaderSource,shaderSourceLength,'\0');
	}
	f.clear();
	f.seekg(0, std::ios_base::beg);

	char type[128];
	char name[128];
	std::string arraySize;
	UniformFloat a;
	UniformInt c;
	bool mat = false;
	while(f.good()){
		getline(f,str);
		str_c = new char[str.length()];
		str_c = strdup(str.c_str());
		if (strstr(str_c,"uniform ") || strstr(str_c,"attribute ")){
			str = str.substr(0,str.length()-2);
			str_c = strdup(str.c_str());
			a.size = 0;
			c.size = 0;
			if (strchr(str_c,'[')!=0){
				arraySize = str.substr(str.find("[")+1,str.find("]")-str.find("[")-1);
				str = str.substr(0, str.find("["));
			} else {
				arraySize = "1";
			}
			sscanf_s(str_c,"%*s %s %s",type,sizeof(type),name,sizeof(name));
			
			if (strcmp(type,"float")==0){
				a.size = 1;
			} else if (strcmp(type,"vec2")==0){
				a.size = 2;
			} else if (strcmp(type,"vec3")==0){
				a.size = 3;
			} else if (strcmp(type,"vec4")==0){
				a.size = 4;
			} else if (strcmp(type,"mat2")==0){
				a.size = 2;
				arraySize = "2";
				mat = true;
			} else if (strcmp(type,"mat3")==0){
				a.size = 3;
				arraySize = "3";
				mat = true;
			} else if (strcmp(type,"mat4")==0){
				a.size = 4;
				arraySize = "4";
				mat = true;
			}
			if (a.size > 0){
				sscanf_s(arraySize.c_str(),"%u",&a.arraySize);
				strcpy_s(a.name,sizeof(a.name),name);
				a.values = new float[a.size*a.arraySize];
				for (unsigned int i=0;i<a.size*a.arraySize;i++){
					a.values[i] = -1;
				}
				a.loc = -1;
				a.mat = mat;
				if (strstr(str_c,"uniform ")){
					floats.push_back(a);
				} else {
					attribs.push_back(a);
				}
			}

			if (strcmp(type,"int")==0){
				c.size = 1;
			} else if (strcmp(type,"ivec2")==0){
				c.size = 2;
			} else if (strcmp(type,"ivec3")==0){
				c.size = 3;
			} else if (strcmp(type,"ivec4")==0){
				c.size = 4;
			}
			if (c.size > 0){
				sscanf_s(arraySize.c_str(),"%u",&c.arraySize);
				strcpy_s(c.name,sizeof(c.name),name);
				c.values = new int[c.size*c.arraySize];
				for (unsigned int i=0;i<c.size*c.arraySize;i++){
					c.values[i] = -1;
				}
				c.loc = -1;
				ints.push_back(c);
			}

			if (strcmp(type,"mat2")==0){
				a.size = 2;
			} else if (strcmp(type,"mat3")==0){
				a.size = 3;
			}
		}

	}
	f.close();
	delete [] str_c;
	str_c = NULL;

	_compileCode = 0;
	
	_shaderObject = glCreateShaderObjectARB(_type);
	glShaderSourceARB(_shaderObject, 1, (const char**)&shaderSource, NULL);
	glCompileShaderARB(_shaderObject);
	glGetObjectParameterivARB(_shaderObject, GL_OBJECT_COMPILE_STATUS_ARB, &_compileCode);

	if(!_compileCode){
		int infoLogSize;
		glGetObjectParameterivARB(_shaderObject, GL_OBJECT_INFO_LOG_LENGTH_ARB,&infoLogSize);
		GLcharARB* infoLog = new GLcharARB[infoLogSize];
		glGetInfoLogARB(_shaderObject, infoLogSize, NULL, infoLog);
		fprintf(stderr, "Error in shader compilation! (%s)\n", _filepath);
		fprintf(stderr, "Info log: %s\n", infoLog);
	}
	delete [] shaderSource;
	shaderSource = NULL;
	free(shaderSource);
}

void Shader::initShaderProcs(void){
	glCreateProgramObjectARB = (PFNGLCREATEPROGRAMOBJECTARBPROC)wglGetProcAddress("glCreateProgramObjectARB");
	glCreateShaderObjectARB = (PFNGLCREATESHADEROBJECTARBPROC)wglGetProcAddress("glCreateShaderObjectARB");
	glCompileShaderARB = (PFNGLCOMPILESHADERARBPROC)wglGetProcAddress("glCompileShaderARB");
	glLinkProgramARB = (PFNGLLINKPROGRAMARBPROC)wglGetProcAddress("glLinkProgramARB");
	glGetInfoLogARB = (PFNGLGETINFOLOGARBPROC)wglGetProcAddress("glGetInfoLogARB");
	glDeleteObjectARB  = (PFNGLDELETEOBJECTARBPROC)wglGetProcAddress("glDeleteObjectARB");
	glUseProgramObjectARB = (PFNGLUSEPROGRAMOBJECTARBPROC)wglGetProcAddress("glUseProgramObjectARB");
	glShaderSourceARB = (PFNGLSHADERSOURCEARBPROC)wglGetProcAddress("glShaderSourceARB");
	glAttachObjectARB = (PFNGLATTACHOBJECTARBPROC)wglGetProcAddress("glAttachObjectARB");
	glDetachObjectARB = (PFNGLDETACHOBJECTARBPROC)wglGetProcAddress("glDetachObjectARB");
	glGetObjectParameterivARB = (PFNGLGETOBJECTPARAMETERIVARBPROC)wglGetProcAddress("glGetObjectParameterivARB");
	glGetUniformLocationARB = (PFNGLGETUNIFORMLOCATIONARBPROC)wglGetProcAddress("glGetUniformLocationARB");
	glUniform4fARB = (PFNGLUNIFORM4FARBPROC)wglGetProcAddress("glUniform4fARB");
	glUniform3fARB = (PFNGLUNIFORM3FARBPROC)wglGetProcAddress("glUniform3fARB");
	glUniform2fARB = (PFNGLUNIFORM2FARBPROC)wglGetProcAddress("glUniform2fARB");
	glUniform1fARB = (PFNGLUNIFORM1FARBPROC)wglGetProcAddress("glUniform1fARB");
	glUniform4iARB = (PFNGLUNIFORM4IARBPROC)wglGetProcAddress("glUniform4iARB");
	glUniform3iARB = (PFNGLUNIFORM3IARBPROC)wglGetProcAddress("glUniform3iARB");
	glUniform2iARB = (PFNGLUNIFORM2IARBPROC)wglGetProcAddress("glUniform2iARB");
	glUniform1iARB = (PFNGLUNIFORM1IARBPROC)wglGetProcAddress("glUniform1iARB");
	glUniform1fvARB = (PFNGLUNIFORM1FVARBPROC)wglGetProcAddress("glUniform1fvARB");
	glUniform2fvARB = (PFNGLUNIFORM2FVARBPROC)wglGetProcAddress("glUniform2fvARB");
	glUniform3fvARB = (PFNGLUNIFORM3FVARBPROC)wglGetProcAddress("glUniform3fvARB");
	glUniform4fvARB = (PFNGLUNIFORM4FVARBPROC)wglGetProcAddress("glUniform4fvARB");
	glUniform1ivARB = (PFNGLUNIFORM1IVARBPROC)wglGetProcAddress("glUniform1ivARB");
	glUniform2ivARB = (PFNGLUNIFORM2IVARBPROC)wglGetProcAddress("glUniform2ivARB");
	glUniform3ivARB = (PFNGLUNIFORM3IVARBPROC)wglGetProcAddress("glUniform3ivARB");
	glUniform4ivARB = (PFNGLUNIFORM4IVARBPROC)wglGetProcAddress("glUniform4ivARB");
	glUniformMatrix2fvARB = (PFNGLUNIFORMMATRIX2FVARBPROC)wglGetProcAddress("glUniformMatrix2fvARB");
	glUniformMatrix3fvARB = (PFNGLUNIFORMMATRIX3FVARBPROC)wglGetProcAddress("glUniformMatrix3fvARB");
	glUniformMatrix4fvARB = (PFNGLUNIFORMMATRIX4FVARBPROC)wglGetProcAddress("glUniformMatrix4fvARB");
	glGetAttribLocationARB = (PFNGLGETATTRIBLOCATIONARBPROC)wglGetProcAddress("glGetAttribLocationARB");
	glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)wglGetProcAddress("glVertexAttribPointer");
	glBindBuffer = (PFNGLBINDBUFFERARBPROC)wglGetProcAddress("glBindBuffer");
	glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glEnableVertexAttribArray");
}

char* Shader::filepath(void){
	return _filepath;
}

unsigned int Shader::shaderID(void){
	return _shaderObject;
}

unsigned int Shader::priority(void){
	return _priority;
}

ShaderType Shader::type(void){
	return _type;
}

int Shader::compileCode(void){
	return _compileCode;
}

float* Shader::getFloat(char* name){
	for (unsigned int i=0;i<floats.size();i++){
		if (strcmp(name,floats[i].name)==0){
			return floats[i].values;
		}
	}
	for (unsigned int i=0;i<attribs.size();i++){
		if (strcmp(name,attribs[i].name)==0){
			return attribs[i].values;
		}
	}
}

void Shader::send(char *name, float value){
	for (unsigned int i=0;i<floats.size();i++){
		if (strcmp(name,floats[i].name)==0){
			floats[i].values[0] = value;
		}
	}
}

void Shader::send(char *name, float value[]){
	for (unsigned int i=0;i<floats.size();i++){
		if (strcmp(name,floats[i].name)==0){
			for (unsigned int j=0;j<floats[i].size*floats[i].arraySize;j++){
				floats[i].values[j] = value[j];
			}
			return;
		}
	}
	for (unsigned int i=0;i<attribs.size();i++){
		if (strcmp(name,attribs[i].name)==0){
			for (unsigned int j=0;j<attribs[i].size*attribs[i].arraySize;j++){
				attribs[i].values[j] = value[j];
			}
			return;
		}
	}
}

void Shader::send(char *name, int value){
	for (unsigned int i=0;i<ints.size();i++){
		if (strcmp(name,ints[i].name)==0){
			ints[i].values[0] = value;
		}
	}
}

void Shader::send(char *name, int value[]){
	for (unsigned int i=0;i<ints.size();i++){
		if (strcmp(name,ints[i].name)==0){
			for (unsigned int j=0;j<ints[i].size;j++){
				ints[i].values[j] = value[j];
			}
		}
	}
}

void Shader::updateVars(void){
	for (unsigned int i=0;i<floats.size();i++){
		if (floats[i].values[0] >= 0){
			if (!floats[i].mat){
				switch(floats[i].size){
					case 1:
						if (floats[i].arraySize == 1){
							glUniform1fARB(floats[i].loc, floats[i].values[0]); break;
						} else {
							glUniform1fvARB(floats[i].loc, floats[i].size*floats[i].arraySize, floats[i].values); break;
						}
					case 2:
						if (floats[i].arraySize == 1){
							glUniform2fARB(floats[i].loc, floats[i].values[0], floats[i].values[1]); break;
						} else {
							glUniform2fvARB(floats[i].loc, floats[i].size*floats[i].arraySize, floats[i].values); break;
						}
					case 3:
						if (floats[i].arraySize == 1){
							glUniform3fARB(floats[i].loc, floats[i].values[0], floats[i].values[1], floats[i].values[2]); break;
						} else {
							glUniform3fvARB(floats[i].loc, floats[i].size*floats[i].arraySize, floats[i].values); break;
						}
					case 4:
						if (floats[i].arraySize == 1){
							glUniform4fARB(floats[i].loc, floats[i].values[0], floats[i].values[1], floats[i].values[2], floats[i].values[3]); break;
						} else {
							glUniform4fvARB(floats[i].loc, floats[i].size*floats[i].arraySize, floats[i].values); break;
						}
				}
			} else {
				switch(floats[i].size){
					case 2:
						glUniformMatrix2fvARB(floats[i].loc, floats[i].size*floats[i].arraySize, false, floats[i].values); break;
					case 3:
						glUniformMatrix3fvARB(floats[i].loc, floats[i].size*floats[i].arraySize, false, floats[i].values); break;
					case 4:
						glUniformMatrix4fvARB(floats[i].loc, floats[i].size*floats[i].arraySize, false, floats[i].values); break;
				}
			}
		}
	}
	for (unsigned int i=0;i<ints.size();i++){
		if (ints[i].values[0] >= 0){
			switch(ints[i].size){
				case 1:
					if (ints[i].arraySize == 1){
						glUniform1iARB(ints[i].loc, ints[i].values[0]); break;
					} else {
						glUniform1ivARB(ints[i].loc, ints[i].size*ints[i].arraySize, ints[i].values); break;
					}
				case 2:
					if (ints[i].arraySize == 1){
						glUniform2iARB(ints[i].loc, ints[i].values[0], ints[i].values[1]); break;
					} else {
						glUniform2ivARB(ints[i].loc, ints[i].size*ints[i].arraySize, ints[i].values); break;
					}
				case 3:
					if (ints[i].arraySize == 1){
						glUniform3iARB(ints[i].loc, ints[i].values[0], ints[i].values[1], ints[i].values[2]); break;
					} else {
						glUniform3ivARB(ints[i].loc, ints[i].size*ints[i].arraySize, ints[i].values); break;
					}
				case 4:
					if (ints[i].arraySize == 1){
						glUniform4iARB(ints[i].loc, ints[i].values[0], ints[i].values[1], ints[i].values[2], ints[i].values[3]); break;
					} else {
						glUniform4ivARB(ints[i].loc, ints[i].size*ints[i].arraySize, ints[i].values); break;
					}
			}
		}
	}
	for (unsigned int i=0;i<attribs.size();i++){
		glBindBuffer(GL_ARRAY_BUFFER,0);
		glEnableVertexAttribArray(attribs[i].loc);
		/*if (strcmp(attribs[i].name,"vNormal")==0){
			for (unsigned int j=0;j<12;j++){
				std::cout<<attribs[i].values[j*3]<<" "<<attribs[i].values[j*3+1]<<" "<<attribs[i].values[j*3+2]<<"\n";
			}
		}*/
		glVertexAttribPointer(attribs[i].loc, 3, GL_FLOAT, GL_TRUE, 0, attribs[i].values);
	}
			
}

void Shader::getLocations(unsigned int proc_id){
	glUniform1iARB(glGetUniformLocationARB(proc_id,"image"),0);
	glUniform1iARB(glGetUniformLocationARB(proc_id,"normMap"),1);
	for (unsigned int i=0;i<floats.size();i++){
		floats[i].loc = glGetUniformLocationARB(proc_id, floats[i].name);
	}
	for (unsigned int i=0;i<ints.size();i++){
		ints[i].loc = glGetUniformLocationARB(proc_id, ints[i].name);
	}
	for (unsigned int i=0;i<attribs.size();i++){
		attribs[i].loc = glGetAttribLocationARB(proc_id, attribs[i].name);
	}
}