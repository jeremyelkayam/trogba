#include "burninate_text.h"

#include "bn_sprite_items_trogdor_variable_8x16_font_black.h"

namespace trog{ 
    

burninate_text::burninate_text() : 
    big_text(true, 0, 0, "BURNINATE!", bn::sprite_items::trogdor_variable_8x16_font_black.palette_item()) {

}

void burninate_text::update(){

    //animate the flame effect
    ++_timer;
}

void burninate_text::set_visible(bool visible){
    big_text::set_visible(visible);
}

}