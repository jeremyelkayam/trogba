#include <bn_keypad.h>
#include <bn_sound_items.h>
#include <bn_log.h>

#include "constants.h"
#include "bn_regular_bg_items_day.h"
#include "play_scene.h"

namespace trog {

play_scene::play_scene(session_info& sesh, bn::sprite_text_generator& generator) : 
        _sesh(sesh),
        _trogdor(sesh),
        _hud(sesh, generator, TROG_TROGMETER_MAX),
        _pfact(_cottages,_peasants),
        _blueknight(35,35,TROG_KNIGHT_SPEED,180),
        _countryside(bn::regular_bg_items::day.create_bg(0, 58)){
    _cottages.emplace_back(bn::fixed(-30), bn::fixed(-40), direction::DOWN);
    _cottages.emplace_back(bn::fixed(60), bn::fixed(-20), direction::LEFT);
    _cottages.emplace_back(bn::fixed(0), bn::fixed(60), direction::UP);
    _archers.emplace_front(0,false);

}

bn::optional<scene_type> play_scene::update(){

    bn::optional<scene_type> result;

    _trogdor.update();
    for(cottage &c : _cottages){
        c.update();
        _trogdor.handle_cottage_collision(c);
    }

    for(peasant &p : _peasants) {
        p.update();
        _trogdor.handle_peasant_collision(p);

        //TODO this nest goes too deep

        if(p.remove_from_map() && p.onfire()){
            //check if it should burn any cottages
            for(cottage &c : _cottages){
                if(p.collides_with(c)){
                    bool cottage_burninated = c.burninate();
                    if(cottage_burninated) {
                        //bonus points if the peasant burns his house down
                        _sesh.score+=TROG_COTTAGE_PEASANTBURN_SCORE;
                    }else{
                        //the peasant is still dead so you get points
                        _sesh.score+=TROG_PEASANT_STOMP_SCORE;
                    }
                }
            }
        }
    }
    for(archer &a : _archers) {
        a.update();
        _trogdor.handle_arrow_collision(a);
    }


    _blueknight.update();
    _trogdor.handle_knight_collision(_blueknight);

    //despawn any peasants that need to be despawned
    _peasants.remove_if(peasant_deletable);
    _archers.remove_if(archer_deletable);

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