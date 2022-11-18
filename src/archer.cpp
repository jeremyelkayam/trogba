#include <bn_log.h>
#include "archer.h"

namespace trog { 

archer::archer(bn::fixed ycor, bool facing_left) : 
        entity(0, ycor, bn::fixed(0), bn::fixed(0), bn::sprite_items::archer.create_sprite(0, ycor)),
        _facing_left(facing_left) {
    _sprite.set_visible(false);
    _sprite.set_horizontal_flip(facing_left);
    if(facing_left){
        _pos.set_x(110);
    }else{
        _pos.set_x(-110);
    }
    _time_since_spawn = 0;
}

void archer::update(){
    entity::update();
    _sprite.set_visible(true);
    ++_time_since_spawn;
    if(TROG_ARCHER_SHOOT_WAITTIME < _time_since_spawn 
        && _time_since_spawn < TROG_ARCHER_SHOOT_WAITTIME){
        _sprite.set_item(bn::sprite_items::archer_bowdrawn);
    }else if (TROG_ARCHER_SHOOT_WAITTIME < _time_since_spawn && !_arrow){
        _sprite.set_item(bn::sprite_items::archer);
        shoot();
    }else if(TROG_ARCHER_DISAPPEAR_WAITTIME < _time_since_spawn){
        _sprite.set_visible(false);
    }

    if(_arrow){
        _arrow->update();
    }
}

void archer::shoot(){
    bn::sound_items::shoot.play(TROG_DEFAULT_VOLUME);
    _arrow = arrow(_pos, _facing_left);
}

bool archer::remove_from_map(){
    if(_arrow) {
        return _arrow->out_of_bounds();
    }else return false;
}

}
