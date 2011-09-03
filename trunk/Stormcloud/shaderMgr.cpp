#include "shaderMgr.h"

ShaderManager* ShaderManager::singleton = 0;

ShaderManager::ShaderManager(void){}

ShaderManager::~ShaderManager(void){
	delete singleton;
}

Shader* ShaderManager::createShader(ShaderType type, char* filepath, unsigned int priority){
	//std::cout<<filepath<<"\n";
	std::vector<Shader*>::iterator i;
	for (i=shaders.begin(); i!=shaders.end(); i++){
		if(!strcmp((*i)->filepath(), filepath)){
			return (*i);
		}
	}
	Shader* temp = new Shader(type, filepath, priority);
	shaders.push_back(temp);
	//std::cout<<shaders.size()<<"\n";
	return temp;
}

/** Pop the top shader from the stack */
void ShaderManager::pop(ShaderType type){
	if(type!=ShaderType::ALL){
		std::stack<Shader*>* curStack = getStack(type);
		if(!curStack->empty()){
			Shader* prevTop = curStack->top();
			curStack->pop();
			if(!curStack->empty()){
				if (prevTop->shaderID() != curStack->top()->shaderID()){
					invalid = true;
				}
			} else {
				invalid = true;
			}
			//delete[] prevTop;
			//prevTop = NULL;
		}
		//curStack = NULL;
	} else {
		vtShaders.pop();
		pxShaders.pop();
		geoShaders.pop();
		updateCurrentShaderProc();
	}
}

/** Push a shader onto the top of the stack */
void ShaderManager::push(Shader* shdr){
	//std::cout << "Pushing " << shdr->filepath()<< " ID:" << shdr->shaderID() << std::endl;
	bool newProc = false;
	std::stack<Shader*>* curStack = getStack(shdr->type());
	if (!curStack->empty()){
		if (curStack->top()->priority() > shdr->priority()){
			curStack->push(curStack->top());
		} else {
			newProc = true;
		}
	} else {
		newProc = true;
	}
	if (newProc){
		curStack->push(shdr);
		invalid = true;
	}
	//curStack = NULL;
}

/** Return the stack for the specified shader type */
std::stack<Shader*>* ShaderManager::getStack(ShaderType type){
	switch(type){
		case ShaderType::VERTEX:
			return &vtShaders;
		case ShaderType::FRAGMENT:
			return &pxShaders;
		case ShaderType::GEOMETRY:
			return &geoShaders;
		default:
			return NULL;
	}
}

void ShaderManager::updateCurrentShaderProc(void){
	bool newProc = false;
	unsigned int curShaders[3];
	curShaders[0] = (!vtShaders.empty())?vtShaders.top()->shaderID():0;
	curShaders[1] = (!pxShaders.empty())?pxShaders.top()->shaderID():0;
	curShaders[2] = (!geoShaders.empty())?geoShaders.top()->shaderID():0;
	std::vector<ShaderProc*>::iterator i;
	for (i=shaderPrograms.begin(); i!=shaderPrograms.end(); i++){
		if((*i)->vtShader == curShaders[0] && (*i)->pxShader == curShaders[1] && (*i)->geoShader == curShaders[2]){
			Shader::glUseProgramObjectARB((*i)->proc);
			if ((*i)->vtShader){
				if (!vtShaders.empty()){
					vtShaders.top()->getLocations((*i)->proc);
					vtShaders.top()->updateVars();
				}
			}
			if ((*i)->pxShader){
				if (!pxShaders.empty()){
					pxShaders.top()->getLocations((*i)->proc);
					pxShaders.top()->updateVars();
				}
			}
			return;
		}
	}

	ShaderProc* newShaderProc = new ShaderProc();
	
	newShaderProc->proc = Shader::glCreateProgramObjectARB();

	newShaderProc->pxShader = (pxShaders.empty())?0:pxShaders.top()->shaderID();
	if(newShaderProc!=0){Shader::glAttachObjectARB(newShaderProc->proc,newShaderProc->pxShader);}
	newShaderProc->vtShader = (vtShaders.empty())?0:vtShaders.top()->shaderID();
	if(newShaderProc!=0){Shader::glAttachObjectARB(newShaderProc->proc,newShaderProc->vtShader);}
	newShaderProc->geoShader = (geoShaders.empty())?0:geoShaders.top()->shaderID();
	if(newShaderProc!=0){Shader::glAttachObjectARB(newShaderProc->proc,newShaderProc->geoShader);}

	//std::cout << "Creating Shader: " << newShaderProc->proc << " (" << newShaderProc->pxShader << " " << newShaderProc->vtShader << " " << newShaderProc->geoShader << ")" << std::endl;
	
	Shader::glLinkProgramARB(newShaderProc->proc);
	
	int result = 0;
	Shader::glGetObjectParameterivARB(newShaderProc->proc, GL_OBJECT_LINK_STATUS_ARB, &result);
	
	if(!result){
		int infoLogSize;
		Shader::glGetObjectParameterivARB(newShaderProc->proc, GL_OBJECT_INFO_LOG_LENGTH_ARB,&infoLogSize);
		GLcharARB* infoLog = new GLcharARB[infoLogSize];
		Shader::glGetInfoLogARB(newShaderProc->proc, infoLogSize, NULL, infoLog);
		fprintf(stderr, "Error in program linkage!\n");
		fprintf(stderr, "Info log: %s\n", infoLog);
		Shader::glDeleteObjectARB(newShaderProc->proc);
	} else {
		shaderPrograms.push_back(newShaderProc);
		Shader::glUseProgramObjectARB(newShaderProc->proc);
		if (newShaderProc->vtShader){
			if (!vtShaders.empty()){
				vtShaders.top()->getLocations(newShaderProc->proc);
				vtShaders.top()->updateVars();
			}
		}
		if (newShaderProc->pxShader){
			if (!pxShaders.empty()){
				pxShaders.top()->getLocations(newShaderProc->proc);
				pxShaders.top()->updateVars();
			}
		}
	}
}

void ShaderManager::checkInvalid(void){
	if(invalid){
		invalid = false;
		updateCurrentShaderProc();
	}
}

ShaderManager& ShaderManager::SHMgr(){
	if (!singleton){
		singleton = new ShaderManager();
		init();
	}
	return *singleton;
}

void ShaderManager::init(){
	ShaderProc* blankProc = new ShaderProc();
	blankProc->pxShader = 0;
	blankProc->vtShader = 0;
	blankProc->geoShader = 0;
	blankProc->proc = 0;
	singleton->shaderPrograms.push_back(blankProc);
	singleton->invalid = false;
	//std::cout<<"Shader Manager initialized.\n";
}

void ShaderManager::update(){
	if (!vtShaders.empty()){
		vtShaders.top()->updateVars();
	}
	if (!pxShaders.empty()){
		pxShaders.top()->updateVars();
	}
}