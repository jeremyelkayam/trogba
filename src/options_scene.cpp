#include <bn_keypad.h>
#include <bn_string.h>
#include <bn_log.h>

#include "options_scene.h"
#include "constants.h"

#include "bn_regular_bg_items_hi_scores_bg.h"
#include "bn_sprite_items_trogdor_variable_8x8_font.h"
#include "bn_sprite_items_checkbox.h"
#include "bn_sprite_items_slider_bar.h"
#include "bn_sprite_items_volume_graph.h"
#include "small_fonts.h"
#include "serif_fonts.h"

namespace trog {

options_scene::options_scene(common_stuff &common_stuff, const scene_type &last_scene) : 
        _common_stuff(common_stuff),
        _last_scene(last_scene),
        _scroll(bn::regular_bg_items::hi_scores_bg.create_bg(8, 64)),
        _black_generator(small_font_black),
        _red_generator(small_font_red),
        _index(0),
        _old_save(common_stuff.savefile.options) {
    bn::sprite_text_generator serif_gen(serif_font_red); 
    serif_gen.set_center_alignment();
    serif_gen.generate(0, -72, "YE OLDE OPTIONS MENU", _header_sprites);

    _options_vec.emplace_back(
        new bool_option("Troghammer", 
            "Toggles the Troghammer: a new, tougher variant of the knight that appears if you take too long to beat a level.", 
            _red_generator, _black_generator, ycor(0),
            _common_stuff.savefile.options.troghammer));
    _options_vec.emplace_back(
        new bool_option("Trogmeter Depreciation",
            "If enabled, the Trog-Meter will decrease by one every time an unburninated peasant returns to their home cottage.", 
            _red_generator, _black_generator, ycor(1),
            _common_stuff.savefile.options.decrement_trogmeter));

    bn::vector<uint8_t,4 > lives_options;
    lives_options.emplace_back(0);
    lives_options.emplace_back(3);
    lives_options.emplace_back(5);

    if(_common_stuff.acm.is_achieved("konami"))
        lives_options.emplace_back(30);

    _options_vec.emplace_back(
        new selector_option("Starting Lives", "Controls the number of lives Trogdor starts out with.", 
            _red_generator, _black_generator, ycor(2), 
            _common_stuff.savefile.options.starting_lives, 
            lives_options));
    _options_vec.emplace_back(
        new percent_option("Music Volume", "Controls the volume of any music playing.", 
            _red_generator, _black_generator, ycor(3), 
            _common_stuff.savefile.options.music_vol));
    _options_vec.emplace_back(
        new percent_option("Sound Volume", "Controls the volume of any sound effects playing.", 
            _red_generator, _black_generator, ycor(4), 
            _common_stuff.savefile.options.sound_vol));
    _options_vec.emplace_back(
        new percent_option("Voice Volume", "Controls the volume of any voice clips playing.", 
            _red_generator, _black_generator, ycor(5), 
            _common_stuff.savefile.options.voice_vol));
    _options_vec.emplace_back(
        new option("confirm", "Save changes and return.", 
            _red_generator, _black_generator, ycor(6)));
    _options_vec.emplace_back(
        new option("cancel", "Exit without saving changes.", 
            _red_generator, _black_generator, ycor(7)));
    _options_vec.at(_index)->set_selected(true);

    _black_generator.set_center_alignment();
    _black_generator.generate(0, 65, "Press L or R for more information.", 
        _notice_sprites);
}

bn::optional<scene_type> options_scene::update(){
    bn::optional<scene_type> result;

    if(!_options_vec.empty()){
        //this logic pattern is kinda lame but idk what to do bout it 
        if(bn::keypad::up_pressed() || bn::keypad::down_pressed()){

            _options_vec.at(_index)->set_selected(false);

            if(bn::keypad::up_pressed()){
                if(_index == 0) _index = _options_vec.size();
                _index--;
            }else if(bn::keypad::down_pressed()){
                _index++;
                if(_index == _options_vec.size()) _index = 0;
            }

            _options_vec.at(_index)->set_selected(true);

            if(_description_box && !_description_box->is_off_screen()){
                _description_box->set_text(_options_vec.at(_index)->get_description().c_str());
            }


        }else if(bn::keypad::l_pressed() || bn::keypad::r_pressed()) {
            //might be weird but it prevents making the thing judder by spamming L and R
            //less responsive though idk
            if(!_description_box){
                _description_box.emplace();
            }
            if(_description_box->is_off_screen()){
                _description_box->move_on_screen();
                _description_box->set_text(_options_vec.at(_index)->get_description().c_str());
            }else{
                _description_box->move_off_screen();
            }
        }

        _options_vec.at(_index)->update();
        if(_description_box) _description_box->update();
        
        
        if((bn::keypad::a_pressed() && _index == _options_vec.size() -2)){
            _description_box.reset();
            _notice_sprites.clear();

            bool troghammer_changed = _old_save.troghammer != _common_stuff.savefile.options.troghammer;
            bool trogmeter_changed = _old_save.decrement_trogmeter != _common_stuff.savefile.options.decrement_trogmeter;
            bool lives_changed = _old_save.starting_lives != _common_stuff.savefile.options.starting_lives;

            if(troghammer_changed || trogmeter_changed || lives_changed) {
                bn::vector<bn::string<32>, 5>  _changed_settings;
                if(troghammer_changed) _changed_settings.emplace_back("Troghammer");
                if(trogmeter_changed) _changed_settings.emplace_back("Trogmeter depreciation");
                if(lives_changed) _changed_settings.emplace_back("Starting lives");
                _options_vec.clear();

                _red_generator.set_center_alignment();
                _red_generator.generate(0, -50, "IMPORTANT!", _notice_sprites);

                _black_generator.set_center_alignment();
                _black_generator.generate(0, -35, "Changes to the following settings:", _notice_sprites);
                bn::fixed ycor = -15;
                for(bn::string<32> &change : _changed_settings){
                    _black_generator.generate(0, ycor, change, _notice_sprites);
                    ycor+=10;
                }

                _black_generator.generate(0, 40, "will be applied the next time ", _notice_sprites);
                _black_generator.generate(0, 50, "you start a new game.", _notice_sprites);

                _black_generator.generate(0, 65, "Press A to continue", _notice_sprites);
            }else{
                _common_stuff.save();
                result = _last_scene;
            }

            

        }else if(bn::keypad::b_pressed() || (bn::keypad::a_pressed() &&_index == _options_vec.size() -1)){
            //revert to the saved data;
            _description_box.reset();
            _common_stuff.savefile.options = _old_save;
            result = _last_scene;
        }
    }else{
        if(bn::keypad::a_pressed()){
            _common_stuff.save();
            _description_box.reset();
            result = _last_scene;
        }
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





bool_option::bool_option(const bn::string<32> &name, 
    const bn::string<128> &description, 
    bn::sprite_text_generator &red_generator, 
    bn::sprite_text_generator &black_generator, 
    const bn::fixed &ycor, bool &value) : 
    option(name, description, red_generator, black_generator, ycor),
    _value(value),
    _checkbox(bn::sprite_items::checkbox.create_sprite(
        80, ycor, value)) 
{
}


void bool_option::update()
{
    if(bn::keypad::a_pressed() || bn::keypad::left_pressed() || 
        bn::keypad::right_pressed()){
        _value = !_value;
        _checkbox.set_tiles(bn::sprite_items::checkbox.tiles_item(), 
        2 + _value);
    }
}

void bool_option::set_selected(const bool &selected){
    option::set_selected(selected);
    int gfx_index = (_value ? 1 : 0) + (selected ? 2 : 0);
    _checkbox.set_tiles(
        bn::sprite_items::checkbox.tiles_item(),
        gfx_index
    );
    
}

percent_option::percent_option(const bn::string<32> &name, 
    const bn::string<128> &description, 
    bn::sprite_text_generator &red_generator, 
    bn::sprite_text_generator &black_generator, 
    const bn::fixed &ycor, bn::fixed &value) : 
    option(name, description, red_generator, black_generator, ycor),
    // _options(options),
    _value(value),
    _speed(0.01),
    _slider_bar(bn::sprite_items::slider_bar.create_sprite(0,ycor)),
    _hold_timer(0){
    
    for(int z = 0; z < _vol_graph.max_size() ; ++z){
        _vol_graph.push_back(bn::sprite_items::volume_graph.create_sprite(32*z, ycor, z));
        _vol_graph.at(z).put_below();
    }
    set_selected(false);
}

void percent_option::update(){
    
    if(bn::keypad::right_held() || bn::keypad::left_held()){
        _hold_timer++;
    }else if(bn::keypad::right_released() || 
        bn::keypad::left_released()){
        _hold_timer = 0;
    }

    if(bn::keypad::right_pressed() || 
        (bn::keypad::right_held() && _hold_timer > 20)){
        _value += _speed;
    }
    else if(bn::keypad::left_pressed() || 
        (bn::keypad::left_held() && _hold_timer > 20)){
        _value -= _speed;
    }


    if(_value < 0) _value = 0;
    if(_value > 1) _value = 1;
    update_text_and_slider(true);
}

void percent_option::update_text_and_slider(const bool &selected){
    _vol_text_sprites.clear();
    bn::sprite_text_generator &txtgen = 
        selected ? _red_generator : _black_generator;
        txtgen.set_left_alignment();

    bn::string<16> percentage;
    bn::ostringstream pct_stream(percentage);

    pct_stream << (_value * 100).integer() << "%";           

    txtgen.generate(72, _slider_bar.y(), 
        percentage, _vol_text_sprites);
    _slider_bar.set_x(-16 + 74 * _value);
}

void percent_option::set_selected(const bool &selected){
    option::set_selected(selected);

    update_text_and_slider(false);

    _slider_bar.set_tiles(bn::sprite_items::slider_bar.tiles_item(),
        selected);
}

option::option(const bn::string<32> &name, 
    const bn::string<128> &description, 
    bn::sprite_text_generator &red_generator, 
    bn::sprite_text_generator &black_generator, 
    const bn::fixed &ycor) : 
        _name(name),
        _description(description),
        _red_generator(red_generator),
        _black_generator(black_generator),
        _ycor(ycor)
{ 
    regen_text(_black_generator);
}

void option::set_selected(const bool &selected)
{
    regen_text(selected ? _red_generator : _black_generator);
}

void option::regen_text(bn::sprite_text_generator &gen)
{
    _text_sprites.clear();
    gen.set_left_alignment();
    gen.generate(-90, _ycor, _name, _text_sprites);

}


uint8_t selector_option::current_index(){
    for(uint8_t z = 0; z < _selector_options.size(); z++){
        if(_selector_options.at(z) == _value) return z;
    }
    BN_ERROR("Current value not in option list: ", _value);
    return -1;
}

selector_option::selector_option(const bn::string<32> &name, 
    const bn::string<128> &description, 
    bn::sprite_text_generator &red_generator, 
    bn::sprite_text_generator &black_generator, 
    const bn::fixed &ycor, uint8_t &value, 
    const bn::ivector<uint8_t> &selector_options) : 
        option(name, description, red_generator, 
            black_generator, ycor),
        _value(value),
        _timer(0),
        _left_sprite(bn::sprite_items::small_font_red.create_sprite(0, ycor, 27)),
        _right_sprite(bn::sprite_items::small_font_red.create_sprite(0, ycor, 29)),
        _selector_options(selector_options)
{
    set_selected(false);
}

void selector_option::update(){
    uint8_t index = current_index();

    update_text(true);

    if(bn::keypad::right_pressed() && index < _selector_options.size() - 1){
        _value = _selector_options.at(index + 1);
    }
    else if(bn::keypad::left_pressed() && index > 0){
        _value = _selector_options.at(index - 1);
    }

}

void selector_option::update_text(const bool &is_selected){ 
    _selector_text_sprites.clear();
    bn::string<16> selected;
    bn::ostringstream stream(selected);

    stream << _value;            

    bn::sprite_text_generator &txtgen = is_selected ? _red_generator : 
        _black_generator;
    txtgen.set_center_alignment();
    txtgen.generate(80, _text_sprites.at(0).y(), 
        selected, _selector_text_sprites);

    _left_sprite.set_visible(current_index() != 0);
    _right_sprite.set_visible(current_index() != 
        _selector_options.size() - 1);

    if(bn::keypad::left_pressed()){
        _timer = -1;
    }else if(bn::keypad::right_pressed()){
        _timer = 1;
    }
    


    if(_timer < 0){
        _left_sprite.set_x(80 - 10);
        --_timer;
    }else{
        _left_sprite.set_x(80 - 8);
    }
    if(_timer > 0){
        _right_sprite.set_x(80 + 10);
        ++_timer;
    }else{
        _right_sprite.set_x(80 + 8);
    }

    if(_timer < -5 || _timer > 5 ) _timer = 0;

}

void selector_option::set_selected(const bool &selected){
    option::set_selected(selected);
    
    update_text(selected);
    
    _left_sprite.set_visible(selected);
    _right_sprite.set_visible(selected);
}

}