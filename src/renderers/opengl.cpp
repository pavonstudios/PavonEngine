#include "../engine.h"
#include "opengl.hpp"


#ifdef WINDOWS
#pragma warning(disable:4996)
#endif // WINDOWS


GLuint Renderer::load_shader(shader_src &shader_data, GLenum type)
{
	const char *shader_source;
	if (type == GL_VERTEX_SHADER)
	{
		shader_source = shader_data.vertex_code;
		shader_data.type = 0;
	}
	if (type == GL_FRAGMENT_SHADER)
	{
		shader_source = shader_data.fragment_code;
		shader_data.type = 1;
	}

	GLuint shader = glCreateShader(type);

	glShaderSource(shader, 1, &shader_source, NULL);
	glCompileShader(shader);

	print_shader_info_log(shader, shader_data, shader_source);

	return shader;
}

void Renderer::print_shader_info_log(GLuint shader, shader_src &shader_data, const char *src)
{

	GLint length;

	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

	if (length)
	{
		char *buffer = new char[length];
		glGetShaderInfoLog(shader, length, NULL, buffer);
#ifdef ANDROID
		LOGW("------------->SHADER info %s ", buffer);
#else
		cout << "SHADER info: " << buffer << flush;
#endif
		delete[] buffer;

		GLint success;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (success != GL_TRUE)
		{

#ifdef ANDROID
			LOGW("ERROR in shader loader");
			if (shader_data.type == 0)
			{
				std::cout << "Error in file " << shader_data.vertex_shader_path << std::endl;
				LOGW("ERROR in file %s", shader_data.vertex_shader_path.c_str());
			}
			if (shader_data.type == 1)
			{
				std::cout << "Error in file " << shader_data.fragment_shader_path << std::endl;
				LOGW("ERROR in file %s", shader_data.fragment_shader_path.c_str());
			}
#endif
#ifdef ES2
			if (shader_data.type == 0)
			{
				std::cout << "Error in file " << shader_data.vertex_shader_path << std::endl;
			}
			if (shader_data.type == 1)
			{
				std::cout << "Error in file " << shader_data.fragment_shader_path << std::endl;
			}
			exit(1);
#endif
		}
	}
}

char *Renderer::load_shader_file(const char *path)
{
#ifdef ANDROID
	AAsset *file = AAssetManager_open(app->activity->assetManager, path, AASSET_MODE_BUFFER);

	size_t file_length = AAsset_getLength(file);
	char *fileContent = new char[file_length + 1];

	AAsset_read(file, fileContent, file_length);
	AAsset_close(file);
	return fileContent;
#else
	std::string spath = path;
	std::string content;
	std::ifstream fileStream(spath, std::ios::in);

	if (!fileStream.is_open())
	{
		std::string error = "Could not read file " + spath;
		throw std::runtime_error(error);
	}

	std::string line = "";
	while (!fileStream.eof())
	{
		std::getline(fileStream, line);
		content.append(line + "\n");
	}

	fileStream.close();
	int n = content.length();
	char *char_content = new char[n + 1];
	strcpy(char_content, content.c_str());

	return char_content;
#endif
}

void Renderer::load_shaders(EMesh *mesh)
{

	char *vertex_shader_src = load_shader_file(mesh->data_shader.vertex_shader_path.c_str());
	char *fragment_shader_src = load_shader_file(mesh->data_shader.fragment_shader_path.c_str());

	shader_src shader;
	shader.vertex_shader_path = mesh->data_shader.vertex_shader_path;
	shader.fragment_shader_path = mesh->data_shader.fragment_shader_path;
	shader.vertex_code = vertex_shader_src;
	shader.fragment_code = fragment_shader_src;

	GLuint vertexShader = load_shader(shader, GL_VERTEX_SHADER);	 // load vertex shader
	GLuint fragmentShader = load_shader(shader, GL_FRAGMENT_SHADER); // load fragment shader

	mesh->shader_program = glCreateProgram();			  // create program object
	glAttachShader(mesh->shader_program, vertexShader);   // and attach both...
	glAttachShader(mesh->shader_program, fragmentShader); // ... shaders to it

	glLinkProgram(mesh->shader_program); // link the program
}

