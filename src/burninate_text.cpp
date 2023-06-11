#include "burninate_text.h"

#include "bn_sprite_items_trogdor_variable_8x16_font_black.h"
#include "bn_sprite_items_trogdor_variable_8x16_font_red.h"
#include "bn_sprite_items_cottagefire.h"

namespace trog{ 
    

burninate_text::burninate_text(bn::sprite_text_generator &generator) : 
    big_text(true, 0, 0, "BURNINATE!", 
    bn::sprite_items::trogdor_variable_8x16_font_red.palette_item(),
    bn::sprite_items::trogdor_variable_8x16_font_black.palette_item(), generator)    {

}

void burninate_text::update(){

}

void burninate_text::set_visible(bool visible){
    big_text::set_visible(visible);
}

}