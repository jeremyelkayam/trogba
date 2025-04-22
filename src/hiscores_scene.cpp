#include <bn_keypad.h>
#include <bn_sound_items.h>
#include <bn_log.h>
#include <bn_string.h>

#include "bn_regular_bg_items_hi_scores_bg.h"
#include "hiscores_scene.h"
#include "constants.h"
#include "serif_fonts.h"

namespace trog {

hiscores_scene::hiscores_scene(session_info &sesh, common_stuff &common_stuff, const scene_type &last_scene) : 
        _selectable_letters({'A','B','C','D','E','F','G','H','I','J','K','L','M','N',
                             'O','P','Q','R','S','T','U','V','W','X','Y','Z',
                             '0','1','2','3','4','5','6','7','8','9','.',',','!','?',' '}),
        _sesh(sesh),
        _common_stuff(common_stuff),
        _fw_gen(serif_font_brown_fixed),
        _vw_gen(serif_font_brown),
        _red_gen(serif_font_red),
        _last_scene(last_scene),
        _cursor_sprite(
            bn::sprite_items::serif_font_brown.create_sprite(130,0,62)),
        _scroll(bn::regular_bg_items::hi_scores_bg.create_bg(8, 64)), 
        _high_scores_table(_common_stuff.savefile.high_scores_table),
        _table_index(-1),
        _string_index(0),
        _selectable_letters_index(_selectable_letters.size() - 1),
        _blink_timer(0),
        _go_to_credits(false) {
    
    _fw_gen.set_z_order(1);
    _vw_gen.set_z_order(1);
    _red_gen.set_z_order(1);

    if(_sesh.get_level() == 101){
        //101 isn't a real level. If you beat the game, the game ends before you start level 101.
        // So we count you as ending on level 100.
        // Maybe add a crown or something if you beat the game? 
        _go_to_credits = true;
    }    
    
    _red_gen.set_center_alignment();

    if(last_scene != scene_type::PLAY && last_scene != scene_type::MENU){

        //Propagate the player's score within the scores list (if applicable)
        if(_sesh.get_score() > _high_scores_table[7].get_score()){
            BN_LOG("you got a high score: ", _sesh.get_score());
            _high_scores_table[7] = high_score_entry("         ", _sesh.get_level(), _sesh.get_score());
            _table_index=7;
            for(int z = 6; z >= 0; --z){
                high_score_entry& current = _high_scores_table[z];
                high_score_entry& previous = _high_scores_table[z+1];
                if(previous.get_score() > current.get_score()){
                    bn::swap(previous, current);
                    _table_index=z;
                }
            }
            //move our cursor into position
            _cursor_sprite.set_y(-42 + _table_index*15);
        }


        BN_LOG("score", _sesh.get_score());
        BN_LOG("score entry at index ", _table_index);
        _cursor_sprite.set_visible(false);

        _sesh.reset();
    }
    draw_high_scores_table();
}

void hiscores_scene::draw_high_scores_table(){
    _text_sprites.clear();

    bn::string<64> title_text;
    if(_table_index > -1){
        title_text = "ENTER YOUR NAME!";
    }else{
        title_text = "YE OLDE SCROLL OF HI-SCORES";
    }

    _red_gen.generate(0, -72, title_text, _header_sprites);

    _vw_gen.set_center_alignment();
    _vw_gen.generate(-52, -55, "name", _text_sprites);
    _vw_gen.generate(16, -55, "level", _text_sprites);
    _vw_gen.generate(68, -55, "score", _text_sprites);


    for(int z = 0; z < _high_scores_table.size(); ++z){
        _vw_gen.set_right_alignment();
        bn::string<3> place;
        bn::ostringstream place_string_stream(place);
        place_string_stream << z + 1 << ".";

        _vw_gen.generate(-76, -43 + z*15, place, _text_sprites);   

        _vw_gen.set_left_alignment();
        _vw_gen.generate(-74, -43 + z*15, _high_scores_table.at(z).get_name(), _text_sprites);   

        if(_high_scores_table.at(z).get_score() != 0){
            _vw_gen.set_center_alignment();
            bn::string<3> level;
            bn::ostringstream lv_string_stream(level);
            lv_string_stream << _high_scores_table.at(z).get_level();

            bn::string<12> score;
            bn::ostringstream score_string_stream(score);
            score_string_stream << _high_scores_table.at(z).get_score();

            _vw_gen.generate(16, -43 + z*15, level, _text_sprites);
            _vw_gen.generate(68, -43 + z*15, score, _text_sprites);
        }
    }
}

bn::optional<scene_type> hiscores_scene::update(){
    bn::optional<scene_type> result;

    if(_table_index != -1){
        draw_name_entry();
        update_name_entry();
        if(bn::keypad::start_pressed()){
            end_name_entry();
        }
    }else if(bn::keypad::start_pressed() || bn::keypad::a_pressed()|| bn::keypad::b_pressed()){
        if(_go_to_credits){
            result = scene_type::CREDITS;
        }
        else if(_last_scene == scene_type::LOSE)
        {
            result = scene_type::MENU;
        }
        else
        {
            result = _last_scene;
        }
        _text_sprites.clear();
    }    
    return result;
}

void hiscores_scene::draw_name_entry(){
    BN_ASSERT(_table_index != -1);
    _name_entry_sprites.clear();

    if(_blink_timer == 15){
        _cursor_sprite.set_visible(true);
    }else if(_blink_timer == 0){
        _cursor_sprite.set_visible(false);
    }


    _cursor_sprite.set_x(-70 + _string_index * 8);

    _fw_gen.generate(-74, -43 + _table_index*15, _high_scores_table.at(_table_index).get_name(), _name_entry_sprites);   
}

void hiscores_scene::update_name_entry(){
    ++_blink_timer;

    if(_nothing_pressed_timer < 15 SECONDS) ++_nothing_pressed_timer;
    if(_blink_timer > 30){
        _blink_timer = 0;
    }

    high_score_entry &current_entry = _high_scores_table[_table_index];
    
    BN_ASSERT(0 <= _table_index && _table_index < 8,"Valid index required...");

    //Redraw the table when strictly necessary (e.g. changing letters
    // or when the cursor blinks)

    if(bn::keypad::any_pressed()) _nothing_pressed_timer = 0;
    
    if(_nothing_pressed_timer == 10 SECONDS){
        _header_sprites.clear();
        _red_gen.generate(0, -72, "PRESS START WHEN YORE DONE", _header_sprites);
    }

    if(bn::keypad::a_pressed()){
        ++_string_index;
        
        if(_string_index == 9){
            end_name_entry();
            return;
        }else{
            set_selectable_chars_index_to_current_char_in_str();
        }
    }else if(bn::keypad::right_pressed()){
        if(_string_index < 8){
            ++_string_index;
            set_selectable_chars_index_to_current_char_in_str();
        }
    }else if(bn::keypad::b_pressed()){
        if(_string_index != 0){
            --_string_index;
            _selectable_letters_index = _selectable_letters.size() - 1;
        }
    }else if(bn::keypad::left_pressed()){
        if(_string_index != 0){
            --_string_index;
            set_selectable_chars_index_to_current_char_in_str();
        }
    }
    if(bn::keypad::down_pressed()){
        ++_selectable_letters_index;
    }
    if(bn::keypad::up_pressed()){
        --_selectable_letters_index;
    }
    if(bn::keypad::down_held() || bn::keypad::up_held()){
        ++_hold_down_timer;

        if(_hold_down_timer >= 30 && _hold_down_timer % 3 == 0){

            if(bn::keypad::down_held())
                ++_selectable_letters_index;
            else
                --_selectable_letters_index;            
        }
        //just keep holding if you hold for more than 4 seconds
        if(_hold_down_timer >=210) _hold_down_timer = 30;
    }

    if(bn::keypad::down_released() || bn::keypad::up_released()) _hold_down_timer = 0;
    _selectable_letters_index = (_selectable_letters_index + _selectable_letters.size()) % _selectable_letters.size();

    current_entry.set_name_char(_selectable_letters[_selectable_letters_index], _string_index);
}


void hiscores_scene::end_name_entry(){
    _table_index = -1;
    _common_stuff.save();
    _cursor_sprite.set_visible(false);

    _header_sprites.clear();
    
    _red_gen.generate(0, -72, "YE OLDE SCROLL OF HI-SCORES", _header_sprites);
}


void hiscores_scene::set_selectable_chars_index_to_current_char_in_str(){
    char &current_char = _high_scores_table[_table_index].get_name().at(_string_index);
    if('A' <= current_char && current_char <= 'Z' ){
        _selectable_letters_index = current_char - 65;
    }if('0' <= current_char && current_char <= '9' ){
        _selectable_letters_index = current_char - 22;
    }else if(current_char == ' '){
        _selectable_letters_index = _selectable_letters.size() - 1;
    }else if(current_char == '?'){
        _selectable_letters_index = _selectable_letters.size() - 2;
    }else if(current_char == '!'){
        _selectable_letters_index = _selectable_letters.size() - 3;
    }else if(current_char == ','){
        _selectable_letters_index = _selectable_letters.size() - 4;
    }else if(current_char == '.'){
        _selectable_letters_index = _selectable_letters.size() - 5;
    }
}


}