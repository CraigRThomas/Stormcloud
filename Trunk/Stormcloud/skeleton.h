#pragma once

#include <vector>
#include <cstdio>
#include <iostream>
#include <istream>
#include <fstream>
#include <string>
#include "matrix.h"
#include "vector3f.h"
#include "quaternion.h"

class Joint {
public:
	char name[256];
	unsigned int index;
	Joint* parent;
	Vector3f pos;
	Vector3f offset;
	Quaternion rotation;
	std::vector<Joint> children;
};

class Skeleton {
public:
	bool hasRun;
	bool hasUpdated;
	float rotation;
	Joint root;
	std::vector<float*> weights;
	std::vector<Quaternion> jointTransforms;
	std::vector<Quaternion> vertexTransforms;
	unsigned int numJoints;

	Skeleton(void);
	~Skeleton(void);
	void load(char* filePath, char* meshName);
	void update(void);
	void draw(void);
	void drawRecursive(Joint* j);
	Quaternion transformByIndex(const unsigned int &index);
private:
	void updateTransforms(Quaternion m, Joint* j);
	void toArray(std::vector<Matrix> &t, Joint* j);
};