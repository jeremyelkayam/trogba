#include "tutorial_arrow.h"
#include "bn_sprite_items_tutorial_arrow.h"

namespace trog {

tutorial_arrow::tutorial_arrow(bn::fixed xcor, bn::fixed ycor, direction dir) : 
    entity(xcor, ycor, 0, 0, bn::sprite_items::tutorial_arrow.create_sprite(xcor, ycor)),
    _timer(0),
    _dir(dir){

    switch(_dir){
        case direction::DOWN:
            set_rotation_angle(180);
        break;
        case direction::LEFT:
            set_rotation_angle(90);
        break;
        case direction::RIGHT:
            set_rotation_angle(270);
        break;
        default:

        break;
    }
}

void tutorial_arrow::update(){
    uint8_t oscillate_time = 50;
    ++_timer;
    bn::fixed increment(0.25);
    if(_dir == direction::DOWN || _dir == direction::LEFT){
        increment = -increment;
    }

    uint8_t multiplier = _timer;
    if(_timer >= oscillate_time / 2){
        multiplier = oscillate_time - _timer;
    }

    if(_dir == direction::UP || _dir == direction::DOWN){
        _sprite.set_y(_pos.y() + increment * multiplier);
    }else{
        _sprite.set_x(_pos.x() + increment * multiplier);
    }

    
    if(_timer >= oscillate_time){
        _timer = 0;
    }
}


}