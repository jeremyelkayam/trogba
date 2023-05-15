#include <bn_keypad.h>
#include <bn_string.h>

#include "bn_regular_bg_items_hi_scores_bg.h"
#include "options_scene.h"
#include "constants.h"

#include "bn_sprite_items_trogdor_variable_8x16_font_red.h"
#include "bn_sprite_items_trogdor_variable_8x16_font_brown.h"

namespace trog {

options_scene::options_scene(common_stuff &common_stuff) : 
        _common_stuff(common_stuff),
        _scroll(bn::regular_bg_items::hi_scores_bg.create_bg(8, 64)),
        _index(0) {
    _common_stuff.text_generator.set_center_alignment();
    _common_stuff.text_generator.set_palette_item(RED_PALETTE);
    _common_stuff.text_generator.generate(0, -72, "YE OLDE OPTIONS MENU", _header_sprites);

    _options_vec.emplace_back(new bool_option("Troghammer", _common_stuff.savefile.troghammer));
    _options_vec.emplace_back(new bool_option("Trogmeter Depreciation", _common_stuff.savefile.decrement_trogmeter));

    for(int z = 0; z < _options_vec.size(); z++){
        _options_vec.at(z)->redraw(_index == z, _common_stuff.small_generator, -50 + z*10);
    }
}

bn::optional<scene_type> options_scene::update(){
    bn::optional<scene_type> result;

    if(bn::keypad::left_pressed()){
        _options_vec.at(_index)->left();
    }else if(bn::keypad::right_pressed()){
        _options_vec.at(_index)->right();
    }else if(bn::keypad::up_pressed()){
        _options_vec.at(_index)->redraw(false, _common_stuff.small_generator, -50 + _index*10);
        if(_index == 0) _index = _options_vec.size();
        _index--;
    }else if(bn::keypad::down_pressed()){
        _options_vec.at(_index)->redraw(false, _common_stuff.small_generator, -50 + _index*10);
        _index++;
        if(_index == _options_vec.size()) _index = 0;
    }
    
    if(bn::keypad::any_pressed()){
        _options_vec.at(_index)->redraw(true, _common_stuff.small_generator, -50 + _index*10);
    }

    if(bn::keypad::b_pressed()){
        _common_stuff.save();
        result = scene_type::MENU;
    }

    return result;
}

bool_option::bool_option(const bn::string<32> &name, bool &value) : 
    _options({true, false}),
    _value(value),
    _name(name) {
}

void bool_option::left(){
    if(_value == false){
        _value = true;
    }
}
void bool_option::right(){
    if(_value == true){
        _value = false;
    }
}

void bool_option::redraw(const bool &selected, bn::sprite_text_generator &text_generator, const bn::fixed &ycor){
    _text_sprites.clear();
    bn::sprite_palette_item palette_item = BROWN_PALETTE;
    if(selected) palette_item = RED_PALETTE;

    text_generator.set_palette_item(palette_item);
    text_generator.set_left_alignment();
    text_generator.generate(-90, ycor, _name, _text_sprites);

    text_generator.set_center_alignment();
    text_generator.generate(50, ycor, _value ? "<ON>" : "ON", _text_sprites);
    text_generator.generate(80, ycor, !_value ? "<OFF>" : "OFF", _text_sprites);
}

}