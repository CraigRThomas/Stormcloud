#pragma once

#define NUM_BUFFERS 2

#include <stdlib.h>
#include <gl/glut.h>
#include <gl/glext.h>
#include <vector>

enum {
	DATA_BUFFER = 0,
	INDICES_BUFFER = 1
};

class Mesh {
public:
	GLuint num_verts;		/**< Number of vertices. Also the number of normals and texcoords. */
	GLfloat* vertices;		/**< List of vertices. */
	GLfloat* normals;		/**< List of normals. */
	GLfloat* texCoords;		/**< List of texture coordinates. */
	GLuint* buffers;		/**< OpenGL GPU data buffer ids. */
	char groupName[256];	/**< Mesh's group name. */
	GLuint num_faces;		/**< Number of triangle faces composing this mesh. */
	GLuint* faces;			/**< List of vertex/normal/tex coord indices for each face. */
	GLuint texId;			/**< Registed texture id. */

	/** Default constructor. */
	Mesh();
	/** Destructor. */
	~Mesh();
	/** Step function. */
	void update(const GLfloat& dt);
	/** Draw function. */
	void draw();
	/** Allocates GPU buffers for sending data. Pre: num_verts must be set. */
	void allocateBuffers();
private:
	static PFNGLGENBUFFERSARBPROC glGenBuffers;		 /**< Function for generating GPU buffers. */
	static PFNGLBINDBUFFERARBPROC glBindBuffer;		 /**< Function for setting the active buffer. */
	static PFNGLBUFFERDATAARBPROC glBufferData;		 /**< Function for sending data to the active buffer. */
	static PFNGLBUFFERSUBDATAARBPROC glBufferSubData;/**< Function for updating a specififed part of the active buffer. */
	static PFNGLDELETEBUFFERSARBPROC glDeleteBuffers;/**< Cleanup function for GPU buffers. */
	static bool init;								 /**< True once the above functions have been retrieved. */
	/** Retrieves the processes above. */
	void initBufferProcs();
};