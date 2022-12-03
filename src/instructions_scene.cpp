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

instructions_scene::instructions_scene(bn::sprite_text_generator& a_generator) : 
        _text_generator(a_generator) {
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
    for(bn::string<64> line : instructions) { 
        _text_generator.generate(0, ycor, line, _instruction_text_sprites);
        ycor+=14;
    }


    _text_generator.set_palette_item(bn::sprite_items::trogdor_variable_8x16_font_red.palette_item());
    _text_generator.generate(0, -30+(14*4)+7, "Press A to start 'em up ", _start_text_sprites);    

    _text_generator.set_palette_item(bn::sprite_items::trogdor_variable_8x16_font.palette_item());

    // text_generator.generate(0, 30, , start_text_sprites);    

    // text_generator.generate(0, 40, , start_text_sprites);    

}

bn::optional<scene_type> instructions_scene::update(){
    

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

    // text stuff
    bn::optional<scene_type> result;
    
    if(bn::keypad::a_pressed()){
        result = scene_type::PLAY;
    }
    return result;
}

}