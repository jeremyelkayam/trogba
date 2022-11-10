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
        _countryside(bn::regular_bg_items::day.create_bg(0, 58)){
    _cottages.emplace_front(bn::fixed(-30), bn::fixed(-40), direction::DOWN);
    _cottages.emplace_front(bn::fixed(60), bn::fixed(-20), direction::RIGHT);
    _cottages.emplace_front(bn::fixed(0), bn::fixed(60), direction::UP);
    _peasants.emplace_front(bn::fixed(-30), bn::fixed(-30), bn::fixed(0.5), bn::fixed(90), direction::DOWN);
}

bn::optional<scene_type> play_scene::update(){

    bn::optional<scene_type> result;

    _trogdor.update();
    for(cottage &c : _cottages){
        c.update();
        _trogdor.check_cottage_collision(c);
    }

    bn::forward_list<peasant, 20>::iterator cur = _peasants.begin();
    bn::forward_list<peasant, 20>::iterator prev = _peasants.before_begin();
    while(cur != _peasants.end() && !_peasants.empty()) {
        peasant &peasant = *cur;

        peasant.update();
        _trogdor.check_peasant_collision(peasant);
        if(peasant.remove_from_map()){
            BN_LOG("dead peasant");
            if(_peasants.size() <= 1){
                _peasants.pop_front();
            }else{
                _peasants.erase_after(prev);
                
            }
        }

        prev = cur;
        ++cur;
    }

    // _peasant_spawn_counter++;
    // if(_peasant_spawn_counter==_peasant_spawn_interval){
    //     _peasant_spawn_counter=0;
    //     _peasants.emplace_front(bn::fixed(-30), bn::fixed(-30), bn::fixed(0.5), bn::fixed(90), direction::DOWN);
    // }

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