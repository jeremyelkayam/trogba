#include "text_box.h"
#include "constants.h"
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
    bn::vector<bn::string<64>, 3> lines = split_into_lines(text);
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



bn::vector<bn::string<64>, 3> text_box::split_into_lines(const char *text){
    const uint8_t max_line_width = 220;
    bn::vector<bn::string<64>, 3>result;
    
    uint8_t line_width = 0;
    uint8_t line_num = 0;
    uint16_t line_start = 0;
    uint16_t line_end = 0;
    for(uint16_t pos = 0; text[pos] != '\0'; ++pos){
        char c = text[pos];
        line_width += variable_8x8_sprite_font_character_widths[c - ' '];
        if(line_width > max_line_width){
            uint16_t last_space = pos;
            while(text[last_space] != ' '){
                //Find the last space
                last_space--;
            }
            line_end = last_space;
        }

        //this case handles the end of the string (\0)
        //pos + 1 here because the for loop won't process the end-of-string character
        if(text[pos + 1] == '\0') line_end = pos + 1;

        //this case lets us handle manual newlines (\n)
        if(text[pos] == '\n') line_end = pos;

        //if this variable is set, we know where the end of the line is
        if(line_end){
            //copy the current line, up to the last space, into the result vector
            bn::string<64> line;
            bn::ostringstream line_stream(line);
            for(uint16_t i = line_start; i < line_end; i++){
                line_stream << text[i];
            }
            result.emplace_back(line);

            if(text[pos + 1] != '\0'){
                pos = line_end + 1;
            }
            line_start = pos;

            //we are effectively un-setting these variables by zeroing them
            line_width = 0;
            line_end = 0;

            ++line_num;
        }

    }

    return result;
}

}