void Renderer::load_shaders(const std::vector<EMesh *> &meshes)
{
	for (EMesh *mesh : meshes)
	{
		char *vertex_shader_src = load_shader_file(mesh->data_shader.vertex_shader_path.c_str());
		char *fragment_shader_src = load_shader_file(mesh->data_shader.fragment_shader_path.c_str());

		shader_src shader;
		shader.vertex_shader_path = mesh->data_shader.vertex_shader_path;
		shader.fragment_shader_path = mesh->data_shader.fragment_shader_path;
		shader.vertex_code = vertex_shader_src;
		shader.fragment_code = fragment_shader_src;

		GLuint vertexShader = load_shader(shader, GL_VERTEX_SHADER);	 // load vertex shader
		GLuint fragmentShader = load_shader(shader, GL_FRAGMENT_SHADER); // load fragment shader

		mesh->shader_program = glCreateProgram();			  // create program object
		glAttachShader(mesh->shader_program, vertexShader);   // and attach both...
		glAttachShader(mesh->shader_program, fragmentShader); // ... shaders to it

		glLinkProgram(mesh->shader_program); // link the program
	}
}

void Renderer::init_gl(){
	

	glClearColor(1, 0.0, 0.0, 1.0);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
}

void Renderer::load_mesh_texture(EMesh *mesh)
{

	glGenTextures(1, &mesh->texture_id);
	glBindTexture(GL_TEXTURE_2D, mesh->texture_id);

	AssetManager assets;
#ifdef ANDROID
	float texture[]{
		0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f};
	Image size = assets.load_bmp("textures/GUI/white_logo.jpg", app->activity->assetManager);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	GLenum error;
	error = glGetError();
	Image sdf;
#else
	Image size;
	if (mesh->texture.hasTexture)
	{
		size.heigth = mesh->texture.height;
		size.width = mesh->texture.width;
		size.data = mesh->texture.data;
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.width, size.heigth, 0, GL_RGBA, GL_UNSIGNED_BYTE, size.data);
	}
	else
	{
		size = assets.load_and_get_size(mesh->texture_path.c_str());
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size.width, size.heigth, 0, GL_RGB, GL_UNSIGNED_BYTE, size.data);
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
#endif
}
void Renderer::load_textures(const std::vector<EMesh *> &meshes)
{
	GLuint texture_id;
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);

	for (EMesh *mesh : meshes)
	{
		if (mesh->texture.texture_id == -1)
		{

			glGenTextures(1, &mesh->texture_id);
			glBindTexture(GL_TEXTURE_2D, mesh->texture_id);

		
#ifndef ANDROID

			Image size;
			if (mesh->texture.hasTexture)
			{
				size.heigth = mesh->texture.height;
				size.width = mesh->texture.width;
				size.data = mesh->texture.data;
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.width, size.heigth, 0, GL_RGBA, GL_UNSIGNED_BYTE, size.data);
			}
			else
			{
				if (mesh->texture_path != "")
				{
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mesh->image.width, mesh->image.heigth, 0, GL_RGB, GL_UNSIGNED_BYTE, mesh->image.data);
				}
			}

#endif
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			//glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			mesh->texture_id = textures_ids[mesh->texture.texture_id];
		}
	}
}

void Renderer::load_textures(std::vector<std::string> &textures_paths)
{

	for (auto path : textures_paths)
	{
		GLuint texture_id;
		glGenTextures(1, &texture_id);
		glBindTexture(GL_TEXTURE_2D, texture_id);

		AssetManager assets;
#ifdef ANDROID

		Image size = assets.load_bmp(assets.path(path).c_str(), app->activity->assetManager); //TODO: load texture with android path
#else
		Image size;

		size = assets.load_and_get_size(assets.path(path).c_str());

#endif
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size.width, size.heigth, 0, GL_RGB, GL_UNSIGNED_BYTE, size.data);
		//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		textures_ids.push_back(texture_id);
	}
}

void Renderer::activate_vertex_attributes(EMesh *mesh)
{

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertex_buffer);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), (void *)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(Vertex), (void *)offsetof(Vertex, color));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(Vertex), (void *)offsetof(Vertex, texCoord));

	if (mesh->type == MESH_TYPE_SKINNED)
	{
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, false, sizeof(Vertex), (void *)offsetof(Vertex, joint0));

		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, false, sizeof(Vertex), (void *)offsetof(Vertex, weight0));
	}
}

void Renderer::update_mvp(EMesh *mesh)
{

	if (mesh->type == MESH_TYPE_SKINNED)
	{
		GLint view = glGetUniformLocation(mesh->shader_program, "view");
		GLint model = glGetUniformLocation(mesh->shader_program, "model");
		GLint proj = glGetUniformLocation(mesh->shader_program, "projection");
		GLint joints_matrix_id = glGetUniformLocation(mesh->shader_program, "joint_matrix");

		glUniformMatrix4fv(view, 1, GL_FALSE, &mesh->ubo.view[0][0]);
		glUniformMatrix4fv(model, 1, GL_FALSE, &mesh->ubo.model[0][0]);
		glUniformMatrix4fv(proj, 1, GL_FALSE, &mesh->ubo.proj[0][0]);

		glUniformMatrix4fv(joints_matrix_id, mesh->node_uniform.joint_count, GL_FALSE, &mesh->node_uniform.joint_matrix[0][0][0]);
	}
	else
	{
		glUniformMatrix4fv(0, 1, GL_FALSE, &mesh->MVP[0][0]);
	}
}

