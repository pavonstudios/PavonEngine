#include "game.hpp"
#include "weapon.hpp"
#include "../Multiplayer/connectivity.hpp"
void Game::init_player(){

		player = new ThirdPerson();		
		//player = new Vehicle();		
		
		player->mesh = player_mesh;
		
		if(player->mesh == nullptr){
			std::runtime_error("no player mesh pointer assigned");
		}
		
		player->engine = this->engine;			
		player->camera_position = vec3(-0.4,-1.7,-2.8);

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
Game::Game(){
	
}
void Game::init(){
#ifdef ES2
	gui = new GUI(engine);
	gui->update_elements_mvp();
	gui->update_engine_meshes();
#endif
	init_player();

}

Game::~Game(){
#ifdef LINUX
	#ifdef ES2
	delete gui;
	#endif // ES2		
	delete engine->net_manager;
#endif // LINUX


	delete player->weapons;
}

void Game::update(){
	if(!engine->edit_mode){
			if(player->mesh){
				player->update();
#ifdef LINUX
				engine->net_manager->send_player_data(player);					
#endif
			}
			if(player2){
				if(player2->mesh){
				player2->mesh->model_matrix = glm::translate( glm::mat4(1.0),player2->location );

				}
			}
			
		}			

#ifdef ES2
	gui->calculate_mouse_position();			
	
	if(gui->is_button_pressed("jump")){
		#ifdef ANDROID
			//LOGW("jump");
		#endif
		//std::cout << "jump pressd\n";

	}
#endif
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

		new_player->mesh = new_player_mesh;
	}		
	
	if(new_player->mesh == nullptr){
		std::runtime_error("no player mesh pointer assigned");
	}

	this->player2 = new_player;
}