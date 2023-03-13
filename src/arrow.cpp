#include <bn_log.h>
#include "arrow.h"

namespace trog { 

arrow::arrow(bn::fixed_point pos, bool facing_left) : 
        entity(pos.x(), pos.y(), 10, 3, bn::sprite_items::arrow.create_sprite(_pos)),
        _facing_left(facing_left),
        _speed(1.5),
        _rotating(false),
        _plink_timer(0) {
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
void arrow::update_anim(){
    entity::update_anim();
    if(_plink_timer){
        ++_plink_timer;
    }
    if(_plink_timer == 10){
        move_to(20, 0, 90);        
    }

    BN_LOG("update arrow anim");
    if(_rotating){
        short new_angle = _sprite.rotation_angle().integer() - 10;
        new_angle %= 360;
        if(new_angle < 0) new_angle += 360;
        _sprite.set_rotation_angle(new_angle);
    }
}

void arrow::plink(){
    _plink_timer = 1;
    rotate();
    move_to(10, -40, -15);
}

}

