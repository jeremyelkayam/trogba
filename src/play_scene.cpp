#include <bn_keypad.h>
#include <bn_sound_items.h>
#include <bn_log.h>

#include "constants.h"
#include "bn_regular_bg_items_day.h"
#include "play_scene.h"

namespace trog {

play_scene::play_scene(session_info& sesh, bn::sprite_text_generator& generator) : 
        _sesh(sesh),
        _hud(sesh, generator, TROG_TROGMETER_MAX),
        _pfact(_cottages,_peasants),
        _countryside(bn::regular_bg_items::day.create_bg(0, 58)){
    _cottages.emplace_back(bn::fixed(-30), bn::fixed(-40), direction::DOWN);
    _cottages.emplace_back(bn::fixed(60), bn::fixed(-20), direction::RIGHT);
    _cottages.emplace_back(bn::fixed(0), bn::fixed(60), direction::UP);
    // ;

}

bn::optional<scene_type> play_scene::update(){

    bn::optional<scene_type> result;

    _trogdor.update();
    for(cottage &c : _cottages){
        c.update();
        _trogdor.check_cottage_collision(c);
    }

    for(peasant &p : _peasants) {
        p.update();
        _trogdor.check_peasant_collision(p);
    }
    _peasants.remove_if(peasant_deletable);
    

    _pfact.update();

    _hud.update();
    _hud.update_burninatemeter(_trogdor.get_burninating_time());
    _hud.update_trogmeter(_trogdor.get_trogmeter());

    if(level_complete()){
        result = scene_type::LEVELBEAT;
    }else if(false){
        result = scene_type::LOSE;
    }
    
    return result;
}

bool play_scene::level_complete(){
    return false;
}

}