void Renderer::draw_mesh()
{
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 3);
}

void Renderer::draw_gui(EMesh *mesh)
{
	// int samplerid = glGetUniformLocation(shaderProgram, "texture_sampler");
	//  glUniform1i(samplerid, 0);

	glBindTexture(GL_TEXTURE_2D, mesh->texture_id);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertex_buffer);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, mesh->vertices.size());
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Renderer::draw(EMesh *mesh)
{

	glBindTexture(GL_TEXTURE_2D, mesh->texture_id);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertex_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indices_buffer);
	glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, (void *)0);
}


void Renderer::create_buffers(Engine* engine, const std::vector<EMesh*>& meshes){
	for(EMesh* mesh : meshes){
        
            if(mesh->model_id == -1){
                glGenBuffers(1,&mesh->vertex_buffer);
                glBindBuffer(GL_ARRAY_BUFFER,mesh->vertex_buffer);
                glBufferData(GL_ARRAY_BUFFER,mesh->vertices.size() * sizeof(Vertex),mesh->vertices.data(),GL_STATIC_DRAW);

                if(mesh->indices.size() > 0){
                        glGenBuffers(1,&mesh->indices_buffer);
                        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,mesh->indices_buffer);
                        glBufferData(GL_ELEMENT_ARRAY_BUFFER,mesh->indices.size() * sizeof(unsigned int),mesh->indices.data(), GL_STATIC_DRAW);

                }
                
                glBindBuffer(GL_ARRAY_BUFFER,0);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
            }
            else{
                mesh->vertex_buffer = engine->unique_meshes[0]->vertex_buffer;
                mesh->indices_buffer = engine->unique_meshes[0]->indices_buffer;

            }

	}
}

void Renderer::create_buffer(EMesh* mesh){
	   
	glGenBuffers(1,&mesh->vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER,mesh->vertex_buffer);
	
	if(mesh->type == MESH_TYPE_SKINNED)
	glBufferData(GL_ARRAY_BUFFER,mesh->vertices.size() * sizeof(Vertex),mesh->vertices.data(),GL_DYNAMIC_DRAW);
	else
	glBufferData(GL_ARRAY_BUFFER,mesh->vertices.size() * sizeof(Vertex),mesh->vertices.data(),GL_STATIC_DRAW);

	if(mesh->indices.size() > 0){
			glGenBuffers(1,&mesh->indices_buffer);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,mesh->indices_buffer);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER,mesh->indices.size() * sizeof(unsigned int),mesh->indices.data(), GL_STATIC_DRAW);

	}
	
	glBindBuffer(GL_ARRAY_BUFFER,0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

   
}
#ifdef DEVELOPMENT
void Renderer::draw_gizmos(Engine* engine, std::vector<EMesh*> colliders_draw_mesh, std::vector<EMesh*> helpers){
	glClear(GL_DEPTH_BUFFER_BIT);
		for (EMesh *mesh : colliders_draw_mesh)
		{
			glUseProgram(mesh->shader_program);
			activate_vertex_attributes(mesh);
			update_mvp(mesh);
			glBindBuffer(GL_ARRAY_BUFFER, mesh->vertex_buffer);
			glDrawArrays(GL_POINTS, 0, mesh->vertices.size());
			glLineWidth(3);
			glDrawArrays(GL_LINES, 0, mesh->vertices.size());
		}

		glClear(GL_DEPTH_BUFFER_BIT);
		for (auto *mesh : helpers)
		{
			SkeletalManager::update_joint_vertices_data(engine);
			glUseProgram(mesh->shader_program);
			activate_vertex_attributes(mesh);

			update_mvp(mesh);
			glBindBuffer(GL_ARRAY_BUFFER, mesh->vertex_buffer);
			glDrawArrays(GL_POINTS, 0, mesh->vertices.size());
			glLineWidth(3);
			//glDrawArrays(  GL_LINES,0,mesh->vertices.size());
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indices_buffer);
			glDrawElements(GL_LINES, mesh->indices.size(), GL_UNSIGNED_INT, (void *)0);
		}
}
#endif
void Renderer::use_shader_from(EMesh* mesh){
	glUseProgram(mesh->shader_program);
}