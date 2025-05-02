#include "strongbad.h"

namespace trog { 

strongbad::strongbad(int xcor, int ycor) : 
        entity(xcor, ycor, 0, 0, bn::sprite_items::strongbad.create_sprite(xcor, ycor)),
        _anim(bn::create_sprite_animate_action_forever(
                _sprite, 5, bn::sprite_items::strongbad.tiles_item(),
                0, 1 )),
        _advance_anim(true){
    
}

void strongbad::update(){
    entity::update();
}

void strongbad::update_anim(){
    entity::update_anim();

    if(_advance_anim) _anim.update();

}
void strongbad::talk(){
    _anim = bn::create_sprite_animate_action_forever(
                _sprite, 10, bn::sprite_items::strongbad.tiles_item(),
                2, 3 );
    _anim.update();
}


}

