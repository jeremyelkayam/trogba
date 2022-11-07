#include <bn_log.h>
#include "entity.h"


namespace trog {

entity::entity(bn::fixed xcor, bn::fixed ycor, bn::fixed width, bn::fixed height, bn::sprite_ptr sprite) : 
        _pos(xcor, ycor),
        _hitbox(xcor, ycor, width, height),
        _sprite(sprite){
    BN_LOG("entity created at", xcor, ", ", ycor);
}
}