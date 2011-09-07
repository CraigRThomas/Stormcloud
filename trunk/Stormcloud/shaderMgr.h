#pragma once

#define ShaderMgr ShaderManager::SHMgr()

#include "shader.h"
#include <stack>
#include <vector>

typedef struct _ShaderProc{
	unsigned int vtShader, pxShader, geoShader, proc;
	int compileCode;
	_ShaderProc(){
		compileCode = 0;
	}
}ShaderProc;

class ShaderManager {
public:
	/** Singleton constructor. */
	static ShaderManager& SHMgr();
	/** Create new shader */
	Shader* createShader(ShaderType type, char* filepath, unsigned int priority = 0);
	/** Pop the top shader from the specified stack */
	void pop(ShaderType type = ShaderType::ALL);
	/** Push a shader onto the top of the appropreate stack */
	void push(Shader* shdr);
	/** Calls updateCurrentShaderProc if ShaderManager has been invalidated */
	void checkInvalid(void);
	void update();
protected:
	/** Constructor. */
	ShaderManager(void);
	/** Destructor. */
	~ShaderManager(void);
private:
	static ShaderManager* singleton; /**< Singleton pointer. */
	/** Singleton initialization function. */
	static void init();
	/** Is the shader manager invalid */
	bool invalid;
	/** List of existing shaders */
	std::vector<Shader*> shaders;
	/** List of existing shader programs */
	std::vector<ShaderProc*> shaderPrograms;
	/** Vertex shaders stack */
	std::stack<Shader*> vtShaders;
	/** Fragment shaders Stack */
	std::stack<Shader*> pxShaders;
	/** Geometry shaders stack */
	std::stack<Shader*> geoShaders;
	/** Return the stack for the specified shader type */
	std::stack<Shader*>* getStack(ShaderType type);
	/** Checks the current shader combination and creates/updates the active shader proc as needed */
	void updateCurrentShaderProc(void);
};
