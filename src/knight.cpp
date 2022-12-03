#include <bn_log.h>
#include "knight.h"

namespace trog { 

knight::knight(bn::fixed xcor, bn::fixed ycor, bn::fixed speed, unsigned short direction) : 
        entity(xcor, ycor, bn::fixed(TROG_KNIGHT_WIDTH), bn::fixed(TROG_KNIGHT_HEIGHT), bn::sprite_items::knight.create_sprite(xcor, ycor)),
        _speed(speed),
        _direction(direction),
        _walkcycle(bn::create_sprite_animate_action_forever(
                    _sprite, 20, bn::sprite_items::knight.tiles_item(), 0, 3, 2, 3)) {
    BN_LOG("help im a knight. speed: ",_speed);
    _frames_moving = 0;
}

void knight::update(){
    entity::update();
    _walkcycle.update();
    if(going_to_go_offscreen(_speed, _direction)){
       _direction+=180;
    }
    _pos += unit_vector(_direction).multiplication(_speed);
}

}
