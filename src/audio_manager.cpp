
#include "audio_manager.hpp"
#include <SDL2/SDL.h>
#include "audio.h"
#include <iostream>

void AudioManager::init(){
	if(SDL_Init(SDL_INIT_AUDIO) < 0)
	{
		std::cout << "sdl init error\n";
	}
	initAudio();
}

void AudioManager::play(){
	
	playSound("Game/Assets/Sounds/weapon_reload.wav",SDL_MIX_MAXVOLUME);
	

}
void AudioManager::finish(){
	endAudio();
	SDL_Quit();
}

AudioManager::~AudioManager(){
	finish();
}