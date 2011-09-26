#pragma once

#define NUM_BUFFERS 2

#include <stdlib.h>
#include <gl/glut.h>
#include <gl/glext.h>
#include <vector>
#include "shader.h"
#include "shaderMgr.h"
#include "matrix.h"

enum {
	VBO = 0,
	IBO = 1
};

struct Material {
	GLfloat diffuse[4];
	GLfloat ambient[4];
	GLfloat specular[4];
	GLint shininess;
};

class Mesh {
public:
	GLuint num_verts;			/**< Number of vertices. Also the number of normals and texcoords. */
	GLfloat* vertices;			/**< List of vertices. */
	GLfloat* normals;			/**< List of normals. */
	GLfloat* texCoords;			/**< List of texture coordinates. */
	GLuint* buffers;			/**< OpenGL GPU data buffer ids. */
	char groupName[256];		/**< Mesh's group name. */
	GLuint num_faces;			/**< Number of triangle faces composing this mesh. */
	GLuint* faces;				/**< List of vertex/normal/tex coord indices for each face. */
	GLuint texId;				/**< Registered texture id. */
	GLuint normMapId;			/**< Registered normal map id. */
	Shader* vertShader;			/**< Vertex shader. */
	Shader* fragShader;			/**< Fragment shader. */
	Material mat;				/**< Light interaction properties. */
	bool normMapped;			/**< True if this mesh has normal map information. */
	std::vector<Matrix> TBNs;	/**< Tangent, bitangent, normal matrices for normal mapping. */
	Matrix transform;			/**< Homogenous transformation matrix. */

	/** Default constructor. */
	Mesh();
	/** Destructor. */
	~Mesh();
	/** Step function. */
	void update(const GLfloat& dt);
	/** Draw function. */
	void draw(Matrix &t);
	/** Allocates GPU buffers for sending data. Pre: num_verts must be set. */
	void allocateBuffers();
private:
	static PFNGLGENBUFFERSARBPROC glGenBuffers;		 /**< Function for generating GPU buffers. */
	static PFNGLBINDBUFFERARBPROC glBindBuffer;		 /**< Function for setting the active buffer. */
	static PFNGLBUFFERDATAARBPROC glBufferData;		 /**< Function for sending data to the active buffer. */
	static PFNGLBUFFERSUBDATAARBPROC glBufferSubData;/**< Function for updating a specififed part of the active buffer. */
	static PFNGLDELETEBUFFERSARBPROC glDeleteBuffers;/**< Cleanup function for GPU buffers. */
	static PFNGLACTIVETEXTUREARBPROC glActiveTexture;/**< Function for setting the active texture. */
	static PFNGLVERTEXATTRIBPOINTERARBPROC glVertexAttribPointer;
	static PFNGLENABLEVERTEXATTRIBARRAYARBPROC glEnableVertexAttribArray;
	static bool init;								 /**< True once the above functions have been retrieved. */
	/** Retrieves the processes above. */
	void initBufferProcs();
	/** Calculates the TBN matrices for normal mapping. */
	void calcTBNs();
};