#pragma once

#define NUM_KEYS 256
#define NUM_SPEC_KEYS 22
#define INMGR_F1			1
#define INMGR_F2			2
#define INMGR_F3			3
#define INMGR_F4			4
#define INMGR_F5			5
#define INMGR_F6			6
#define INMGR_F7			7
#define INMGR_F8			8
#define INMGR_F9			9
#define INMGR_F10			10
#define INMGR_F11			11
#define INMGR_F12			12
#define INMGR_LEFT			13
#define INMGR_UP			14
#define INMGR_RIGHT			15
#define INMGR_DOWN			16
#define INMGR_PAGE_UP		17
#define INMGR_PAGE_DOWN		18
#define INMGR_HOME			19
#define INMGR_END			20
#define INMGR_INSERT		21
#define InputMgr InputManager::INMgr()

#include <iostream>
#include <gl/glut.h>

struct Mouse {
	unsigned short left_button;		/**< Button state for left mouse button. */
	unsigned short right_button;	/**< Button state for right mouse button. */
	unsigned short middle_button;   /**< Button state for middle mouse button. */
	GLuint x;						/**< Pointer X-coord. */
	GLuint y;						/**< Pointer Y-coord. */
	GLuint px;						/**< Previous X-coord. */
	GLuint py;						/**< Previous Y-coord. */
};

class InputManager {
public:
	unsigned short keys[NUM_KEYS];			/**< State of GLUT's "regular" keys. */
	unsigned short specKeys[NUM_SPEC_KEYS]; /**< State of GLUT's "special" keys. */
	Mouse mouse;							/**< State of the mouse. */

	/** Keyup callback for regular keys. */
	void keyup(char k, int x, int y);
	/** Keyup callback for special keys. */
	void speckeyup(int k, int x, int y);
	/** Keydown callback for regular keys. */
	void keydown(char k, int x, int y);
	/** Keydown callback for special keys. */
	void speckeydown(int k, int x, int y);
	/** Callback for mouse button presses. */
	void mousebutton(int button, int state, int x, int y);
	/** Callback for mouse movement. */
	void mouseMove(int x, int y);
	/** Updates current state sets. */
	void update();
	/** Singleton constructor. */
	static InputManager& INMgr();
protected:
	/** Constructor. */
	InputManager();
	/** Destructor. */
	~InputManager();
private:
	bool keysDown[NUM_KEYS];			/**< Binary up or down state for regular keys. */
	bool specKeysDown[NUM_SPEC_KEYS];	/**< Binary up or down state for special keys. */
	static InputManager* singleton;		/**< Singleton pointer. */
	/** Singleton initialization function. */
	static void init();		
};