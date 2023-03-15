#include "arrow.h"

namespace trog { 

arrow::arrow(bn::fixed_point pos, bool facing_left) : 
        entity(pos.x(), pos.y(), 10, 3, bn::sprite_items::arrow.create_sprite(_pos)),
        _facing_left(facing_left),
        _speed(1.5) {
    _sprite.set_horizontal_flip(facing_left);
    _sprite.set_visible(true);
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
    _pos.set_x(_pos.x() + _speed);
}

}

