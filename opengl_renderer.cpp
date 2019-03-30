#include "opengl_renderer.h"
#include "gl_shader_loader.hpp"
#include "engine.h"


static const GLfloat g_vertex_buffer_data[] = {
   -0.5f, -0.5f, 0.0f,
   0.5f, -0.5f, 0.0f,
   0.0f,  0.5f, 0.0f,
};

static const float cube_vertex_data[] = {
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
	
	

	mesh_to_draw->model_matrix = glm::rotate(glm::mat4(1.0f), engine->get_time() * glm::radians(90.f), glm::vec3(0.0f, 0.0f, 1.0f));
	cube.SetLocation(-1,0,0);
	my_model.SetLocation(0,0,2);
	//glm::mat4 Model = glm::mat4(1.0f);
	// Our ModelViewProjection : multiplication of our 3 matrices
	mvp = main_camera.Projection * main_camera.View * mesh_to_draw->model_matrix;

	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);

}
void RendererGL::run(){
	main_camera.SetLocation(2.f,3.f,2.f);
	if(!my_model.load_model2("models/character.obj")){
		fprintf( stderr, "Failed to load 3d mesh\n" );
		return;
	}
	int size = sizeof(cube_vertex_data)/sizeof(cube_vertex_data[0]);
	std::cout << "size %i" << size << std::endl;
	int i = 0;
	for (i ; i < size; i++){
		glm::vec3 vertex;
		for (int o = 0; o < 3; o++){
			vertex = glm::vec3(cube_vertex_data[i],cube_vertex_data[i+1],cube_vertex_data[i+2]);
		}
		i = i + 2;
		cube.simple_vertices.push_back(vertex);

	}
	meshes.push_back(cube);
	meshes.push_back(my_model);

	init_ogl();	
	generate_mvp_matrix();
	main_loop();
}

void RendererGL::init_window(){
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
}


void RendererGL::draw(){

	for(int i = 0; i < meshes.size(); i++){
		mesh_to_draw = &meshes[i];
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, meshes[i].vertexbuffer);								
		glDrawArrays(GL_TRIANGLES, 0, meshes[i].simple_vertices.size()); 
		glDisableVertexAttribArray(0);

	}
		
	//glBindVertexArray(vertexbuffer);
	//glDrawElements(GL_TRIANGLES, mesh_to_draw->simple_vertices.size(), GL_UNSIGNED_SHORT, 0); 

	
}

void RendererGL::init_ogl(){
	mesh_to_draw = &cube;
	init_window();
	shadersID = LoadShaders("shaders/simple_vert_mvp.vert","shaders/simple_frag.frag");
	
	//generate vertex buffers for any mesh to load in real time
	{
		for(int i = 0; i < meshes.size(); i++){
			glGenVertexArrays(1, &meshes[i].VertexArrayID);
			glBindVertexArray(meshes[i].VertexArrayID);
			glGenBuffers(1, &meshes[i].vertexbuffer);
			glBindBuffer(GL_ARRAY_BUFFER, meshes[i].vertexbuffer);	
	
			glBufferData(GL_ARRAY_BUFFER, meshes[i].simple_vertices.size() * sizeof(glm::vec3),
					 &meshes[i].simple_vertices[0], GL_STATIC_DRAW);
		}
		
	}	

	

	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);
}

void RendererGL::main_loop(){
	//glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	 while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glClearColor(0.0f, 1.0f, .0f, 1.0f);

		glUseProgram(shadersID);

		update_matrix();
		draw();

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	 }
	

}

