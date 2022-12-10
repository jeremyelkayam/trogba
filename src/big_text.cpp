#include "big_text.h"
#include "trogdor_variable_16x32_sprite_font.h"
#include "trogdor_variable_32x64_sprite_font.h"
namespace trog { 



big_text::big_text(bool bigger, const bn::fixed &x, const bn::fixed &y, const char *text, const bn::sprite_palette_item &palette) :
        _bigger(bigger) {

    bn::sprite_text_generator generator(_bigger ? 
        trog::variable_32x64_sprite_font : trog::variable_16x32_sprite_font);
    generator.set_center_alignment();
    generator.set_palette_item(palette);
    generator.generate(x, y, text, _text_sprites);
}

void big_text::set_visible(bool visible){
    for(bn::sprite_ptr &sprite : _text_sprites){
        sprite.set_visible(visible);
    }
}

}