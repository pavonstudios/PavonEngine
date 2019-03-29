#ifndef _GL_RENDERER_H_
#define _GL_RENDERER_H_


#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <algorithm>

#include <GL/glew.h>
#include "GLFW/glfw3.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


class RendererGL{
public:
	void run();
	bool bIsRunnning = false;
private:
	class GLFWwindow* window; // (In the accompanying source code, this variable is global for simplicity)
	void main_loop();
	void init_ogl();
	void draw_trigangle();
	void generate_mvp_matrix();
	void update_matrix();
	// This will identify our vertex buffer
	GLuint vertexbuffer;
	GLuint VertexArrayID;
	GLuint shadersID;
	GLuint MatrixID;
	glm::mat4 mvp;


};
#endif