#include "tutorial_box.h"
#include "constants.h"
#include "common_stuff.h"
#include "bn_regular_bg_items_textbox.h"
#include "bn_sprite_items_trogdor_variable_8x16_font.h"
#include "small_fonts.h"

namespace trog { 

tutorial_box::tutorial_box(const char *text) : 
    _box(bn::regular_bg_items::textbox.create_bg(0, 67)),
    _speed(0),
    _small_gen(small_font_white)
{
    _box.set_priority(0);
    _small_gen.set_bg_priority(0);
    _small_gen.set_z_order(1);
    _small_gen.set_left_alignment();
    set_text(text);
}

tutorial_box::tutorial_box() : 
    _box(bn::regular_bg_items::textbox.create_bg(0, 103)),
    _speed(0),
    _small_gen(small_font_white)
{
    _box.set_priority(0);
    _small_gen.set_bg_priority(0);
    _small_gen.set_z_order(1);
    _small_gen.set_left_alignment();
}

void tutorial_box::set_text(const char *text){
    _text_sprites.clear();
    bn::vector<bn::string<64>, 3> lines = common_stuff::split_into_lines(text);

    for(uint8_t i = 0; i < lines.size(); i++ ){    
        _small_gen.generate(_box.x() - 115, _box.y() - 12 + i*9, lines.at(i), _text_sprites);
    }
}

void tutorial_box::update(){
    _box.set_y(_box.y() + _speed);
    for(bn::sprite_ptr &text_sprite : _text_sprites) {
        text_sprite.set_y(text_sprite.y() + _speed);
    }

    if(_box.y() <= 67 || is_off_screen()){
        _speed = 0;
    }
}

}