#pragma once
#ifndef _Engine_H_
#define _Engine_H_

#define GLM_FORCE_SWIZZLE


#ifdef VULKAN
    #include "renderers/vulkan.hpp"
    #define GLFW_INCLUDE_VULKAN
    #include "VulkanData.hpp"
    #include <GLFW/glfw3.h>
#endif    

#if defined (ES2)
    #include "renderers/opengl.hpp"
#endif

#if defined (DX11)
    #include "renderers/directx.hpp"
#endif

#include <chrono>
#include <vector>

#include "input.h"
#include "asset_manager.h"
#include "WindowManager.hpp"
#include "objects.h"
#include "camera.h"
#include "game_map.hpp"
#include "audio_manager.hpp"
class Game;

#include "gui.hpp"
#include "animation_manager.hpp"

#ifdef LINUX
#include  <sys/time.h>
typedef std::chrono::time_point<std::chrono::system_clock> TTime;
#endif
#ifdef WINDOWS
typedef std::chrono::time_point<std::chrono::steady_clock> TTime;
#endif // DEBUG
class ConnectionManager;

using namespace engine;
#include "engine_component.hpp"
class Engine {
  
public:
	Engine();
	std::vector<EngineComponent*> components;
	void setup_components();
	EngineComponent* component_by_name(const char* name);

	Game* game = nullptr;

    Renderer renderer; 
    WindowManager window_manager;
	MeshManager mesh_manager;
    Camera main_camera;
    AssetManager assets;
	MapManager maps;
	TexturesManager textures_manager;
#ifdef LINUX
	AudioManager audio_manager;
	ConnectionManager* net_manager;
#endif // LINUX


	AnimationManager animation_manager;

	void draw_loading_screen();
    void update_mvp(EMesh* mesh);

    bool ready_to_game = false;
    bool loading = true;
    bool play_animations = false;
    Input input;
    bool edit_mode = false;
    

    float deltaTime = 0.0f;	// Time between current frame and last frame

    bool draw_gizmos = false;

	float lastFrame = 0.0f; // Time of last frame

	int frames = 0;
	float fps = 0;
	int last_fps = 0;
	float frame_time = 0;
	
	int  num_frames = 0;
	float LIMIT_FPS = 8;

	std::vector<EMesh*> meshes;//mesh to draw
	std::vector<EMesh*> linear_meshes;//loaded mesh 
	std::vector<EMesh*> unique_meshes;
	TranslationUpdate tranlation_update;
    
    std::vector<EMesh*> skeletal_meshes;//loaded mesh

    std::vector<EMesh*> helpers;
    std::vector<EMesh*> colliders_draw_mesh;  

    std::vector<Model*> models_to_draw;

	void es2_draw_frame();
	void loop_data();

	void distance_object_from_camera();

	void init();
	void translate_mesh(EMesh* mesh, uint direction, float value);
	float get_time();


	
	void update_render_size();

	void configure_window_callback();
	void main_loop();

	void delete_meshes();
	
#ifdef LINUX
	struct timezone  tz;
	timeval  t1, t2;
#endif

#ifdef DEVELOPMENT
	void print_vector(glm::vec3);
	void print_debug(const std::string text, int8_t posx, int8_t posy);
	void print_fps();
	void calculate_time(std::string, TTime);
	void calculate_fps(TTime);
#endif 

	void init_collision_engine();

    #ifdef ES2 
    std::vector<GLuint> texture_ids;
    #endif

    #ifdef ANDROID
        Engine(android_app * pApp);
        android_app * pAndroid_app;
    #endif
    #ifdef VULKAN        
        VulkanData vkdata = {VK_NULL_HANDLE};
        vks::VulkanDevice* vulkan_device = nullptr; 
        void vulkan_loop();     
      
  
    #endif    
#if defined (VULKAN) || defined (DX11)

		GLFWwindow* window;

#endif
    

    void init_renderer();
	void update_window_size();
    void load_and_assing_location(std::string path, glm::vec3 location);
    void load_and_assing_location(struct MapDataToLoad data);
      
   
   

    
};
#endif
