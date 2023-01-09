#include <bn_log.h>
#include "heart.h"

namespace trog { 

heart::heart(int xcor, int ycor) : 
        entity(xcor, ycor, 0, 0, bn::sprite_items::heart.create_sprite(xcor, ycor)) {
    
}

void heart::update(){
    entity::update();
}
void heart::update_anim(){
    entity::update_anim();

    if(_anim){
        if(!_anim->done()){
            _anim->update();
        }else{
            _sprite.set_visible(false);
        }
    }
}
void heart::dissipate(){
    _anim = bn::create_sprite_animate_action_once(
                _sprite, 5, bn::sprite_items::heart.tiles_item(),
                0, 1, 2, 3 );
}


}

