#include "knight.h"

namespace trog { 

knight::knight(bn::fixed xcor, bn::fixed ycor, bn::fixed speed, bn::fixed_point direction) : 
        entity(xcor, ycor, bn::fixed(TROG_KNIGHT_WIDTH), bn::fixed(TROG_KNIGHT_HEIGHT), bn::sprite_items::knight.create_sprite(xcor, ycor)),
        _speed(speed),
        _direction(direction) {
    
}

void knight::update(){
    
}

}
