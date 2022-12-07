#include "burninate_text.h"

#include "bn_sprite_items_trogdor_variable_8x16_font_black.h"
#include "bn_affine_bg_items_burninatefire.h"

namespace trog{ 
    

burninate_text::burninate_text(bn::sprite_text_generator &big_text_generator) : 
    big_text(big_text_generator, 0, 0, "BURNINATE!", bn::sprite_items::trogdor_variable_8x16_font_black.palette_item()),
    _fire(bn::affine_bg_items::burninatefire.create_bg(0,0)),
    _scaler(_fire, TROG_BURNINATE_PAUSETIME / 2, 0.2){

    for(int z = 0; z < TROG_BURNINATE_PAUSETIME / 2 ; z++){
        _scaler.update();
    }
    _fire.set_priority(0);
    _fire.set_wrapping_enabled(false);
}

void burninate_text::update(){
    _scaler.update();
}

void burninate_text::set_visible(bool visible){
    big_text::set_visible(visible);
    _fire.set_visible(visible);
}

}