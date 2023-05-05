#include <bn_keypad.h>
#include <bn_sound_items.h>
#include <bn_log.h>
#include <bn_sram.h>
#include <bn_string.h>

#include "bn_regular_bg_items_hi_scores_bg.h"
#include "hiscores_scene.h"
#include "constants.h"

namespace trog {

hiscores_scene::hiscores_scene(session_info &sesh) : 
        _selectable_letters({'A','B','C','D','E','F','G','H','I','J','K','L','M','N',
                             'O','P','Q','R','S','T','U','V','W','X','Y','Z','!','?', ' '}),
        _sesh(sesh),
        _text_generator(fixed_8x16_sprite_font),
        _cursor_sprite(bn::sprite_items::trogdor_fixed_8x16_font.create_sprite(0,0,62)),
        _scroll(bn::regular_bg_items::hi_scores_bg.create_bg(8, 64)), 
        _high_scores_table(
            {high_score_entry("", 0, 0),
            high_score_entry("", 0, 0),
            high_score_entry("", 0, 0),
            high_score_entry("", 0, 0),
            high_score_entry("", 0, 0),
            high_score_entry("", 0, 0),
            high_score_entry("", 0, 0),
            high_score_entry("", 0, 0)}),
        _table_index(-1),
        _string_index(0),
        _selectable_letters_index(28),
        _blink_timer(0),
        _go_to_credits(false) {
    
    //Initialize our format tag
    bn::istring_base expected_format_tag_istring(_format_tag._data);
    bn::ostringstream expected_format_tag_stream(expected_format_tag_istring);
    expected_format_tag_stream.append("TROG_HISCORES");

    //Load the table if applicable
    load_high_scores_table();
    
    if(_sesh.get_level() == 101){
        //101 isn't a real level. If you beat the game, the game ends before you start level 101.
        // So we count you as ending on level 100.
        // Maybe add a crown or something if you beat the game? 
        _go_to_credits = true;
    }

    //Propagate the player's score within the scores list (if applicable)
    if(_sesh.get_score() > _high_scores_table[7].get_score()){
        BN_LOG("you got a high score: ", _sesh.get_score());
        _high_scores_table[7] = high_score_entry("         ", _sesh.get_level(), _sesh.get_score());
        _table_index=7;
        for(int z = 6; z >= 0; --z){
            high_score_entry& current = _high_scores_table[z];
            high_score_entry& previous = _high_scores_table[z+1];
            if(previous.get_score() >= current.get_score()){
                bn::swap(previous, current);
                _table_index=z;
            }
        }
        //move our cursor into position
        _cursor_sprite.set_y(-42 + _table_index*15);
    }


    BN_LOG("score", _sesh.get_score());
    BN_LOG("score entry at index ", _table_index);
    _cursor_sprite.set_palette(BROWN_PALETTE);
    _cursor_sprite.set_visible(false);

    draw_high_scores_table();
    _sesh.reset();

}

void hiscores_scene::draw_high_scores_table(){
    _text_sprites.clear();

    bn::string<64> title_text;
    if(_table_index > -1){
        title_text = "ENTER YOUR NAME!";
    }else{
        title_text = "YE OLDE SCROLL OF HI-SCORES";
    }

    _text_generator.set_center_alignment();
    _text_generator.set_palette_item(RED_PALETTE);
    _text_generator.generate(0, -72, title_text, _header_sprites);

    _text_generator.set_palette_item(BROWN_PALETTE);
    _text_generator.generate(-52, -55, "name", _text_sprites);
    _text_generator.generate(16, -55, "level", _text_sprites);
    _text_generator.generate(68, -55, "score", _text_sprites);


    for(int z = 0; z < _high_scores_table.size(); ++z){
        _text_generator.set_right_alignment();
        bn::string<3> place;
        bn::ostringstream place_string_stream(place);
        place_string_stream << z + 1 << ".";

        _text_generator.generate(-76, -43 + z*15, place, _text_sprites);   

        _text_generator.set_left_alignment();
        _text_generator.generate(-74, -43 + z*15, _high_scores_table.at(z).get_name(), _text_sprites);   

        if(_high_scores_table.at(z).get_score() != 0){
            _text_generator.set_center_alignment();
            bn::string<3> level;
            bn::ostringstream lv_string_stream(level);
            lv_string_stream << _high_scores_table.at(z).get_level();

            bn::string<12> score;
            bn::ostringstream score_string_stream(score);
            score_string_stream << _high_scores_table.at(z).get_score();

            _text_generator.generate(16, -43 + z*15, level, _text_sprites);
            _text_generator.generate(68, -43 + z*15, score, _text_sprites);
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
    }else if(bn::keypad::start_pressed() || bn::keypad::a_pressed()){
        if(_go_to_credits){
            result = scene_type::CREDITS;
        }else{
            result = scene_type::MENU;
        }
        _text_sprites.clear();
    }    
    return result;
}

void hiscores_scene::draw_name_entry(){
    BN_ASSERT(_table_index != -1);
    _name_entry_sprites.clear();
    _text_generator.set_left_alignment();

    if(_blink_timer == 15){
        _cursor_sprite.set_visible(true);
    }else if(_blink_timer == 0){
        _cursor_sprite.set_visible(false);
    }
    _cursor_sprite.set_x(-70 + _string_index * 8);

    _text_generator.generate(-74, -43 + _table_index*15, _high_scores_table.at(_table_index).get_name(), _name_entry_sprites);   
}

void hiscores_scene::update_name_entry(){
    ++_blink_timer;
    if(_blink_timer > 30){
        _blink_timer = 0;
    }

    high_score_entry &current_entry = _high_scores_table[_table_index];
    
    BN_ASSERT(0 <= _table_index && _table_index < 8,"Valid index required...");

    //Redraw the table when strictly necessary (e.g. changing letters
    // or when the cursor blinks)


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
            _selectable_letters_index = 28;
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

        if(_hold_down_timer >= 30 && _hold_down_timer % 6 == 0){

            if(bn::keypad::down_held())
                ++_selectable_letters_index;
            else
                --_selectable_letters_index;            
        }
        //just keep holding if you hold for more than 4 seconds
        if(_hold_down_timer >=210) _hold_down_timer = 30;
    }

    if(bn::keypad::down_released() || bn::keypad::up_released()) _hold_down_timer = 0;
    _selectable_letters_index = (_selectable_letters_index + _selectable_letters.max_size()) % _selectable_letters.max_size();

    current_entry.set_name_char(_selectable_letters[_selectable_letters_index], _string_index);
}


void hiscores_scene::end_name_entry(){
    _table_index = -1;
    save_high_scores_table();
    _cursor_sprite.set_visible(false);

    _header_sprites.clear();
    
    _text_generator.set_center_alignment();
    _text_generator.set_palette_item(RED_PALETTE);
    _text_generator.generate(0, -72, "YE OLDE SCROLL OF HI-SCORES", _header_sprites);
}


void hiscores_scene::set_selectable_chars_index_to_current_char_in_str(){
    char &current_char = _high_scores_table[_table_index].get_name().at(_string_index);
    if('A' <= current_char && current_char <= 'Z' ){
        _selectable_letters_index = current_char - 65;
    }else if(current_char == ' '){
        _selectable_letters_index = 28;
    }else if(current_char == '!'){
        _selectable_letters_index = 26;
    }else if(current_char == '?'){
        _selectable_letters_index = 27;
    }
}

void hiscores_scene::load_high_scores_table(){

    bn::array<char, 16> loaded_format_tag;
    bn::sram::read_offset(loaded_format_tag, sizeof(_sesh));

    if(_format_tag == loaded_format_tag){
        bn::sram::read_offset(_high_scores_table, sizeof(_sesh) + sizeof(_format_tag));
    }
}
void hiscores_scene::save_high_scores_table(){
    bn::sram::write_offset(_format_tag, sizeof(_sesh));
    bn::sram::write_offset(_high_scores_table, sizeof(_sesh) + sizeof(_format_tag));
}

high_score_entry::high_score_entry(bn::string<9> name, unsigned short level, unsigned short score) : 
    _level(level),
    _score(score) {

    bn::istring_base name_istring(_name._data);
    bn::ostringstream name_stream(name_istring);
    name_stream.append(name);
}

high_score_entry::high_score_entry() : 
    _level(0),
    _score(0) {

    bn::istring_base name_istring(_name._data);
    bn::ostringstream name_stream(name_istring);
    name_stream.append("DUMMY");
}

bn::string<9> high_score_entry::get_name(){
    bn::string<9>result;
    //go until you start hitting null chars
    for(int z=0; _name[z] != 0; ++z){
        result.append(_name[z]);
    }

    return result;
}

}