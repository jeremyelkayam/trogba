#include <bn_sound_items.h>
#include <bn_log.h>
#include <bn_sprite_ptr.h>
#include <bn_keypad.h>
#include <bn_sprite_text_generator.h>
#include <bn_string.h>
#include "instructions_scene.h"
#include "bn_sprite_items_titlegraphic.h"
#include "constants.h"

#define RED_PALETTE bn::sprite_items::trogdor_variable_8x16_font_red.palette_item()
#define WHITE_PALETTE bn::sprite_items::trogdor_variable_8x16_font.palette_item()


namespace trog {

instructions_scene::instructions_scene(session_info &sesh, bn::sprite_text_generator& text_generator) : 
        _text_generator(text_generator),
        _level_select(false),
        _show_secret_hints(false),
        _sesh(sesh),
        _text_ycor(-30) {
    _flashing_text_counter = 0;

    for(int z = 0; z < 4 ; ++z){
        _title_sprites.push_back(bn::sprite_items::titlegraphic.create_sprite(TROG_TITLE_TEXT_X + 64*z, TROG_INSTRUCTION_TITLE_Y, z));
    }

    bn::sound_items::trogador.play(TROG_DEFAULT_VOLUME);

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
        _text_generator.set_palette_item(bn::sprite_items::trogdor_variable_8x16_font_red.palette_item());
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
    }

    // text stuff
    bn::optional<scene_type> result;
    
    if(bn::keypad::a_pressed()){
        #ifdef DEBUG
            if(_level_select) result = scene_type::PLAY;
            clear_text();
            _level_select=true;
        #endif 
        #ifndef DEBUG
            result = scene_type::PLAY;
        #endif
    }
    if(bn::keypad::l_pressed() && !_level_select){
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

}