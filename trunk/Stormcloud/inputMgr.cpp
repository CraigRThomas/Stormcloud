#include "inputMgr.h"

void InputManager::update(){
	for (unsigned int i=0;i<NUM_KEYS;i++){
		if (keysDown[i]){
			keys[i] = (keys[i])?2:1;
		} else {
			keys[i] = 0;
		}
	}
	for (unsigned int i=0;i<NUM_SPEC_KEYS;i++){
		if (specKeysDown[i]){
			specKeys[i] = (specKeys[i])?2:1;
		} else {
			specKeys[i] = 0;
		}
	}
}

void InputManager::mouseMove(int x, int y){
	mouse.px = mouse.x;
	mouse.py = mouse.y;
	mouse.x = x;
	mouse.y = y;
}

void InputManager::mousebutton(int button, int state, int x, int y){
	mouse.px = mouse.x;
	mouse.py = mouse.y;

	if (state == GLUT_DOWN){
		if (button == GLUT_LEFT_BUTTON){
			mouse.left_button = (mouse.left_button)?2:1;
		} else if (button == GLUT_RIGHT_BUTTON){
			mouse.right_button = (mouse.right_button)?2:1;
		} else if (button == GLUT_MIDDLE_BUTTON){
			mouse.middle_button = (mouse.middle_button)?2:1;
		}
	} else if (state == GLUT_UP){
		if (button == GLUT_LEFT_BUTTON){
			mouse.left_button = 0;
		} else if (button == GLUT_RIGHT_BUTTON){
			mouse.right_button = 0;
		} else if (button == GLUT_MIDDLE_BUTTON){
			mouse.middle_button = 0;
		}
	}
	mouse.x = x;
	mouse.y = y;
}

void InputManager::keyup(char k, int x, int y){
	keysDown[k] = 0;
}

void InputManager::keydown(char k, int x, int y){
	keysDown[k] = 1;
}

void InputManager::speckeyup(int k, int x, int y){
	if (k >= 100){
		k -= 100;
		k += 13;
	}
	specKeysDown[k] = 0;
}

void InputManager::speckeydown(int k, int x, int y){
	if (k >= 100){
		k -= 100;
		k += 13;
	}
	specKeysDown[k] = (specKeysDown[k])?2:1;
}

void InputManager::init(){
	for (unsigned int i=0;i<NUM_KEYS;i++){
		singleton->keys[i] = 0;
		singleton->keysDown[i] = 0;
	}
	for (unsigned int i=0;i<NUM_SPEC_KEYS;i++){
		singleton->specKeys[i] = 0;
		singleton->specKeysDown[i] = 0;
	}

	singleton->mouse.left_button = 0;
	singleton->mouse.right_button = 0;
	singleton->mouse.middle_button = 0;
	singleton->mouse.x = 0;
	singleton->mouse.y = 0;
	singleton->mouse.px = 0;
	singleton->mouse.py = 0;

	std::cout<<"Input Manager initialized.\n";
}

InputManager* InputManager::singleton = 0;

InputManager::InputManager(){}

InputManager::~InputManager(){ 
	delete singleton;
}

InputManager& InputManager::INMgr(){
	if (!singleton){
		singleton = new InputManager();
		init();
	}
	return *singleton;
}