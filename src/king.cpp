#include "king.h"

namespace trog { 

king::king(int xcor, int ycor) : 
        entity(xcor, ycor, 0, 0, 
            bn::sprite_items::king.create_sprite(
                xcor - 32, ycor, 0)),
        _right_sprite(
            bn::sprite_items::king.create_sprite(
                xcor + 32, ycor, 1))
        {
    _sprite.set_z_order(MID_ZORDER);

    
}

void king::update(){
    entity::update();
}

void king::update_anim(){
    entity::update_anim();
}

}

