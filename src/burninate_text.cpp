#include "burninate_text.h"

#include "bn_sprite_items_trogdor_variable_8x16_font_black.h"
#include "bn_affine_bg_items_burninatefire0.h"
#include "bn_affine_bg_items_burninatefire1.h"
#include "bn_affine_bg_items_burninatefire2.h"

namespace trog{ 
    

burninate_text::burninate_text() : 
    big_text(true, 0, 0, "BURNINATE!", bn::sprite_items::trogdor_variable_8x16_font_black.palette_item()),
    _fire(bn::affine_bg_items::burninatefire0.create_bg(0,-16)),
    _scaler(_fire, TROG_BURNINATE_PAUSETIME / 2, 0.2){

    for(int z = 0; z < TROG_BURNINATE_PAUSETIME / 2 ; z++){
        _scaler.update();
    }
    _fire.set_priority(0);
    _fire.set_wrapping_enabled(false);
}

void burninate_text::update(){
    _scaler.update();

    //animate the flame effect
    ++_timer;
    if(_timer == TROG_BURNINATE_ANIM_TIME_BTWN_FRAMES){
        _fire.set_item(bn::affine_bg_items::burninatefire1);
    }else if(_timer==TROG_BURNINATE_ANIM_TIME_BTWN_FRAMES * 2){
        _fire.set_item(bn::affine_bg_items::burninatefire2);
    }else if(_timer>=TROG_BURNINATE_ANIM_TIME_BTWN_FRAMES * 3){
        _timer=0;
        _fire.set_item(bn::affine_bg_items::burninatefire0);
    }
}

void burninate_text::set_visible(bool visible){
    big_text::set_visible(visible);
    _fire.set_visible(visible);
}

}