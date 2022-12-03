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
    if(_time_since_spawn == 0) { 
        //if he just spawned in we must make him visible
        _sprite.set_visible(true);
    }
    ++_time_since_spawn;
    if(TROG_ARCHER_DRAWBOW_WAITTIME < _time_since_spawn 
        && _time_since_spawn < TROG_ARCHER_SHOOT_WAITTIME){
        _sprite.set_item(bn::sprite_items::archer_bowdrawn);
    }else if (TROG_ARCHER_SHOOT_WAITTIME == _time_since_spawn && !_arrow){
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
    }else if(!_arrow && TROG_ARCHER_DISAPPEAR_WAITTIME < _time_since_spawn){
        //at this point we've shot our arrow and it also got deleted
        // possibly from hitting something
        // so we need to despawn
        return true;
    }else return false;
}

bn::fixed_rect archer::get_hitbox() {
    if(_arrow){
        return _arrow->get_hitbox();
    }else{
        //bogus rectangle that's way offscreen  
        return bn::fixed_rect(-500,-500, 0, 0);
    }
}

void archer::destroy_arrow() {
    _arrow.reset();
}

void archer::set_visible(bool visible){
    entity::set_visible(visible);
    if(_arrow){
        _arrow->set_visible(visible);
    }
}

}
