#pragma once

#define hard_mesh 0

#include <vector>
#include <cstdio>
#include <iostream>
#include <istream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <gl/glut.h>
#include <il/ilut.h>
#include <il/il.h>
#include <math.h>
#include "mesh.h"
#include "shader.h"
#include "shaderMgr.h"

class Object {
public:
	std::vector<Mesh*> meshes;		/**< Collection of drawable objects representing this object. */

	/** Constructor. */
	Object();
	/** Destructor. */
	~Object();
	/** Step function. */
	void update(const float & dt);
	/** Draw function. */
	void draw();
	/** Loads a .mesh file into meshes. */
	bool loadFromFile(char* path);
};