#include "weapon.hpp"
#include "ThirdPerson.hpp"
#include <string>
void WeaponManager::shoot(Weapon* weapon){
    
    shoot_time += this->owner->engine->deltaTime;
    if(shoot_time > 0.1){
        shoot_time = 0;
        if(weapon->ammo >= 1){
            std::string path = owner->engine->assets.path("Sounds/shoot.wav");
            owner->engine->audio_manager.play(path);
            weapon->ammo -= 1;
            std::cout << "shoot\n";
        }

    }
}

void WeaponManager::reload(Weapon* weapon){
    if(weapon->ammo >= 0){
    std::string path = owner->engine->assets.path("Sounds/weapon_reload.wav");
    owner->engine->audio_manager.play(path);
    weapon->ammo = 30;
    }

}
