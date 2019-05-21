#ifndef WEAPON_H
#define WEAPON_H
class ThirdPerson;
struct Weapon{
    int ammo = 0;
    int fire_rate = 0;
};

class WeaponManager{
public:
    ThirdPerson* owner = nullptr;
    Weapon* main_weapon = nullptr;
    void shoot(Weapon*);
    void reload(Weapon*);
    float shoot_time = 0;
};

#endif