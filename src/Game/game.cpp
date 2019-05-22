#include "game.hpp"
#include "weapon.hpp"
#include "../Multiplayer/connectivity.hpp"
void Game::init_player(){

		player = new ThirdPerson();		
		//player = new Vehicle();

		player->engine = this->engine;	
		player->mesh = nullptr;
		player->camera_position = vec3(-0.4,-1.7,-2.8);
		if(this->player_id == -1){
			std::runtime_error("no player assigned from map file");
		}else{
				
			player->mesh = engine->meshes[this->player_id];
		}		
		
		if(player->mesh == nullptr){
			std::runtime_error("no player mesh pointer assigned");
		}
		
		#ifdef LINUX
		engine->net_manager = new ConnectionManager();
		engine->net_manager->engine = engine;
		engine->net_manager->game = this;
		engine->net_manager->connect_to_game_server();
		#endif

		player->weapons = new WeaponManager();
		player->weapons->owner = player;
		player->weapons->main_weapon = new Weapon;
		player->weapons->main_weapon->ammo = 30;


}
Game::Game(Engine* engine){
	this->engine = engine;
	
}
void Game::init(){
	gui = new GUI(engine);
	gui->update_elements_mvp();
	gui->update_engine_meshes();
	init_player();
	
}

Game::~Game(){
	delete gui;
	delete engine->net_manager;
	delete player->weapons;
}

void Game::update(){
	if(!engine->edit_mode){
			if(player->mesh){
				player->update();
				engine->net_manager->send_player_data(player);					
			}
			
		}			

	gui->calculate_mouse_position();			
	
	if(gui->is_button_pressed("jump")){
		#ifdef ANDROID
			//LOGW("jump");
		#endif
		//std::cout << "jump pressd\n";

	}
}

void Game::spawn_new_player(){
	ThirdPerson* new_player = new ThirdPerson();		
	
	new_player->engine = this->engine;	
	new_player->mesh = nullptr;
	new_player->camera_position = vec3(-0.4,-1.7,-2.8);
	if(this->player_id == -1){
		std::runtime_error("no player assigned from map file");
	}else{
			
		EMesh* player_mesh = engine->meshes[this->player_id];
		EMesh* new_player_mesh = new EMesh;
		memcpy(new_player_mesh,player_mesh,sizeof(EMesh));
		new_player_mesh->model_matrix = translate(mat4(1.0),vec3(0,0,2));
		engine->meshes.push_back(new_player_mesh);


	}		
	
	if(new_player->mesh == nullptr){
		std::runtime_error("no player mesh pointer assigned");
	}
}