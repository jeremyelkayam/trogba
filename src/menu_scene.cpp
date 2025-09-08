#include <bn_string.h>
#include <bn_keypad.h>
#include <bn_sound_items.h>

#include "menu_scene.h"
#include "serif_fonts.h"
#include "common_functions.h"

namespace trog {

menu_scene::menu_scene(common_stuff &common_stuff,
    const bn::regular_bg_item &bgitem) : 
        _common_stuff(common_stuff),
        _red_gen(serif_font_red),
        _gray_gen(serif_font_darkgray),
        _white_gen(serif_font_white),
        _bg(bgitem.create_bg(0, 0)),
        _selected_option_index(0)
{
    _red_gen.set_z_order(1);
    _gray_gen.set_z_order(1);
    _white_gen.set_z_order(1);

}

bn::optional<scene_type> menu_scene::update(){
    
    bn::optional<scene_type> result;

    for(uint8_t i = 0; i < _menu_options.size(); i++){
        if(i == _selected_option_index) {
            _menu_options.at(i).turn_red();
        } else {
            _menu_options.at(i).turn_white();
        }
    }

    return result;    
}

menu_option::menu_option(const bn::fixed &x, const bn::fixed &y, 
    const char *text, bn::sprite_text_generator& red_generator, 
    bn::sprite_text_generator& gray_generator, 
    bn::sprite_text_generator& white_generator, 
    const scene_type &scene_type) :
    _x(x),
    _y(y),
    _width(0),
    _scene_type(scene_type) {

    //split into lines doesn't *quite* apply here b/c it's based on the 8x8 font 
    //but for our purposes it's fine
    bn::vector<bn::string<64>, 3> lines = split_into_lines(text);
    
    red_generator.set_center_alignment();
    gray_generator.set_center_alignment();
    white_generator.set_center_alignment();

    bn::fixed spacing = 12;
    bn::fixed starting_y = y - spacing * (lines.size() - 1) / 2;

    for(int line_num = 0; line_num < lines.size(); ++line_num)
    {
        bn::string<64> &current_line = lines.at(line_num);

        bn::fixed line_width = 9 * current_line.size();
        if(line_width > _width) _width = line_width;

        gray_generator.generate(x - 1, starting_y + spacing*line_num + 1, 
            current_line, _drop_shadow_sprites);    
    
        white_generator.generate(x, starting_y + spacing*line_num, 
            current_line, _text_sprites);    
    
        red_generator.generate(x, starting_y + spacing*line_num, 
            current_line, _red_text_sprites);       
    }

    turn_white();
}

void menu_option::turn_red(){
    for(bn::sprite_ptr &sprite : _red_text_sprites) sprite.set_visible(true);
}

void menu_option::turn_white(){
    for(bn::sprite_ptr &sprite : _red_text_sprites) sprite.set_visible(false);
}

void menu_option::set_y(const bn::fixed &y){
    for(bn::sprite_ptr &sprite : _text_sprites) sprite.set_y(y);
    for(bn::sprite_ptr &sprite : _drop_shadow_sprites) sprite.set_y(y + 1);
    for(bn::sprite_ptr &sprite : _red_text_sprites) sprite.set_y(y);
}

}