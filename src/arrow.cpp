#include <bn_log.h>
#include "arrow.h"

namespace trog { 

arrow::arrow(bn::fixed_point pos, bool facing_left) : 
        entity(pos.x(), pos.y(), TROG_ARROW_WIDTH, TROG_ARROW_HEIGHT, bn::sprite_items::arrow.create_sprite(_pos)),
        _facing_left(facing_left),
        _speed(TROG_ARROW_SPEED),
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
    if(_plink_timer){
        update_anim();
        _pos = _sprite.position();
        _hitbox.set_position(_pos);
    }else{
        entity::update();
        _pos.set_x(_pos.x() + _speed);
    }
}
void arrow::update_anim(){
    entity::update_anim();
    if(_plink_timer){
        ++_plink_timer;
    }
    if(_plink_timer == 10){
        int8_t sign = _sprite.horizontal_flip() ? 1 : -1;
        move_to(30, _sprite.x() + (40 * sign), _sprite.y() + 160);  
    }

    // BN_LOG("update arrow anim");
    if(_rotating){
        int8_t sign = _sprite.horizontal_flip() ? 1 : -1;
        short new_angle = _sprite.rotation_angle().integer() - (10 * sign);
        new_angle %= 360;
        if(new_angle < 0) new_angle += 360;
        _sprite.set_rotation_angle(new_angle);
    }
}

void arrow::plink(){
    _plink_timer = 1;
    rotate();

    int8_t sign = _sprite.horizontal_flip() ? 1 : -1;
    move_to(10, _sprite.x() + (40 * sign) , _sprite.y() - 10);

}

}

