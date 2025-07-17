#include "end_char.h"

namespace trog { 

end_char::end_char(int xcor, int ycor, const bn::sprite_item &item) : 
        entity(xcor, ycor, 0, 0, item.create_sprite(xcor, ycor)),
        _anim(bn::create_sprite_animate_action_forever(
                _sprite, 5, item.tiles_item(),
                0, 1 )),
        _advance_anim(true),
        _item(item){
    
}

void end_char::update(){
    entity::update();
}

void end_char::update_anim(){
    entity::update_anim();

    if(_advance_anim) _anim.update();

}
void end_char::talk(){
    _anim = bn::create_sprite_animate_action_forever(
                _sprite, 10, _item.tiles_item(),
                2, 3 );
    _anim.update();
}


}

