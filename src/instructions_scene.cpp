#include <bn_sound_items.h>
#include <bn_log.h>
#include <bn_sprite_ptr.h>
#include <bn_keypad.h>
#include <bn_sprite_text_generator.h>
#include <bn_string.h>
#include "instructions_scene.h"


namespace trog {

instructions_scene::instructions_scene(bn::sprite_text_generator& a_generator) : 
        title(bn::regular_bg_items::titlegraphic.create_bg(10, 58)),
        text_generator(a_generator){
    flashing_text_counter = 0;

    bn::sound_items::trogador.play(TROG_DEFAULT_VOLUME);

    text_generator.generate(0, -15, "use the dpad to move around y'all", instruction_text_sprites);   

    text_generator.generate(0, 0, "kill 10 peasants to get fire", instruction_text_sprites);   

    text_generator.generate(0, 15, "fire burns cottages", instruction_text_sprites);   

    text_generator.generate(0, 30, "burn all houses 2 win", instruction_text_sprites);   

    text_generator.generate(0, 45, "watch out 4 archers and nites", instruction_text_sprites);   

    text_generator.generate(0, 60, "press a to start 'em up... ", start_text_sprites);    
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