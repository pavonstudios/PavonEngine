#include "opengl_renderer.h"
#include "gl_shader_loader.hpp"
#include "engine.h"

//using namespace engine;

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
	
	UniformBufferObject ubo = {};


	mesh_to_draw->model_matrix = glm::rotate(glm::mat4(1.0f), engine->get_time() * glm::radians(90.f), glm::vec3(0.0f, 0.0f, 1.0f));


	ubo.view = engine->main_camera.View;
	ubo.proj = engine->main_camera.Projection;
	ubo.model = mesh_to_draw->model_matrix;

	mvp = engine->main_camera.Projection * engine->main_camera.View * mesh_to_draw->model_matrix;

	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);

}
void RendererGL::run(){	
	
	int size = sizeof(cube_vertex_data)/sizeof(cube_vertex_data[0]);
	std::cout << "size %i" << size << std::endl;
	int i = 0;
	for (i ; i < size; i++){
		glm::vec3 vertex;
		for (int o = 0; o < 3; o++){
			vertex = glm::vec3(cube_vertex_data[i],cube_vertex_data[i+1],cube_vertex_data[i+2]);
		}
		i = i + 2;
		//cube.simple_vertices.push_back(vertex);

	}
	

	init_ogl();	
	generate_mvp_matrix();
	
}


void RendererGL::draw(){

	for(int i = 0; i < engine->meshes.size(); i++){
		mesh_to_draw = engine->meshes[i];
		
		//glBindVertexArray(engine->meshes[i]->vertexbuffer);
		//glBindBuffer(GL_ARRAY_BUFFER, engine->meshes[i]->vertexbuffer);	
			
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, engine->meshes[i]->indices_buffer);							
		//glDrawArrays(GL_TRIANGLES, 0, engine->meshes[i]->vertices.size());
		glDrawElements(GL_TRIANGLES, engine->meshes[i]->indices.size(), GL_UNSIGNED_INT,(void*)0);  
		

	}
		
	//glBindVertexArray(vertexbuffer);
	//glDrawElements(GL_TRIANGLES, mesh_to_draw->simple_vertices.size(), GL_UNSIGNED_SHORT, 0); 

	
}

void RendererGL::init_ogl(){
	mesh_to_draw = engine->meshes[0];
	
	shadersID = LoadShaders("shaders/simple_vert_mvp.vert","shaders/simple_frag.frag");
	
	//generate vertex buffers for any mesh to load in real time
	{
		for(int i = 0; i < engine->meshes.size(); i++){
			glGenVertexArrays(1, &engine->meshes[i]->VertexArrayID);

			glBindVertexArray(engine->meshes[i]->VertexArrayID);

			glGenBuffers(1, &engine->meshes[i]->vertexbuffer);
			glBindBuffer(GL_ARRAY_BUFFER, engine->meshes[i]->vertexbuffer);		
			glBufferData(GL_ARRAY_BUFFER, engine->meshes[i]->vertices.size() * sizeof(Vertex),
					 &engine->meshes[i]->vertices[0], GL_STATIC_DRAW);

			 glGenBuffers(1,&engine->meshes[i]->indices_buffer);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, engine->meshes[i]->indices_buffer);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, engine->meshes[i]->indices.size() * sizeof(unsigned int),
				&engine->meshes[i]->indices[0], GL_STATIC_DRAW); 
			
		}
		
	}	

	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		sizeof(Vertex),                  // stride
		(void*)0            // array buffer offset
	);
	//glBindVertexArray(0);
}

void RendererGL::main_loop(){
	
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glClearColor(0.0f, 1.0f, .0f, 1.0f);

		glUseProgram(shadersID);

		update_matrix();
		draw();

}

