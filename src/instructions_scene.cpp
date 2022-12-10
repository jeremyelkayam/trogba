#include <bn_sound_items.h>
#include <bn_log.h>
#include <bn_sprite_ptr.h>
#include <bn_keypad.h>
#include <bn_sprite_text_generator.h>
#include <bn_string.h>
#include "instructions_scene.h"
#include "bn_sprite_items_titlegraphic.h"
#include "constants.h"


namespace trog {

instructions_scene::instructions_scene(session_info &sesh, bn::sprite_text_generator& text_generator) : 
        _text_generator(text_generator),
        _level_select(false),
        _show_secret_hints(false),
        _sesh(sesh) {
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

    setup_instructions();


    // _text_generator.set_palette_item(bn::sprite_items::trogdor_variable_8x16_font.palette_item());

    // text_generator.generate(0, 30, , start_text_sprites);    

    // text_generator.generate(0, 40, , start_text_sprites);    

}

void instructions_scene::clear_text(){
    _instruction_text_sprites.clear();
    _start_text_sprites.clear();
}

void instructions_scene::setup_instructions(){
    //todo refactor the text setup into a function
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
    
    int ycor = -30;

    _text_generator.set_palette_item(bn::sprite_items::trogdor_variable_8x16_font.palette_item());

    for(bn::string<64> line : instructions) { 
        _text_generator.generate(0, ycor, line, _instruction_text_sprites);
        ycor+=14;
    }


    _text_generator.set_palette_item(bn::sprite_items::trogdor_variable_8x16_font_red.palette_item());
    _text_generator.generate(0, -30+(14*4)+7, "Press A to start 'em up ", _start_text_sprites);    
}

void instructions_scene::setup_secret_hints(){

    int ycor = -30;
    _text_generator.set_palette_item(bn::sprite_items::trogdor_variable_8x16_font_red.palette_item());
    _text_generator.generate(0, ycor, "SECRET HINTS!!", _start_text_sprites);    
    ycor+=16;

    bn::string<64> instructions[] = {
        "-Don't let peasants return home",
        "-U're invincible while burning",
        "-Flaming peasants burn houses",
        "-Extra man every 300pts"
    };
    
    _text_generator.set_palette_item(bn::sprite_items::trogdor_variable_8x16_font.palette_item());
    for(bn::string<64> line : instructions) { 
        _text_generator.generate(0, ycor, line, _instruction_text_sprites);
        ycor+=16;
    }
    _text_generator.set_palette_item(bn::sprite_items::trogdor_variable_8x16_font_red.palette_item());
    _text_generator.generate(0, ycor, "-What's a treasure hut?!?!", _instruction_text_sprites);
    ycor+=16;
    _text_generator.generate(0, ycor, "-Secret Code?!?!", _instruction_text_sprites);

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
    if(bn::keypad::l_pressed()){
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