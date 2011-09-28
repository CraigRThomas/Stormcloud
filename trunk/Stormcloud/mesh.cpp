#include "mesh.h"

bool Mesh::init = false;
PFNGLGENBUFFERSARBPROC		Mesh::glGenBuffers		= NULL;
PFNGLBINDBUFFERARBPROC		Mesh::glBindBuffer		= NULL;
PFNGLBUFFERDATAARBPROC		Mesh::glBufferData		= NULL;
PFNGLBUFFERSUBDATAARBPROC	Mesh::glBufferSubData	= NULL;
PFNGLDELETEBUFFERSARBPROC	Mesh::glDeleteBuffers	= NULL;
PFNGLACTIVETEXTUREARBPROC	Mesh::glActiveTexture	= NULL;
PFNGLVERTEXATTRIBPOINTERARBPROC Mesh::glVertexAttribPointer = NULL;
PFNGLENABLEVERTEXATTRIBARRAYARBPROC Mesh::glEnableVertexAttribArray = NULL;

Mesh::Mesh(){
	vertices = normals = texCoords = 0;
	texId = normMapId = 0;
	vertShader = fragShader = 0;
	for (int i=0;i<3;i++){
		mat.diffuse[i] = 0.f;
		mat.ambient[i] = 1.f;
		mat.specular[i] = 0.f;
	}
	mat.diffuse[3] = 1.f;
	mat.ambient[3] = 1.f;
	mat.specular[3] = 1.f;
	mat.shininess = 0; //must be 1-128; Outside of range = no shininess
	normMapped = false;
	transform = Matrix(4,4);

	if (!init) { initBufferProcs(); }
	buffers = new GLuint[NUM_BUFFERS];
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
	glBindBuffer(GL_ARRAY_BUFFER, buffers[VBO]);
	glBufferData(GL_ARRAY_BUFFER, num_verts*3*sizeof(GLfloat), vertices, GL_STATIC_DRAW); //8 = xyz for verts + xyz for norms + uv for tex coords
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[IBO]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, num_faces*3*sizeof(GLuint), faces, GL_STATIC_DRAW);
}

void Mesh::update(const GLfloat &dt){

}

void Mesh::draw(Matrix &t){

	if (normMapped && vertShader){
		/*calcTBNs();
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
		delete tangents;
		delete bitangents;
		delete normals;*/
	}

	if (vertShader){
		vertShader->setAttributeArraySize(num_verts,0);
		vertShader->send("vPosition",vertices);
		vertShader->send("vNormal",normals);
		vertShader->send("vTexCoord",texCoords);
		float data[4] = {0.f,0.f,5.f,1.f};
		vertShader->send("lightPos",data);
		float *transformData = vertShader->getFloat("transform");
		t.getData(transformData);

		ShaderMgr.push(vertShader);
	}
	if (fragShader){
		fragShader->send("matAmbient",mat.ambient);
		fragShader->send("matDiffuse",mat.diffuse);
		fragShader->send("matSpecular",mat.specular);
		fragShader->send("matShininess",mat.shininess);
		float data[4] = {1.f,1.f,1.f,1.f};
		fragShader->send("lightAmbient",data);
		fragShader->send("lightDiffuse",data);
		fragShader->send("lightSpecular",data);
		ShaderMgr.push(fragShader);
	}
	ShaderMgr.checkInvalid();
	ShaderMgr.update();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texId);

	if (normMapped){
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, normMapId);
	}

	glBindBuffer(GL_ARRAY_BUFFER,buffers[VBO]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); 
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[IBO]);
	glDrawElements(GL_TRIANGLES, num_faces*3, GL_UNSIGNED_INT, (GLuint*)0);
}

void Mesh::initBufferProcs(){
	glGenBuffers = (PFNGLGENBUFFERSARBPROC)wglGetProcAddress("glGenBuffersARB");
	glBindBuffer = (PFNGLBINDBUFFERARBPROC)wglGetProcAddress("glBindBufferARB");
	glBufferData = (PFNGLBUFFERDATAARBPROC)wglGetProcAddress("glBufferDataARB");
	glBufferSubData = (PFNGLBUFFERSUBDATAARBPROC)wglGetProcAddress("glBufferSubDataARB");
	glDeleteBuffers = (PFNGLDELETEBUFFERSARBPROC)wglGetProcAddress("glDeleteBuffersARB");
	glActiveTexture = (PFNGLACTIVETEXTUREARBPROC)wglGetProcAddress("glActiveTextureARB");
	glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERARBPROC)wglGetProcAddress("glVertexAttribPointerARB");
	glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYARBPROC)wglGetProcAddress("glEnableVertexAttribArrayARB");
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