#include "chiaroscuro.h"
#include "bn_sprite_items_chiaroscuro.h"

namespace trog { 

chiaroscuro::chiaroscuro(bn::fixed xcor, bn::fixed ycor, session_info &sesh, bool iframes, common_stuff &common_stuff, uint8_t initial_trogmeter) : 
    player(xcor, ycor, TROG_TROGDOR_WIDTH, TROG_TROGDOR_HEIGHT, TROG_TROGDOR_SPEED, 
        bn::fixed_point(TROG_FIREBREATH_XOFFSET, TROG_FIREBREATH_YOFFSET), sesh, 
        iframes, bn::sprite_items::chiaroscuro, 8, common_stuff, initial_trogmeter), 
    _walkcycle(bn::create_sprite_animate_action_forever(_sprite, 8, 
            bn::sprite_items::chiaroscuro.tiles_item(), 0, 1, 2, 3, 4, 1)) {
    //TODO: Mess with bg priority on the player sprite to make it appear above the background.
}


void chiaroscuro::update_anim(){
    entity::update_anim();
    if((_move_action && !_move_action->done()) || _move_by_action || _update_anim_when_not_moving){
        _walkcycle.update();
    }
    update_firebreath();
}

void chiaroscuro::update(){
    player::update();
    if(!dead()){
        _walkcycle.update();
    }
}

}