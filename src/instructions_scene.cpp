#include <bn_sound_items.h>
#include <bn_log.h>
#include <bn_sprite_ptr.h>
#include <bn_keypad.h>
#include <bn_sprite_text_generator.h>
#include <bn_string.h>
#include "instructions_scene.h"


namespace trog {

instructions_scene::instructions_scene(bn::sprite_text_generator& a_generator) : 
        title(bn::regular_bg_items::titlegraphic.create_bg(TROG_TITLE_TEXT_X, TROG_INSTRUCTION_TITLE_Y)),
        text_generator(a_generator){
    flashing_text_counter = 0;

    bn::sound_items::trogador.play(TROG_DEFAULT_VOLUME);

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
        "use the squishy + to move",
        "squish 10 guys to get fire",
        "burn all cottages to win",
        "Avoid knights and archers!",
        "",
        "",
        "OG by Mike/Matt/Jonathan",
        "GBA version by Jeremy Elkayam"
    };
    
    int ycor = -30;
    for(bn::string<64> line : instructions) { 
        text_generator.generate(0, ycor, line, instruction_text_sprites);
        ycor+=14;
    }

    //    

    // text_generator.generate(0, -6, , instruction_text_sprites);   

    // text_generator.generate(0, 0, , instruction_text_sprites);   

    // text_generator.generate(0, 10, , instruction_text_sprites);   

    text_generator.generate(0, -30+(14*4)+7, "Press A to start 'em up ", start_text_sprites);    

    // text_generator.generate(0, 30, , start_text_sprites);    

    // text_generator.generate(0, 40, , start_text_sprites);    

}

bn::optional<scene_type> instructions_scene::update(){
    

    flashing_text_counter++;
    if(flashing_text_counter > visible_text_time){
        for(auto it : start_text_sprites) { 
            it.set_visible(false);
        }
    }
    if(flashing_text_counter > visible_text_time + invisible_text_time){
        for(auto it : start_text_sprites) { 
            it.set_visible(true);
        }
        flashing_text_counter = 0;
    }

    // text stuff
    bn::optional<scene_type> result;
    
    if(bn::keypad::a_pressed()){
        result = scene_type::PLAY;
    }
    return result;
}

}