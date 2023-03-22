#include "big_text.h"
#include "trogdor_variable_8x16_sprite_font.h"
#include "bn_sprite_items_trogdor_variable_8x16_font_red.h"
namespace trog { 



big_text::big_text(bool bigger, const bn::fixed &x, const bn::fixed &y, const char *text, const bn::sprite_palette_item &palette, const bn::sprite_palette_item &bg_palette) :
        _bigger(bigger) {

    bn::sprite_text_generator generator(trog::variable_8x16_sprite_font);
    generator.set_center_alignment();
    generator.set_palette_item(bg_palette);
    generator.generate(x, y, text, _text_sprites);
    generator.set_palette_item(palette);
    generator.generate(x - 1, y - 1, text, _text_sprites);
    // int scale_factor = bigger ? 3 : 2;
    for(bn::sprite_ptr &sprite : _text_sprites) {
        sprite.set_scale(2);
        sprite.set_x(sprite.x() * 2);
    }
}

void big_text::set_visible(bool visible){
    for(bn::sprite_ptr &sprite : _text_sprites){
        sprite.set_visible(visible);
    }
}

}