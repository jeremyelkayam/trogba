#include <bn_log.h>
#include <bn_sprite_ptr.h>
#include <bn_keypad.h>
#include <bn_sprite_text_generator.h>
#include <bn_string.h>
#include <bn_sram.h>
#include <bn_version.h>
#include "instructions_scene.h"
#include "bn_sprite_items_titlegraphic.h"
#include "constants.h"

namespace trog {

instructions_scene::instructions_scene(session_info &sesh, bn::sprite_text_generator& text_generator) : 
        _secret_code_index(0),
        _text_generator(text_generator),
        _level_select(false),
        _show_secret_hints(false),
        _show_credits(false),
        _continue_menu_visible(false),
        _continue_selected(true),
        _sesh(sesh),
        _text_ycor(-30) {
    _flashing_text_counter = 0;

    for(int z = 0; z < 4 ; ++z){
        _title_sprites.push_back(bn::sprite_items::titlegraphic.create_sprite(TROG_TITLE_TEXT_X + 64*z, TROG_INSTRUCTION_TITLE_Y, z));
    }

    //set up konami code 
    _secret_code.emplace_back(bn::keypad::key_type::UP);
    _secret_code.emplace_back(bn::keypad::key_type::UP);
    _secret_code.emplace_back(bn::keypad::key_type::DOWN);
    _secret_code.emplace_back(bn::keypad::key_type::DOWN);
    _secret_code.emplace_back(bn::keypad::key_type::LEFT);
    _secret_code.emplace_back(bn::keypad::key_type::RIGHT);
    _secret_code.emplace_back(bn::keypad::key_type::LEFT);
    _secret_code.emplace_back(bn::keypad::key_type::RIGHT);
    _secret_code.emplace_back(bn::keypad::key_type::B);
    _secret_code.emplace_back(bn::keypad::key_type::A);


    // original styles
    // bn::string<64> instructions[] = {
    //     "Use the squishy + to move",
    //     "Stomp 10 peasants to achieve burnination.",
    //     "Burn all cottages to advance a level.",
    //     "Avoid knights and archers!",
    //     "",
    //     "Original by Jonathan Howe",
    //     "GBA version by Jeremy Elkayam",
    //     "Designed by Mike and Matt"
    // };
    text_generator.set_center_alignment();
    setup_instructions();


    // _text_generator.set_palette_item(bn::sprite_items::trogdor_variable_8x16_font.palette_item());

    // text_generator.generate(0, 30, , start_text_sprites);    

    // text_generator.generate(0, 40, , start_text_sprites);    

}

void instructions_scene::clear_text(){
    _instruction_text_sprites.clear();
    _start_text_sprites.clear();
    _text_ycor = -30;
}

void instructions_scene::setup_instructions(){
    bn::string<64> instructions[] = {
        "Use the squishy + to move",
        "Stomp 10 guys to burninate",
        "Burn all cottages to win",
        "Avoid knights and archers!",
        "",
        "",
        "OG by Mike/Matt/Jonathan",
        "GBA version by Jeremy Elkayam"
    };

    for(bn::string<64> line : instructions) { 
        write_instruction(line.c_str(), WHITE_PALETTE, 14);
    }


    _text_generator.set_palette_item(RED_PALETTE);
    _text_generator.generate(0, 33, "Press A to start 'em up ", _start_text_sprites);
}

void instructions_scene::setup_secret_hints(){

    write_instruction("SECRET HINTS!!", RED_PALETTE, 16);

    bn::string<64> instructions[] = {
        "-Don't let peasants return home",
        "-U're invincible while burning",
        "-Flaming peasants burn houses",
        "-Extra man every 300pts"
    };
    
    for(bn::string<64> line : instructions) { 
        write_instruction(line.c_str(), WHITE_PALETTE, 16);
    }
    
    write_instruction("-What's a treasure hut?!?!", RED_PALETTE, 16);
    write_instruction("-Secret Code?!?!", RED_PALETTE, 16);
}

void instructions_scene::setup_credits(){

    write_instruction("CREDITS!!", RED_PALETTE, 16);

    bn::string<64> credits[] = {
        "Designed by Mike and Matt",
        "Flash ver by Jonathan Howe",
        "HTML5 ver by Aeon Softworks",
        "Special thanks to",
        "Mips96 + TrogdorReburninated",
        "for inspiration/code reference"
    };

    for(bn::string<64> line : credits) { 
        write_instruction(line.c_str(), WHITE_PALETTE, 14);
    }    

    bn::string<32> butano_str;
    bn::ostringstream butano_string_stream(butano_str);
    butano_string_stream << "Created using Butano " << bn::version::major() << "." << bn::version::minor();

    write_instruction(butano_str.c_str(), RED_PALETTE, 14);
    write_instruction("by Gustavo Valiente", WHITE_PALETTE, 14);
}

void instructions_scene::write_instruction(const char* str, const bn::sprite_palette_item &palette, int line_spacing){
    _text_generator.set_palette_item(palette);
    _text_generator.generate(0, _text_ycor, str, _instruction_text_sprites);    
    _text_ycor += line_spacing;
}

bn::optional<scene_type> instructions_scene::update(){
    

    if(_level_select){
        short current_level = _sesh.get_level();
        if(bn::keypad::up_pressed()){
            current_level += 10;
        }
        if(bn::keypad::down_pressed()){
            current_level -= 10;
        }
        if(bn::keypad::right_pressed()){
            current_level += 1;
        }
        if(bn::keypad::left_pressed()){
            current_level -= 1;
        }
        if(current_level <= 0){
            current_level+=100;
        }else if(current_level > 100){
            current_level-=100;
        }
        _sesh.set_level(current_level);

        _start_text_sprites.clear();
        bn::string<10> lv_str;
        bn::ostringstream lv_string_stream(lv_str);
        lv_string_stream << "Lv " <<  _sesh.get_level();        
        _text_generator.set_palette_item(RED_PALETTE);
        _text_generator.generate(0, -30+(14*4)+7, lv_str, _start_text_sprites);    
    }else{
        if(!_show_secret_hints){
            _flashing_text_counter++;
            if(_flashing_text_counter > TROG_FLASHING_TEXT_VISIBLE_TIME){
                for(auto it : _start_text_sprites) { 
                    it.set_visible(false);
                }
            }
            if(_flashing_text_counter > TROG_FLASHING_TEXT_VISIBLE_TIME + TROG_FLASHING_TEXT_INVISIBLE_TIME){
                for(auto it : _start_text_sprites) { 
                    it.set_visible(true);
                }
                _flashing_text_counter = 0;
            }        
        }
        if(_continue_menu_visible){

            if(bn::keypad::left_pressed() || bn::keypad::right_pressed()) _continue_selected = !_continue_selected;

            _start_text_sprites.clear();

            if(_continue_selected){
                _text_generator.set_palette_item(GRAY_PALETTE);
                _text_generator.generate(-60, -30+(14*4)+7, "NEW GAME", _start_text_sprites);    

                _text_generator.set_palette_item(RED_PALETTE);
                _text_generator.generate(60, -30+(14*4)+7, "[CONTINUE]", _start_text_sprites);                    
            }else{
                _text_generator.set_palette_item(RED_PALETTE);
                _text_generator.generate(-60, -30+(14*4)+7, "[NEW GAME]", _start_text_sprites);    

                _text_generator.set_palette_item(GRAY_PALETTE);
                _text_generator.generate(60, -30+(14*4)+7, "CONTINUE", _start_text_sprites);                    
            }

        }
    }

    if(bn::keypad::any_pressed() && _secret_code_index < _secret_code.size()){
        if(bn::keypad::pressed(_secret_code.at(_secret_code_index))){
            ++_secret_code_index;
            BN_LOG("progress");
        }else{
            BN_LOG("you failed the code");
            _secret_code_index=0;
        }
    }

    // text stuff
    bn::optional<scene_type> result;
    

    if(bn::keypad::start_pressed()){
        #ifdef DEBUG 
            _level_select = !_level_select;
        #endif
    }

    if(bn::keypad::a_pressed()){
        if(_secret_code_index == _secret_code.size()){
            _sesh.secret_lives_boost();
        }
        session_info loaded_sesh;

        //Check to see if we have a saved session
        bn::sram::read(loaded_sesh);
        // Don't bother with loading if we are going to do the level select cheat
        if(!_level_select && loaded_sesh.is_valid_object()){
            if(!_continue_menu_visible){
                if(_show_secret_hints){
                    clear_text();
                    setup_instructions();
                }
                setup_continue_menu();

            }else{
                if(_continue_selected){
                    _sesh = loaded_sesh;
                }
                result = scene_type::PLAY;
            }
        }else{
            result = scene_type::PLAY;
        }

    }
    if(bn::keypad::r_pressed() && !_level_select && !_continue_menu_visible){
        //toggle secret hints
        clear_text();
        _show_secret_hints = !_show_secret_hints;
        if(_show_secret_hints){
            setup_secret_hints();
        }else{
            setup_instructions();
        }
    }

    return result;
}

void instructions_scene::setup_continue_menu(){
    BN_ASSERT(_continue_menu_visible == false, "Can only set up continue menu once");
    _continue_menu_visible = true;
    for(auto it : _start_text_sprites) { 
        it.set_visible(true);
    }
    _show_secret_hints = false;
}

}