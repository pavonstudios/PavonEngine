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


#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "3D_objects.h"
#include "camera.h"


class RendererGL{
public:
	void run();
	bool bIsRunnning = false;
	class Engine* engine;
private:
	class GLFWwindow* window; 
	void main_loop();
	void init_ogl();
	void init_window();
	void draw_trigangle();
	void generate_mvp_matrix();
	void update_matrix();

	
	
	std::vector<Mesh> meshes;
	GLuint shadersID;
	GLuint MatrixID;
	glm::mat4 mvp;

	Mesh my_model;
	Mesh cube;

	Camera main_camera;

	Mesh *mesh_to_draw;

};
#endif