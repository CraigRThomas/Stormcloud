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
	std::vector<UniformFloat> attribs;	/**< Collection of attributes to be sent to the GPU. */

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
	/** Returns the last error code for the shader **/
	int compileCode(void);
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
	void updateVars(void);
	/** Gets the location of each uniform variable from the current shader program. */
	void getLocations(unsigned int proc_id);
	/** Returns the memory location of the array holding the values for the specified variable. */
	float* getFloat(char* name);
	/** Returns the memory location of the array holding the values for the specified variable. */
	int* getInt(char* name);

	// Various useful shader-related functions defined in GLEXT.H 
	static PFNGLCREATEPROGRAMOBJECTARBPROC   glCreateProgramObjectARB;	/**< Creates an empty shader program. */
	static PFNGLLINKPROGRAMARBPROC           glLinkProgramARB;			/**< Links the specified program to the GPU. */
	static PFNGLCREATESHADEROBJECTARBPROC    glCreateShaderObjectARB;	/**< Creates an empty shader. */
	static PFNGLCOMPILESHADERARBPROC         glCompileShaderARB;		/**< Compiles the shader code. */
	static PFNGLGETINFOLOGARBPROC            glGetInfoLogARB;			/**< Returns useful debug info. */
	static PFNGLDELETEOBJECTARBPROC          glDeleteObjectARB;			/**< Deletes the specified program object. */
	static PFNGLUSEPROGRAMOBJECTARBPROC      glUseProgramObjectARB;		/**< Tells OpenGL which program to use. */
	static PFNGLSHADERSOURCEARBPROC          glShaderSourceARB;			/**< Passes shader source code to the specified shader object. */
	static PFNGLATTACHOBJECTARBPROC          glAttachObjectARB;			/**< Attaches a shader object to a program object. */
	static PFNGLDETACHOBJECTARBPROC          glDetachObjectARB;			/**< Detaches a shader object from a program object. */
	static PFNGLGETOBJECTPARAMETERIVARBPROC  glGetObjectParameterivARB;	/**< Stores the specified integer parameter in the specified integer argument. */
	static PFNGLGETUNIFORMLOCATIONARBPROC    glGetUniformLocationARB;	/**< Returns the location of the specified uniform variable in the program object. */
	static PFNGLUNIFORM4FARBPROC             glUniform4fARB;			/**< Passes 4 floating point numbers to the specified vec4. */
	static PFNGLUNIFORM3FARBPROC			 glUniform3fARB;			/**< Passes 3 floating point numbers to the specified vec3. */
	static PFNGLUNIFORM2FARBPROC			 glUniform2fARB;			/**< Passes 2 floating point numbers to the specified vec2. */
	static PFNGLUNIFORM1FARBPROC             glUniform1fARB;			/**< Passes a floating point number to the specified float. */
	static PFNGLUNIFORM4IARBPROC             glUniform4iARB;			/**< Passes 4 integers to the specified ivec4. */
	static PFNGLUNIFORM3IARBPROC             glUniform3iARB;			/**< Passes 3 integers to the specified ivec3. */
	static PFNGLUNIFORM2IARBPROC             glUniform2iARB;			/**< Passes 2 integers to the specified ivec2. */
	static PFNGLUNIFORM1IARBPROC             glUniform1iARB;			/**< Passes an integer to the specified int. */
	static PFNGLUNIFORM1FVARBPROC			 glUniform1fvARB;			/**< Passes an array of single floats. */
	static PFNGLUNIFORM2FVARBPROC			 glUniform2fvARB;			/**< Passes an array of vec2 floats. */
	static PFNGLUNIFORM3FVARBPROC			 glUniform3fvARB;			/**< Passes an array of vec3 floats. */
	static PFNGLUNIFORM4FVARBPROC			 glUniform4fvARB;			/**< Passes an array of vec4 floats. */
	static PFNGLUNIFORM1IVARBPROC			 glUniform1ivARB;			/**< Passes an array of single ints. */
	static PFNGLUNIFORM2IVARBPROC			 glUniform2ivARB;			/**< Passes an array of ivec2 ints. */
	static PFNGLUNIFORM3IVARBPROC			 glUniform3ivARB;			/**< Passes an array of ivec3 ints. */
	static PFNGLUNIFORM4IVARBPROC			 glUniform4ivARB;			/**< Passes an array of ivec4 ints. */
	static PFNGLUNIFORMMATRIX2FVARBPROC		 glUniformMatrix2fvARB;		/**< Passes an array of 4 floats for a mat2. */
	static PFNGLUNIFORMMATRIX3FVARBPROC		 glUniformMatrix3fvARB;		/**< Passes an array of 9 floats for a mat3. */
	static PFNGLUNIFORMMATRIX4FVARBPROC		 glUniformMatrix4fvARB;		/**< Passes an array of 16 floats for a mat4. */
	static PFNGLGETATTRIBLOCATIONARBPROC	 glGetAttribLocationARB;	/**< Returns the location of a generic vertex attribute. */
	static PFNGLVERTEXATTRIBPOINTERPROC		 glVertexAttribPointer;		/**< Sets the location the specified vertex attribute gets its data from. */
	static PFNGLBINDBUFFERARBPROC			 glBindBuffer;				/**< Sets the active buffer. */
	static PFNGLENABLEVERTEXATTRIBARRAYPROC  glEnableVertexAttribArray; /**< Enables an array of data for the specified vertex attribute. */

private:
	ShaderType _type;			/**< Type of shader (vertex, geometry, pixel). */
	unsigned int _shaderObject;	/**< Shader id. */
	unsigned int _priority;		/**< Shader priority level. */
	char _filepath[255];		/**< File the shader was loaded from. */
	int _compileCode; /**< Error code from shader compilation **/

	/** Gets the process addresses for the functions defined in GLEXT.H. */
	void initShaderProcs(void);

	static bool init;			/**< True once the above functions have been retrieved. */
};
