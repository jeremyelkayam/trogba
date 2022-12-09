#include <bn_log.h>
#include "arrow.h"

namespace trog { 

arrow::arrow(bn::fixed_point pos, bool facing_left) : 
        entity(pos.x(), pos.y(), TROG_ARROW_WIDTH, TROG_ARROW_HEIGHT, bn::sprite_items::arrow.create_sprite(_pos)),
        _facing_left(facing_left),
        _speed(TROG_ARROW_SPEED),
        _just_spawned(true) {
    _sprite.set_visible(false);
    _sprite.set_horizontal_flip(facing_left);
    int xoffset = 15;
    int yoffset = -2;
    if(facing_left){
        _speed = -_speed;
        xoffset = -xoffset;
    }
    _pos.set_x(_pos.x() + xoffset);
    _pos.set_y(_pos.y() + yoffset);
}

void arrow::update(){
    entity::update();
    if(_just_spawned){
        _sprite.set_visible(true);
        _just_spawned = false;
    }
    _pos.set_x(_pos.x() + _speed);
}

}

