#pragma once

#include <cstdio>
#include <iostream>
#include <istream>
#include <fstream>
#include <string>
#include <vector>

#include <GL/glut.h>
#include <GL/glext.h>

enum ShaderType {ALL = 0, VERTEX = GL_VERTEX_SHADER_ARB, FRAGMENT = GL_FRAGMENT_SHADER_ARB, GEOMETRY = GL_OBJECT_TYPE_ARB};

struct UniformFloat {
	char name[256];			/**< Variable name. */
	unsigned int loc;		/**< Location of variable on GPU. */
	unsigned int size;		/**< Size of the variable (1=single, 2=vec2, 3=vec3, 4=vec4). */
	unsigned int arraySize; /**< # of elements this variable has. */
	float* values;			/**< Data to be sent to GPU. */
	bool mat;				/**< True if this variable is a matrix. */
};

struct UniformInt {
	char name[256];			/**< Variable name. */
	unsigned int loc;		/**< Location of variable on GPU. */
	unsigned int size;		/**< Size of the variable (1=single, 2=vec2, 3=vec3, 4=vec4). */
	unsigned int arraySize; /**< # of elements this variable has. */
	int* values;			/**< Data to be sent to GPU. */
};

class Shader {
public:
	std::vector<UniformFloat> floats;	/**< Collection of uniform floats to be sent to the GPU. */
	std::vector<UniformInt> ints;		/**< Collection of uniform ints to be sent to the GPU. */

	/** Default constructor. */
	Shader(void);
	/** Constructor to load from file. */
	Shader(ShaderType type, char* filepath, unsigned int priority = 0);
	/** Destructor. */
	~Shader(void);
	/** Loads a shader from the specified .glsl file. */
	void load(char* filepath);
	/** Returns the filepath for this shader. */
	char* filepath(void);
	/** Returns the id for this shader. */
	unsigned int shaderID(void);
	/** Returns the priority level for this shader. */
	unsigned int priority(void);
	/** Returns this shader's type. */
	ShaderType type(void);
	/** Sends a float to the GPU. */
	void send(char* name, float value);
	/** Sends an array of floats to the GPU. */
	void send(char* name, float value[]);
	/** Sends an integer to the GPU. */
	void send(char* name, int value);
	/** Send an array of ints to the GPU. */
	void send(char* name, int value[]);
	/** Sends a matrix to the GPU. */
	void sendMatrix(char* name, float value[]);
	/** Takes care of actually sending the data to the GPU. */
	void updateUniforms(void);
	/** Gets the location of each uniform variable from the current shader program. */
	void getUniformLocations(unsigned int proc_id);

	// Various useful shader-related functions defined in GLEXT.H 
	static PFNGLCREATEPROGRAMOBJECTARBPROC   glCreateProgramObjectARB;
	static PFNGLCREATESHADEROBJECTARBPROC    glCreateShaderObjectARB;
	static PFNGLLINKPROGRAMARBPROC           glLinkProgramARB;
	static PFNGLCOMPILESHADERARBPROC         glCompileShaderARB;
	static PFNGLGETINFOLOGARBPROC            glGetInfoLogARB;
	static PFNGLDELETEOBJECTARBPROC          glDeleteObjectARB;
	static PFNGLUSEPROGRAMOBJECTARBPROC      glUseProgramObjectARB;
	static PFNGLSHADERSOURCEARBPROC          glShaderSourceARB;
	static PFNGLATTACHOBJECTARBPROC          glAttachObjectARB;
	static PFNGLDETACHOBJECTARBPROC          glDetachObjectARB;
	static PFNGLGETOBJECTPARAMETERIVARBPROC  glGetObjectParameterivARB;
	static PFNGLGETUNIFORMLOCATIONARBPROC    glGetUniformLocationARB;
	static PFNGLUNIFORM4FARBPROC             glUniform4fARB;
	static PFNGLUNIFORM3FARBPROC			 glUniform3fARB;
	static PFNGLUNIFORM2FARBPROC			 glUniform2fARB;
	static PFNGLUNIFORM1FARBPROC             glUniform1fARB;
	static PFNGLUNIFORM4IARBPROC             glUniform4iARB;
	static PFNGLUNIFORM3IARBPROC             glUniform3iARB;
	static PFNGLUNIFORM2IARBPROC             glUniform2iARB;
	static PFNGLUNIFORM1IARBPROC             glUniform1iARB;
	static PFNGLUNIFORM1FVARBPROC			 glUniform1fvARB;
	static PFNGLUNIFORM2FVARBPROC			 glUniform2fvARB;
	static PFNGLUNIFORM3FVARBPROC			 glUniform3fvARB;
	static PFNGLUNIFORM4FVARBPROC			 glUniform4fvARB;
	static PFNGLUNIFORM1IVARBPROC			 glUniform1ivARB;
	static PFNGLUNIFORM2IVARBPROC			 glUniform2ivARB;
	static PFNGLUNIFORM3IVARBPROC			 glUniform3ivARB;
	static PFNGLUNIFORM4IVARBPROC			 glUniform4ivARB;
	static PFNGLUNIFORMMATRIX2FVARBPROC		 glUniformMatrix2fvARB;
	static PFNGLUNIFORMMATRIX3FVARBPROC		 glUniformMatrix3fvARB;
	static PFNGLUNIFORMMATRIX4FVARBPROC		 glUniformMatrix4fvARB;
	static PFNGLGETATTRIBLOCATIONARBPROC	 glGetAttribLocationARB;
	static PFNGLVERTEXATTRIBPOINTERPROC		 glVertexAttribPointer;
	static PFNGLENABLEVERTEXATTRIBARRAYPROC	 glEnableVertexAttribArray;
	static PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;

private:
	ShaderType _type;			/**< Type of shader (vertex, geometry, pixel). */
	unsigned int _shaderObject;	/**< Shader id. */
	unsigned int _priority;		/**< Shader priority level. */
	char _filepath[255];		/**< File the shader was loaded from. */
	/** Gets the process addresses for the functions defined in GLEXT.H. */
	void initShaderProcs(void);	

	static bool init;			/**< True once the above functions have been retrieved. */
};
