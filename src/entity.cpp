#include <bn_log.h>
#include <bn_display.h>

#include "entity.h"


namespace trog {

entity::entity(bn::fixed xcor, bn::fixed ycor, bn::fixed width, bn::fixed height, bn::sprite_ptr sprite) : 
        _pos(xcor, ycor),
        _hitbox(xcor, ycor, width, height),
        _sprite(sprite){
    BN_LOG("entity created at", xcor, ", ", ycor);
}

void entity::update(){
    _hitbox.set_position(_pos);
    _sprite.set_position(_pos);
}

bool entity::collides_with(entity &e){
    return _hitbox.intersects(e.get_hitbox());
}

bool entity::out_of_bounds(){
    return !_hitbox.intersects(bn::fixed_rect(0, 0, bn::display::width(), bn::display::height()));
}

}