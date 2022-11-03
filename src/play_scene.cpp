#include <bn_keypad.h>
#include <bn_sound_items.h>
#include <bn_log.h>


#include "play_scene.h"
#include "scene_type.h"

namespace trog {

play_scene::play_scene(session_info& a_sesh) : sesh(a_sesh) {

}

bn::optional<scene_type> play_scene::update(){
    bn::optional<scene_type> result;
    trogdor.update();

    if(false){
        result = scene_type::LEVELBEAT;
    }else if(false){
        result = scene_type::LOSE;
    }
    
    return result;
}

}