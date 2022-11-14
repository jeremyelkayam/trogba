#include <bn_log.h>
#include "knight.h"

namespace trog { 

knight::knight(bn::fixed xcor, bn::fixed ycor, bn::fixed speed, bn::fixed_point direction) : 
        entity(xcor, ycor, bn::fixed(TROG_KNIGHT_WIDTH), bn::fixed(TROG_KNIGHT_HEIGHT), bn::sprite_items::knight.create_sprite(xcor, ycor)),
        _speed(speed),
        _direction(direction) {
    BN_LOG("help im a knight. speed: ",_speed);
}

void knight::update(){
    entity::update();
    if(going_to_go_offscreen(_pos, _speed, _direction)){
       _speed=-_speed; 
    }
    BN_LOG("knight x: ", _pos.x());
    //probably refactor this into a base class
    //since i use it too dang much 
    _pos += _direction.multiplication(_speed);
}

bool knight::going_to_go_offscreen(bn::fixed_point pos, bn::fixed speed, bn::fixed_point direction){
    // should refactor this later
    // also this wont work anyway because it needs to be mirrored in 2 directions
    bn::fixed_point new_pos = pos + (direction.multiplication(speed));
    bool oob_left = new_pos.x() < TROG_COUNTRYSIDE_LEFT_BOUND; 
    bool oob_right = new_pos.x() > TROG_COUNTRYSIDE_RIGHT_BOUND;
    bool oob_top = new_pos.y() < TROG_COUNTRYSIDE_TOP_BOUND;
    bool oob_bottom =  new_pos.y() > TROG_COUNTRYSIDE_BOTTOM_BOUND;
    return (oob_left || oob_right || oob_top || oob_bottom);
}

}
