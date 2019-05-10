#include "game.hpp"
void Game::init_player(){

		player = new ThirdPerson();		
		//player = new Vehicle();

		player->engine = this->engine;	
		player->mesh = nullptr;
		player->camera_position = vec3(0,-2.5,-4.5);
		if(this->player_id == -1){
			std::runtime_error("no player assigned from map file");
		}else{
				
			player->mesh = engine->meshes[this->player_id];
		}		
		
		if(player->mesh == nullptr){
			std::runtime_error("no player mesh pointer assigner");
		}

}
Game::Game(Engine* engine){
	this->engine = engine;
	
}
void Game::init(){
	gui = new GUI(engine);
	gui->update_elements_mvp();
	init_player();
}

Game::~Game(){
	delete gui;
}

void Game::update(){
	if(!engine->edit_mode){
			if(player->mesh){
				player->update();	
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