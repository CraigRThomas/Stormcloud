#include "mesh.h"

bool Mesh::init = false;
PFNGLGENBUFFERSARBPROC		Mesh::glGenBuffers		= NULL;
PFNGLBINDBUFFERARBPROC		Mesh::glBindBuffer		= NULL;
PFNGLBUFFERDATAARBPROC		Mesh::glBufferData		= NULL;
PFNGLBUFFERSUBDATAARBPROC	Mesh::glBufferSubData	= NULL;
PFNGLDELETEBUFFERSARBPROC	Mesh::glDeleteBuffers	= NULL;
PFNGLACTIVETEXTUREARBPROC	Mesh::glActiveTexture	= NULL;

Mesh::Mesh(){
	vertices = normals = texCoords = 0;
	texId = normMapId = 0;
	vertShader = fragShader = 0;
	buffers = new GLuint[NUM_BUFFERS];
	for (int i=0;i<3;i++){
		mat.diffuse[i] = 0.f;
		mat.ambient[i] = 1.f;
		mat.specular[i] = 0.f;
	}
	mat.diffuse[3] = 1.f;
	mat.ambient[3] = 1.f;
	mat.specular[3] = 1.f;
	mat.shininess = 0; //must be 0-128; both 0 and 128 result in no shine; 1 is shiniest
	normMapped = false;
	
	if (!init) { initBufferProcs(); }

	glGenBuffers(NUM_BUFFERS, buffers);
}

Mesh::~Mesh(){
	glDeleteBuffers(NUM_BUFFERS,buffers);
	delete vertices;
	vertices = NULL;
	delete normals;
	normals = NULL;
	delete texCoords;
	texCoords = NULL;
	delete [] buffers;
	buffers = NULL;
	delete vertShader;
	vertShader = NULL;
	delete fragShader;
	fragShader = NULL;
}

void Mesh::allocateBuffers(){
	glBindBuffer(GL_ARRAY_BUFFER, buffers[DATA_BUFFER]);
	glBufferData(GL_ARRAY_BUFFER, num_verts*8*sizeof(GLfloat), 0, GL_STREAM_DRAW); //8 = xyz for verts + xyz for norms + uv for tex coords

	//face index data doesn't change, so it only has to be sent once
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[INDICES_BUFFER]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, num_faces*3*sizeof(GLuint), faces, GL_STATIC_DRAW);
}

void Mesh::update(const GLfloat &dt){
	
}

void Mesh::draw(){

	//rebuffer the vertex, normal, and tex coord data
	glBindBuffer(GL_ARRAY_BUFFER, buffers[DATA_BUFFER]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, num_verts*3*sizeof(GLfloat), vertices);
	glBufferSubData(GL_ARRAY_BUFFER, num_verts*3*sizeof(GLfloat), num_verts*3*sizeof(GLfloat), normals);
	glBufferSubData(GL_ARRAY_BUFFER, num_verts*6*sizeof(GLfloat), num_verts*2*sizeof(GLfloat), texCoords);

	if (normMapped && vertShader){
		calcTBNs();
		float *tangents = vertShader->getFloat("tangent");
		float *bitangents = vertShader->getFloat("bitangent");
		float *normals = vertShader->getFloat("vNormal");

		for (unsigned int i=0;i<num_faces;i++){
			tangents[i*3] = TBNs[i](0,0);
			tangents[i*3+1] = TBNs[i](0,1);
			tangents[i*3+2] = TBNs[i](0,2);
			//std::cout<<tangents[i*3]<<" "<<tangents[i*3+1]<<" "<<tangents[i*3+2]<<"\n";

			bitangents[i*3] = TBNs[i](1,0);
			bitangents[i*3+1] = TBNs[i](1,1);
			bitangents[i*3+2] = TBNs[i](1,2);
			//std::cout<<bitangents[i*3]<<" "<<bitangents[i*3+1]<<" "<<bitangents[i*3+2]<<"\n";

			normals[i*3] = TBNs[i](2,0);
			normals[i*3+1] = TBNs[i](2,1);
			normals[i*3+2] = TBNs[i](2,2);
			//std::cout<<normals[i*3]<<" "<<normals[i*3+1]<<" "<<normals[i*3+2]<<"\n";
		}

		tangents = bitangents = normals = 0;
		free(tangents);
		free(bitangents);
		free(normals);
	}

	if (vertShader){
		ShaderMgr.push(vertShader);
	}
	if (fragShader){
		ShaderMgr.push(fragShader);
	}
	ShaderMgr.checkInvalid();
	ShaderMgr.update();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texId);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, normMapId);

	glMaterialfv(GL_FRONT,GL_AMBIENT,mat.ambient);
	glMaterialfv(GL_FRONT,GL_DIFFUSE,mat.diffuse);
	glMaterialfv(GL_FRONT,GL_SPECULAR, mat.specular);
	glMateriali (GL_FRONT,GL_SHININESS,mat.shininess);

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
	glActiveTexture = (PFNGLACTIVETEXTUREARBPROC)wglGetProcAddress("glActiveTextureARB");
	init = true;
}

void Mesh::calcTBNs(){
	Vector3f q1, q2, t, b, n, BxN, NxT, TxB;
	int p1, p2, p3;
	float s1, s2, t1, t2;
	float denominator, scale;
	for (unsigned int i=0;i<num_faces;i++){
		p1 = faces[i*3];
		p2 = faces[i*3+1];
		p3 = faces[i*3+2];

		q1 = Vector3f(vertices[p2*3], vertices[p2*3+1], vertices[p2*3+2]) - Vector3f(vertices[p1*3], vertices[p1*3+1], vertices[p1*3+2]);
		q2 = Vector3f(vertices[p3*3], vertices[p3*3+1], vertices[p3*3+2]) - Vector3f(vertices[p1*3], vertices[p1*3+1], vertices[p1*3+2]);

		s1 = texCoords[p2*2] - texCoords[p1*2];
		s2 = texCoords[p3*2] - texCoords[p1*2];
		t1 = texCoords[p2*2+1] - texCoords[p1*2+1];
		t2 = texCoords[p3*2+1] - texCoords[p1*2+1];

		denominator = (s1 * t2) - (s2 * t1);
		if (denominator == 0.0f){
			TBNs[i] = Matrix(3,3);
		} else {
			scale = 1.f / denominator;

			t = Vector3f(q1*t2 - q2*t1) * scale;
			b = Vector3f(q1*(-s2) + q2*s1) * scale;
			n = cross(t,b);

			TBNs[i].populate(0,0,t.x);
			TBNs[i].populate(0,1,t.y);
			TBNs[i].populate(0,2,t.z);

			TBNs[i].populate(1,0,b.x);
			TBNs[i].populate(1,1,b.y);
			TBNs[i].populate(1,2,b.z);

			TBNs[i].populate(2,0,n.x);
			TBNs[i].populate(2,1,n.y);
			TBNs[i].populate(2,2,n.z);
		}
	}
}