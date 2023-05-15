#include <bn_keypad.h>
#include <bn_string.h>

#include "bn_regular_bg_items_hi_scores_bg.h"
#include "options_scene.h"
#include "constants.h"

#include "bn_sprite_items_trogdor_variable_8x16_font_red.h"
#include "bn_sprite_items_trogdor_variable_8x16_font_brown.h"
#include "bn_sprite_items_checkbox.h"
#include "bn_sprite_items_slider_bar.h"
#include "bn_sprite_items_volume_graph.h"

namespace trog {

options_scene::options_scene(common_stuff &common_stuff) : 
        _common_stuff(common_stuff),
        _scroll(bn::regular_bg_items::hi_scores_bg.create_bg(8, 64)),
        _index(0) {
    _common_stuff.text_generator.set_center_alignment();
    _common_stuff.text_generator.set_palette_item(RED_PALETTE);
    _common_stuff.text_generator.generate(0, -72, "YE OLDE OPTIONS MENU", _header_sprites);

    bn::sprite_text_generator &txtgen = common_stuff.small_generator;

    _options_vec.emplace_back(new bool_option("Troghammer", txtgen, ycor(0), _common_stuff.savefile.troghammer));
    _options_vec.emplace_back(new bool_option("Trogmeter Depreciation", txtgen, ycor(1), _common_stuff.savefile.decrement_trogmeter));
    _options_vec.emplace_back(new percent_option("Music Volume", txtgen, ycor(2), _common_stuff.savefile.music_vol));
    _options_vec.emplace_back(new percent_option("Sound Volume", txtgen, ycor(3), _common_stuff.savefile.sound_vol));
    _options_vec.emplace_back(new percent_option("Voice Volume", txtgen, ycor(4), _common_stuff.savefile.voice_vol));
    _options_vec.emplace_back(new option("back", txtgen, ycor(5)));

}

bn::optional<scene_type> options_scene::update(){
    bn::optional<scene_type> result;

    if(bn::keypad::up_pressed()){
        _options_vec.at(_index)->set_selected(false);
        if(_index == 0) _index = _options_vec.size();
        _index--;
        _options_vec.at(_index)->set_selected(true);
    }else if(bn::keypad::down_pressed()){
        _options_vec.at(_index)->set_selected(false);
        _index++;
        if(_index == _options_vec.size()) _index = 0;
        _options_vec.at(_index)->set_selected(true);
    }

    _options_vec.at(_index)->update();
    
    
    if(bn::keypad::b_pressed() || (bn::keypad::a_pressed() && _index == _options_vec.size() - 1)){
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
        else result+=12;
    }
    return result;
}





bool_option::bool_option(const bn::string<32> &name, bn::sprite_text_generator &text_generator, const bn::fixed &ycor, bool &value) : 
    option(name, text_generator, ycor),
    _value(value),
    _checkbox(bn::sprite_items::checkbox.create_sprite(80, ycor, value)) {
}


void bool_option::update(){
    if(bn::keypad::a_pressed()) _value = !_value;
    _checkbox.set_item(bn::sprite_items::checkbox, _value);
}

void bool_option::set_selected(const bool &selected){
    option::set_selected(selected);
    _checkbox.set_palette(_text_sprites.at(0).palette());
}

percent_option::percent_option(const bn::string<32> &name, bn::sprite_text_generator &text_generator, const bn::fixed &ycor, bn::fixed &value) : 
    option(name, text_generator, ycor),
    // _options(options),
    _value(value),
    _slider_bar(bn::sprite_items::slider_bar.create_sprite(0,ycor)){
    
    for(int z = 0; z < _vol_graph.max_size() ; ++z){
        _vol_graph.push_back(bn::sprite_items::volume_graph.create_sprite(32*z, ycor, z));
        _vol_graph.at(z).put_below();
    }
    update_text_and_slider();
    set_selected(false);
}

void percent_option::update(){
    //todo make it start slow and speed up
    if(bn::keypad::right_held()){
        _value += 0.01;
    }
    else if(bn::keypad::left_held()){
        _value -= 0.01;
    }

    if(_value < 0) _value = 0;
    if(_value > 1) _value = 1;
    update_text_and_slider();
}

// uint8_t percent_option::current_index(){
//     for(uint8_t z = 0; z < _options.size(); z++){
//         if(_options.at(z) == _value) return z;
//     }
//     BN_ERROR("Current value not in option list: ", _value);
//     return -1;
// }

void percent_option::update_text_and_slider(){
    _vol_text_sprites.clear();
    _text_generator.set_left_alignment();

    bn::string<16> percentage;
    bn::ostringstream pct_stream(percentage);

    pct_stream << (_value * 100).integer() << "%";;            

    _text_generator.set_palette_item(RED_PALETTE);
    _text_generator.generate(72, _slider_bar.y(), percentage, _vol_text_sprites);
    _slider_bar.set_x(-16 + 74 * _value);
}

void percent_option::set_selected(const bool &selected){
    option::set_selected(selected);
    const bn::sprite_palette_ptr &palette = _text_sprites.at(0).palette();

    for(bn::sprite_ptr &s : _vol_text_sprites) { 
        s.set_palette(palette);
    }
    _slider_bar.set_palette(palette);
}

option::option(const bn::string<32> &name, bn::sprite_text_generator &text_generator, const bn::fixed &ycor) : 
    _name(name), _text_generator(text_generator) { 
    text_generator.set_palette_item(BROWN_PALETTE);
    text_generator.set_left_alignment();
    text_generator.generate(-90, ycor, _name, _text_sprites);
}

void option::set_selected(const bool &selected){
    bn::sprite_palette_item palette_item = BROWN_PALETTE;
    if(selected) palette_item = RED_PALETTE;
    for(bn::sprite_ptr &s : _text_sprites) {
        s.set_palette(palette_item);
    }
}

}