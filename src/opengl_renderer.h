#ifndef _GL_RENDERER_H_
#define _GL_RENDERER_H_



#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <algorithm>

#ifndef  ANDROID
#include <GL/glew.h>
#else
#include "android_renderer.h"
#endif


#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "3D_objects.h"
#include "camera.h"

using namespace engine;


class RendererGL{
public:
	void run();
	bool bIsRunnning = false;
	class Engine* engine;
	void main_loop();
	void finish(){return;}
	
	
	
private:
	class GLFWwindow* window; 
	
	void init_ogl();
	
	void draw();
	void generate_mvp_matrix();
	void update_matrix();	
	

	GLuint shadersID;
	GLuint MatrixID;
	glm::mat4 mvp;

	Mesh *mesh_to_draw;

};
#endif