#include "ratherdashing.h"

namespace trog { 

ratherdashing::ratherdashing(int xcor, int ycor) : 
        entity(xcor, ycor, 0, 0, bn::sprite_items::ratherdashing.create_sprite(xcor, ycor)),
        _anim(bn::create_sprite_animate_action_forever(
                _sprite, 8, bn::sprite_items::ratherdashing.tiles_item(),
                1, 2, 1, 0)) {
    _sprite.set_z_order(MID_ZORDER);
}

void ratherdashing::update(){
    entity::update();
}

void ratherdashing::update_anim(){
    entity::update_anim();
    _anim.update();
}

}

