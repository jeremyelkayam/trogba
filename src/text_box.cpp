#include "text_box.h"
#include "bn_regular_bg_items_textbox.h"

namespace trog { 

text_box::text_box(bn::sprite_text_generator &text_generator, const char *text) : 
    _box(bn::regular_bg_items::textbox.create_bg(0, 67)){
    
    text_generator.set_left_alignment();
    text_generator.generate(-115, 58, text, _text_sprites);
}

}