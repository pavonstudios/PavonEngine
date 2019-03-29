#include "opengl_renderer.h"
#include "gl_shader_loader.hpp"
#include "engine.h"


static const GLfloat g_vertex_buffer_data[] = {
   -0.5f, -0.5f, 0.0f,
   0.5f, -0.5f, 0.0f,
   0.0f,  0.5f, 0.0f,
};

static const GLfloat cube_vertex_data[] = {
    -1.0f,-1.0f,-1.0f, // triangle 1 : begin
    -1.0f,-1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f, // triangle 1 : end
    1.0f, 1.0f,-1.0f, // triangle 2 : begin
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f, // triangle 2 : end
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f
};


void RendererGL::generate_mvp_matrix(){

	MatrixID = glGetUniformLocation(shadersID, "MVP");

}
void RendererGL::update_matrix(){
	
	

	glm::mat4 Model = glm::rotate(glm::mat4(1.0f), engine->get_time() * glm::radians(90.f), glm::vec3(0.0f, 0.0f, 1.0f));

	//glm::mat4 Model = glm::mat4(1.0f);
	// Our ModelViewProjection : multiplication of our 3 matrices
	mvp = main_camera.Projection * main_camera.View * Model; // Remember, matrix multiplication is the other way around

	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);

}
void RendererGL::run(){
	main_camera.SetLocation(2.f,3.f,2.f);
	init_ogl();	
	generate_mvp_matrix();
	main_loop();
}

void RendererGL::init_ogl(){
	glewExperimental = true; // Needed for core profile
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		return;
	}
	glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL 

	// Open a window and create its OpenGL context
	
	window = glfwCreateWindow( 800, 600, "OpenGL", NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		glfwTerminate();
		return;
	}
		glfwMakeContextCurrent(window); // Initialize GLEW
	

	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return;
	}
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	glGenBuffers(1, &vertexbuffer);

	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);

	shadersID = LoadShaders("shaders/simple_vert_mvp.vert","shaders/simple_frag.frag");
}
void RendererGL::main_loop(){
	//glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	 while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glClearColor(0.0f, 1.0f, 0.0f, 1.0f);

		glUseProgram(shadersID);

		update_matrix();
		draw_trigangle();

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	 }
	

}

void RendererGL::draw_trigangle(){



	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertex_data), cube_vertex_data, GL_STATIC_DRAW);


	glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);
	// Draw the triangle !
	glDrawArrays(GL_TRIANGLES, 0, 12*3); // Starting from vertex 0; 3 vertices total -> 1 triangle
	glDisableVertexAttribArray(0);
}