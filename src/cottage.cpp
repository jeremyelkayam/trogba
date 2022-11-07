#include "bn_sprite_items_cottagedown.h"
#include "cottage.h"

namespace trog {

cottage::cottage(bn::fixed xcor, bn::fixed ycor, direction direction) : 
        entity(xcor, ycor, bn::fixed(20), bn::fixed(45), bn::sprite_items::cottagedown.create_sprite(xcor, ycor)),
        _direction(direction) {

}

void cottage::update(){
    
}

}