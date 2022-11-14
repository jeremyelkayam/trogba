#include <bn_log.h>
#include "knight.h"

namespace trog { 

knight::knight(bn::fixed xcor, bn::fixed ycor, bn::fixed speed, unsigned short direction) : 
        entity(xcor, ycor, bn::fixed(TROG_KNIGHT_WIDTH), bn::fixed(TROG_KNIGHT_HEIGHT), bn::sprite_items::knight.create_sprite(xcor, ycor)),
        _speed(speed),
        _direction(direction) {
    BN_LOG("help im a knight. speed: ",_speed);
    _frames_moving = 0;
}

void knight::update(){
    entity::update();
    if(going_to_go_offscreen(_hitbox, _speed, _direction)){
       _direction+=180;
    }
    // probably refactor this into a base class
    // since i use it too dang much 
    _pos += unit_vector(_direction).multiplication(_speed);
    // if(_frames_moving == 120){
    //     //if moving diagonally
    //     if(_direction % 90){
    //         _speed/=2;
    //         //change direction to horizontal
    //         _direction+=45;            
    //     }else{
    //         _speed*=2;
    //         _direction-=45;
    //     }
    //     _frames_moving = 0;
    //         BN_LOG("lol");

    // }
    // //keep it under 360 to prevent weird overflows
    // _direction = _direction % 360;

    // ++_frames_moving;
}

//need to use hitbox in this calculation
bool knight::going_to_go_offscreen(bn::fixed_rect hitbox, bn::fixed speed, bn::fixed direction){
    // should refactor this later
    // also this wont work anyway because it needs to be mirrored in 2 directions
    // depending what side he intersects with
    bn::fixed_rect new_box = bn::fixed_rect(hitbox.position() + (unit_vector(direction).multiplication(speed)), hitbox.dimensions());
    bool oob_left = new_box.left() < TROG_COUNTRYSIDE_LEFT_BOUND; 
    bool oob_right = new_box.right() > TROG_COUNTRYSIDE_RIGHT_BOUND;
    bool oob_top = new_box.top() < TROG_COUNTRYSIDE_TOP_BOUND;
    bool oob_bottom =  new_box.bottom() > TROG_COUNTRYSIDE_BOTTOM_BOUND;
    return (oob_left || oob_right || oob_top || oob_bottom);
}

}
