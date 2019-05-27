#include "weapon.hpp"
#include "ThirdPerson.hpp"
#include <string>
void WeaponManager::shoot(Weapon* weapon){
    
    shoot_time += this->owner->engine->deltaTime;
    if(shoot_time > 0.1){
        shoot_time = 0;
        if(weapon->ammo >= 1){
            std::string path = owner->engine->assets.path("Sounds/shoot.wav");
#ifdef ES2
owner->engine->audio_manager.play(path);
#endif // 

          
            weapon->ammo -= 1;
        }

    }
}

void WeaponManager::reload(Weapon* weapon){
    if(weapon->ammo >= 0){
    std::string path = owner->engine->assets.path("Sounds/weapon_reload.wav");
#ifdef ES2
    owner->engine->audio_manager.play(path);
#endif
    weapon->ammo = 30;
    }

}
