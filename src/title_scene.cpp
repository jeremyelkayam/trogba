#include <bn_keypad.h>
#include <bn_sound_items.h>
#include <bn_log.h>

#include "bn_regular_bg_items_titlebg.h"
#include "bn_regular_bg_items_titlegraphic.h"
#include "title_scene.h"

namespace trog {

title_scene::title_scene() : 
        titlebg(bn::regular_bg_items::titlebg.create_bg(-3, 48)), 
        title(bn::regular_bg_items::titlegraphic.create_bg(10, 58)){

    bn::sound_items::themesong.play(1);
    frame_counter=0;
}

bn::optional<scene_type> title_scene::update(){
    bn::optional<scene_type> result;
    frame_counter++;
    // BN_LOG(frame_counter);

    if(frame_counter > 440 || bn::keypad::start_pressed() || bn::keypad::a_pressed()){
        result = scene_type::INSTRUCTIONS;
    }
    
    return result;
}

}