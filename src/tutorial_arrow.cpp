#include "tutorial_arrow.h"
#include <bn_log.h>
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
    ++_timer;
    bn::fixed increment(0.25);
    if(_dir == direction::DOWN || _dir == direction::LEFT){
        increment = -increment;
    }

    if(_timer > 15){
        increment = -increment;
    }

    if(_dir == direction::UP || _dir == direction::DOWN){
        set_y(get_y() + increment);
    }else{
        set_x(get_x() + increment);
    }

    
    if(_timer >= 30){
        _timer = 0;
    }
}


}