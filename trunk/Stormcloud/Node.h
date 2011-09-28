#pragma once

#include <string>

class Node{
public:
	Node(void);
	~Node(void);
	std::string getName(void);
private:
	std::string name;

};