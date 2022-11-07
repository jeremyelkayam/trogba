#include <bn_keypad.h>
#include <bn_sound_items.h>
#include <bn_log.h>

#include "bn_regular_bg_items_day.h"
#include "play_scene.h"

namespace trog {

play_scene::play_scene(session_info& sesh, bn::sprite_text_generator& generator) : 
        _sesh(sesh),
        _hud(sesh, generator),
        _countryside(bn::regular_bg_items::day.create_bg(0, 58)),
        _cottages(cottage(bn::fixed(-30), bn::fixed(-30), direction::DOWN)){
}

bn::optional<scene_type> play_scene::update(){

    bn::optional<scene_type> result;

    _trogdor.update();
    _hud.update();

    if(false){
        result = scene_type::LEVELBEAT;
    }else if(false){
        result = scene_type::LOSE;
    }
    
    return result;
}

}