#include "bubs.h"

namespace trog { 

bubs::bubs(int xcor, int ycor) : 
        entity(xcor, ycor, 0, 0, bn::sprite_items::bubs.create_sprite(xcor, ycor)),
        _anim(bn::create_sprite_animate_action_forever(
                _sprite, 10, bn::sprite_items::bubs.tiles_item(),
                0, 1, 2, 3)) {
    

    _sprite.set_z_order(MID_ZORDER);
}

void bubs::update(){
    entity::update();
}

void bubs::update_anim(){
    entity::update_anim();
    _anim.update();

}

}

