#include "title_scene.h"
#include "scene_type.h"
#include <bn_sound_items.h>
#include <bn_log.h>

namespace trog {

title_scene::title_scene(){

    bn::sound_items::themesong.play(1);
    frame_counter=0;
}

bn::optional<scene_type> title_scene::update(){
    bn::optional<scene_type> result;
    frame_counter++;
    // BN_LOG(frame_counter);

    if(frame_counter > 440){
        result = scene_type::INSTRUCTIONS;
    }
    
    return result;
}

}