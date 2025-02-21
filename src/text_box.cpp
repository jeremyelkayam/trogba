#include "text_box.h"
#include "constants.h"
#include "common_stuff.h"
#include "trogdor_variable_8x8_sprite_font.h"
#include <bn_log.h>
#include "bn_regular_bg_items_textbox.h"
#include "bn_sprite_items_trogdor_variable_8x16_font.h"

namespace trog { 

text_box::text_box(bn::sprite_text_generator &text_generator, const char *text) : 
    _box(bn::regular_bg_items::textbox.create_bg(0, 67)) {
    _box.set_priority(2);
    set_text(text_generator, text);
}

text_box::text_box() : 
    _box(bn::regular_bg_items::textbox.create_bg(0, 103)) {
    _box.set_priority(2);
}

void text_box::set_text(bn::sprite_text_generator &text_generator, const char *text){
    _text_sprites.clear();
    bn::vector<bn::string<64>, 3> lines = common_stuff::split_into_lines(text);
    text_generator.set_bg_priority(0);
    text_generator.set_left_alignment();
    text_generator.set_palette_item(WHITE_PALETTE);
    for(uint8_t i = 0; i < lines.size(); i++ ){    
        text_generator.generate(_box.x() - 115, _box.y() - 12 + i*9, lines.at(i), _text_sprites);
    }
}

void text_box::update(){
    _box.set_y(_box.y() + _speed);
    for(bn::sprite_ptr &text_sprite : _text_sprites) {
        text_sprite.set_y(text_sprite.y() + _speed);
    }

    if(_box.y() <= 67 || is_off_screen()){
        _speed = 0;
    }
}

}