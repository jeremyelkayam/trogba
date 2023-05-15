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

    bn::vector<bn::fixed, 5> volume_vec;
    volume_vec.emplace_back(0);
    volume_vec.emplace_back(0.25);
    volume_vec.emplace_back(0.5);
    volume_vec.emplace_back(0.75);
    volume_vec.emplace_back(1);

    _options_vec.emplace_back(new bool_option("Troghammer", _common_stuff.savefile.troghammer));
    _options_vec.emplace_back(new bool_option("Trogmeter Depreciation", _common_stuff.savefile.decrement_trogmeter));
    _options_vec.emplace_back(new percent_option("Music Volume", _common_stuff.savefile.music_vol, volume_vec));
    _options_vec.emplace_back(new percent_option("Sound Volume", _common_stuff.savefile.sound_vol, volume_vec));
    _options_vec.emplace_back(new percent_option("Voice Volume", _common_stuff.savefile.voice_vol, volume_vec));

    for(uint8_t z = 0; z < _options_vec.size(); z++){
        _options_vec.at(z)->redraw(_index == z, _common_stuff.small_generator, ycor(z));
    }
}

bn::optional<scene_type> options_scene::update(){
    bn::optional<scene_type> result;

    if(bn::keypad::left_pressed()){
        _options_vec.at(_index)->left();
    }else if(bn::keypad::right_pressed()){
        _options_vec.at(_index)->right();
    }else if(bn::keypad::up_pressed()){
        _options_vec.at(_index)->redraw(false, _common_stuff.small_generator, ycor(_index));
        if(_index == 0) _index = _options_vec.size();
        _index--;
    }else if(bn::keypad::down_pressed()){
        _options_vec.at(_index)->redraw(false, _common_stuff.small_generator, ycor(_index));
        _index++;
        if(_index == _options_vec.size()) _index = 0;
    }
    
    if(bn::keypad::any_pressed()){
        _options_vec.at(_index)->redraw(true, _common_stuff.small_generator, ycor(_index));
    }

    if(bn::keypad::b_pressed()){
        //TODO: Make an alert that says options will be applied on starting a new game
        _common_stuff.save();
        result = scene_type::MENU;
    }

    return result;
}

bn::fixed options_scene::ycor(const uint8_t &index){
    bn::fixed result = -50;
    for(uint8_t z = 0; z < index; z++){
        if(_options_vec.at(z)->taller()) result+=18;
        else result+=10;
    }
    return result;
}





bool_option::bool_option(const bn::string<32> &name, bool &value) : 
    option(name),
    _options({true, false}),
    _value(value) {
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
    option::redraw(selected, text_generator, ycor);

    text_generator.set_center_alignment();
    text_generator.generate(50, ycor, _value ? "<ON>" : "ON", _text_sprites);
    text_generator.generate(80, ycor, !_value ? "<OFF>" : "OFF", _text_sprites);
}

percent_option::percent_option(const bn::string<32> &name, bn::fixed &value, const bn::vector<bn::fixed, 5> &options) : 
    option(name),
    _options(options),
    _value(value){
}


//UGH this could be better. how to reuse this while enforcing type?!
void percent_option::left(){
    if(current_index() != 0){
        _value = _options.at(current_index() - 1);
    }
}
void percent_option::right(){
    if(current_index() != 4){
        _value = _options.at(current_index() + 1);
    }
}

uint8_t percent_option::current_index(){
    for(uint8_t z = 0; z < _options.size(); z++){
        if(_options.at(z) == _value) return z;
    }
    BN_ERROR("Current value not in option list: ", _value);
    return -1;
}

void percent_option::redraw(const bool &selected, bn::sprite_text_generator &text_generator, const bn::fixed &ycor){
    option::redraw(selected, text_generator, ycor);
    text_generator.set_center_alignment();

    for(uint8_t z = 0; z < _options.size() ; z++){
        bn::string<16> opt;
        bn::ostringstream opt_stream(opt);
        if(z == current_index()) opt_stream << "<";            
        opt_stream << _options.at(z) * 100 << "%";
        if(z == current_index()) opt_stream << ">";            

        text_generator.generate(80 + 30 * (z - (_options.size() - 1) ), ycor + 8, opt, _text_sprites);
    }
}

void option::redraw(const bool &selected, bn::sprite_text_generator &text_generator, const bn::fixed &ycor){
    _text_sprites.clear();
    bn::sprite_palette_item palette_item = BROWN_PALETTE;
    if(selected) palette_item = RED_PALETTE;

    text_generator.set_palette_item(palette_item);
    text_generator.set_left_alignment();
    text_generator.generate(-90, ycor, _name, _text_sprites);

    text_generator.set_center_alignment();

}

}