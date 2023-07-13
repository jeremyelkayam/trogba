#include "trogdor.h"
#include "bn_sprite_items_trogdor.h"

namespace trog { 

trogdor::trogdor(bn::fixed xcor, bn::fixed ycor, session_info &sesh, bool iframes, common_stuff &common_stuff, uint8_t initial_trogmeter) : 
    player(xcor, ycor, sesh, iframes, bn::sprite_items::trogdor, 8, common_stuff, initial_trogmeter), 
    _walkcycle(bn::create_sprite_animate_action_forever(_sprite, 2, 
            bn::sprite_items::trogdor.tiles_item(), 0, 1, 2, 3, 4, 5, 6, 7)) {
    //TODO: Mess with bg priority on the player sprite to make it appear above the background.
}

void trogdor::pass_out(){
    _sprite.set_tiles(_spritem.tiles_item(), 16);
}

void trogdor::thumb_it_up(){
    _sprite.set_tiles(_spritem.tiles_item(), 15);
}

void trogdor::flex(){
    _flex = bn::create_sprite_animate_action_forever(
                _sprite, 5, _spritem.tiles_item(), 
                11, 12, 13, 14, 14, 13, 12);
}


void trogdor::update_anim(){
    entity::update_anim();
    if((_move_action && !_move_action->done()) || _move_by_action || _update_anim_when_not_moving){
        _walkcycle.update();
    }
    if(_flex){
        _flex->update();
    }
    update_firebreath();
}

void trogdor::update(){
    player::update();
    if(!dead() && any_dpad_input()){
        _walkcycle.update();
    }
}

}