#include "mesh.h"

bool Mesh::init = false;
PFNGLGENBUFFERSARBPROC		Mesh::glGenBuffers		= NULL;
PFNGLBINDBUFFERARBPROC		Mesh::glBindBuffer		= NULL;
PFNGLBUFFERDATAARBPROC		Mesh::glBufferData		= NULL;
PFNGLBUFFERSUBDATAARBPROC	Mesh::glBufferSubData	= NULL;
PFNGLDELETEBUFFERSARBPROC	Mesh::glDeleteBuffers	= NULL;

Mesh::Mesh(){
	vertices = normals = texCoords = 0;
	texId = 0;
	buffers = new GLuint[NUM_BUFFERS];
	
	if (!init) { initBufferProcs(); }

	glGenBuffers(NUM_BUFFERS, buffers);
}

Mesh::~Mesh(){
	glDeleteBuffers(NUM_BUFFERS,buffers);
	vertices = normals = texCoords = 0;
	buffers = 0;
	free(vertices);
	free(normals);
	free(texCoords);
	free(buffers);
}

void Mesh::allocateBuffers(){
	glBindBuffer(GL_ARRAY_BUFFER, buffers[DATA_BUFFER]);
	glBufferData(GL_ARRAY_BUFFER, num_verts*8*sizeof(GLfloat), 0, GL_STREAM_DRAW); //8 = xyz for verts + xyz for norms + uv for tex coords

	//face index data doesn't change, so it only has to be sent once
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[INDICES_BUFFER]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, num_faces*3*sizeof(GLuint), faces, GL_STATIC_DRAW);
}

void Mesh::update(const GLfloat &dt){
	//rebuffer the vertex, normal, and tex coord data
	glBindBuffer(GL_ARRAY_BUFFER, buffers[DATA_BUFFER]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, num_verts*3*sizeof(GLfloat), vertices);
	glBufferSubData(GL_ARRAY_BUFFER, num_verts*3*sizeof(GLfloat), num_verts*3*sizeof(GLfloat), normals);
	glBufferSubData(GL_ARRAY_BUFFER, num_verts*6*sizeof(GLfloat), num_verts*2*sizeof(GLfloat), texCoords);
}

void Mesh::draw(){
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texId);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[DATA_BUFFER]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[INDICES_BUFFER]);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, 0);
	glNormalPointer(GL_FLOAT, 0, (GLvoid*)((char*)NULL + num_verts*3*sizeof(GLfloat)));
	glTexCoordPointer(2, GL_FLOAT, 0, (GLvoid*)((char*)NULL + num_verts*6*sizeof(GLfloat)));

	glDrawElements(GL_TRIANGLES, num_faces*3, GL_UNSIGNED_INT, (GLuint*)0);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void Mesh::initBufferProcs(){
	glGenBuffers = (PFNGLGENBUFFERSARBPROC)wglGetProcAddress("glGenBuffersARB");
	glBindBuffer = (PFNGLBINDBUFFERARBPROC)wglGetProcAddress("glBindBufferARB");
	glBufferData = (PFNGLBUFFERDATAARBPROC)wglGetProcAddress("glBufferDataARB");
	glBufferSubData = (PFNGLBUFFERSUBDATAARBPROC)wglGetProcAddress("glBufferSubDataARB");
	glDeleteBuffers = (PFNGLDELETEBUFFERSARBPROC)wglGetProcAddress("glDeleteBuffersARB");
	init = true;
}