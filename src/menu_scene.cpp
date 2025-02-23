#include <bn_string.h>
#include <bn_keypad.h>
#include <bn_log.h>
#include <bn_sound_items.h>

#include "menu_scene.h"
#include "bn_sprite_items_trogdor_variable_8x16_font.h"
#include "bn_sprite_items_trogdor_variable_8x16_font_gray.h"
#include "bn_sprite_items_trogdor_variable_8x16_font_red.h"

namespace trog {

menu_scene::menu_scene(common_stuff &common_stuff,
    const bn::regular_bg_item &bgitem) : 
        _common_stuff(common_stuff),
        _bg(bgitem.create_bg(0, 0)),
        _selected_option_index(0)
{

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
    const char *text, bn::sprite_text_generator& text_generator, 
    const scene_type &scene_type) :
    _x(x),
    _y(y),
    _width(0),
    _scene_type(scene_type) {

    //split into lines doesn't *quite* apply here b/c it's based on the 8x8 font 
    //but for our purposes it's fine
    bn::vector<bn::string<64>, 3> lines = common_stuff::split_into_lines(text);

    BN_LOG("lines size: ", lines.size());
    
    text_generator.set_center_alignment();

    bn::fixed spacing = 12;
    bn::fixed starting_y = y - spacing * (lines.size() - 1) / 2;

    for(int line_num = 0; line_num < lines.size(); ++line_num)
    {
        bn::string<64> &current_line = lines.at(line_num);

        bn::fixed line_width = 9 * current_line.size();
        if(line_width > _width) _width = line_width;

        text_generator.set_palette_item(
            bn::sprite_items::trogdor_variable_8x16_font_gray.palette_item());
        text_generator.generate(x - 1, starting_y + spacing*line_num + 1, 
            current_line, _drop_shadow_sprites);    
    
        text_generator.set_palette_item(
            bn::sprite_items::trogdor_variable_8x16_font.palette_item());
        text_generator.generate(x, starting_y + spacing*line_num, 
            current_line, _text_sprites);    
    
        text_generator.set_palette_item(
            bn::sprite_items::trogdor_variable_8x16_font_red.palette_item());
        text_generator.generate(x, starting_y + spacing*line_num, 
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