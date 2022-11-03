#include "instructions_scene.h"
#include "scene_type.h"
#include <bn_sound_items.h>
#include <bn_log.h>

namespace trog {

instructions_scene::instructions_scene(){

    bn::sound_items::trogador.play(1);
}

bn::optional<scene_type> instructions_scene::update(){
    bn::optional<scene_type> result;
    
    return result;
}

}