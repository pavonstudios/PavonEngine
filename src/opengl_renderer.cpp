#include "opengl_renderer.h"
#include "gl_shader_loader.hpp"
#include "engine.h"
#include <glm/gtc/type_ptr.hpp>

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
static const float rect_vert[] = {
    // positions          // colors           // texture coords
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
};


void RendererGL::update_matrix(){
	
	UniformBufferObject ubo = {};

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3( 0.0f,  13.0f,  0.0f));
	mesh_to_draw->model_matrix = glm::rotate(model, engine->get_time() * glm::radians(90.f), glm::vec3(0.0f, 1.0f, 1.0f));


	ubo.view = engine->main_camera.View;
	ubo.proj = engine->main_camera.Projection;
	ubo.model = mesh_to_draw->model_matrix;

	mvp = engine->main_camera.Projection * engine->main_camera.View * mesh_to_draw->model_matrix;
	int modelLoc = glGetUniformLocation(shadersID, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mesh_to_draw->model_matrix));

	int viewLoc = glGetUniformLocation(shadersID, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(ubo.view));

	int projlLoc = glGetUniformLocation(shadersID, "projection");
	glUniformMatrix4fv(projlLoc, 1, GL_FALSE, glm::value_ptr(ubo.proj));
	//glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);

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
}

void RendererGL::init_ret(){
	unsigned int indices[] = {  
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rect_vert), rect_vert, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

	rect.vertexbuffer = VAO;

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	image_size size =  engine->objects_manager.load_and_get_size("textures/character.jpg");
	if(size.pPixels){
		std::cout << "image loaded" << std::endl;
	}
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,size.width, size.heigth,0,GL_RGB, GL_UNSIGNED_BYTE, size.pPixels);
	//glGenerateMipmap(GL_TEXTURE_2D);
	engine->objects_manager.free_image(size.pPixels);



}
void RendererGL::draw(){

	glBindVertexArray(rect.vertexbuffer);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	//glBindVertexArray(0);

	//glBindTexture(GL_TEXTURE_2D, texture);
	
	glUseProgram(shadersID);
	glUniform4f(glGetUniformLocation(shadersID, "mycolor"),1.f,0.f,0.f,1.0f);

	
	for(int i = 0; i < engine->meshes.size(); i++){
		glBindVertexArray(engine->meshes[i]->vertexbuffer);	
		
		
		//glBindVertexArray(engine->meshes[i]->vertexbuffer);
		//glBindBuffer(GL_ARRAY_BUFFER, engine->meshes[i]->vertexbuffer);	
		
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, engine->meshes[i]->indices_buffer);		
						
		//glDrawArrays(GL_TRIANGLES, 0, engine->meshes[i]->vertices.size());
		glDrawElements(GL_TRIANGLES, engine->meshes[i]->indices.size(), GL_UNSIGNED_INT,(void*)0);  
		

	}
	//draw ten objects
	for (unsigned int i = 0; i < 10; i++){
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, engine->objects_positions[i]);
		float angle = 20.0f * i; 
		model = glm::rotate(model, engine->get_time() *glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

		int modelLoc = glGetUniformLocation(shadersID, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glDrawArrays(GL_TRIANGLES,0,engine->meshes[0]->vertices.size());
	}	
}

void RendererGL::init_ogl(){
	mesh_to_draw = engine->meshes[0];
	init_ret();
	texture_and_attribure();
	
	shadersID = LoadShaders("shaders/simple_vert_mvp.vert","shaders/simple_frag.frag");
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);	
		
	//generate vertex buffers for any mesh to load in real time
	
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
		
	//;
}

void RendererGL::texture_and_attribure(){
		
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		sizeof(Vertex),                  // stride
		(void*)0            // array buffer offset
	);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		1,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		sizeof(Vertex),                  // stride
		(void*)offsetof(Vertex,color)            // array buffer offset
	);
	glEnableVertexAttribArray(1);
	
	glVertexAttribPointer(
		2,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		2,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		sizeof(Vertex),                  // stride
		(void*)offsetof(Vertex,texCoord)            // array buffer offset
	);
	glEnableVertexAttribArray(2);
	//glBindVertexArray(0);

	image_size size =  engine->objects_manager.load_and_get_size("textures/wall.jpg");
	if(size.pPixels){
		std::cout << "image loaded" << std::endl;
	}
	
	glGenTextures(1,&texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	float pixels[] = {
		0.0f, 0.0f, 0.0f,   1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,   0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,   1.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 0.0f,   1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,   0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f,   0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f,   0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,   1.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 0.0f,   1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,   0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,   1.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 0.0f,   1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,   0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f,   0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f,   0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,   1.0f, 1.0f, 1.0f,
	};
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,size.width, size.heigth,0,GL_RGB, GL_UNSIGNED_BYTE, size.pPixels);
	//glGenerateMipmap(GL_TEXTURE_2D);
	engine->objects_manager.free_image(size.pPixels);

}
void RendererGL::main_loop(){
	
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);		
		
		update_matrix();		
	
		draw();

}

