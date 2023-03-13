#include <bn_log.h>
#include "archer.h"

namespace trog { 

archer::archer(bn::fixed ycor, bool facing_left) : 
        entity(0, ycor, bn::fixed(0), bn::fixed(0), bn::sprite_items::archer.create_sprite(-300, ycor)),
        _facing_left(facing_left),
        _time_since_spawn(0) {
    _sprite.set_horizontal_flip(facing_left);
    if(facing_left){
        _pos.set_x(110);
        _sprite.set_x(110);
    }else{
        _pos.set_x(-110);
        _sprite.set_x(-110);
    }
}

void archer::update(){
    entity::update();

    ++_time_since_spawn;
    if(60 < _time_since_spawn 
        && _time_since_spawn < 90){
        set_bow_drawn(true);
    }else if (90 == _time_since_spawn && !_arrow){
        set_bow_drawn(false);
        shoot();
    }else if(120 < _time_since_spawn){
        _sprite.set_visible(false);
    }

    if(_arrow){
        _arrow->update();
    }
}

void archer::shoot(){
    _arrow = arrow(_pos, _facing_left);
}

void archer::set_bow_drawn(bool drawn){
    if(drawn){
        _sprite.set_tiles(bn::sprite_items::archer.tiles_item(), 1);
    }else{
        _sprite.set_tiles(bn::sprite_items::archer.tiles_item(), 0);
    }
}

bool archer::remove_from_map(){
    if(_arrow) {
        return _arrow->out_of_bounds();
    }else if(!_arrow && 120 < _time_since_spawn){
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
