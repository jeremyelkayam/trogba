#include "sucks.h"
#include "bn_sprite_items_sucks.h"

namespace trog { 

sucks::sucks(bn::fixed xcor, bn::fixed ycor, session_info &sesh, bool iframes, common_stuff &common_stuff, uint8_t initial_trogmeter) : 
    player(xcor, ycor, sesh, iframes, bn::sprite_items::sucks, 4, common_stuff, initial_trogmeter), 
    _walkcycle(bn::create_sprite_animate_action_forever(_sprite, 5, 
            bn::sprite_items::sucks.tiles_item(), 0, 1, 2, 3, 2, 1)) {
}

void sucks::update(){
    player::update();
    if(!dead() && any_dpad_input()){
        _walkcycle.update();
    }
}

}