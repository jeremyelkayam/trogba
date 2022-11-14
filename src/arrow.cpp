#include <bn_log.h>
#include "arrow.h"

namespace trog { 

arrow::arrow(bn::fixed_point pos, bool facing_left) : 
        entity(pos.x(), pos.y(), 10, 2, bn::sprite_items::arrow.create_sprite(_pos)),
        _facing_left(facing_left),
        _speed(TROG_ARROW_SPEED) {
    _sprite.set_visible(false);
    _sprite.set_horizontal_flip(facing_left);
    if(facing_left){
        _speed = -_speed;
    }
}

void arrow::update(){
    entity::update();
    _sprite.set_visible(true);
    _pos.set_x(_pos.x() + _speed);
}

}

