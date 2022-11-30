#include <bn_keypad.h>
#include <bn_sound_items.h>
#include <bn_log.h>

#include "constants.h"
#include "bn_regular_bg_items_day.h"
#include "play_scene.h"

namespace trog {

play_scene::play_scene(session_info& sesh, hud& hud) : 
        _sesh(sesh),
        _trogdor(new player(TROG_PLAYER_SPAWN_X, TROG_PLAYER_SPAWN_Y, sesh, false)),
        _hud(hud),
        _pfact(_cottages,_peasants),
        _afact(_archers),
        _blueknight(35,35,TROG_KNIGHT_SPEED,180),
        _countryside(bn::regular_bg_items::day.create_bg(0, 58))
{
    _cottages.emplace_back(bn::fixed(-30), bn::fixed(-40), direction::DOWN, true);
    _cottages.emplace_back(bn::fixed(60), bn::fixed(-20), direction::LEFT, false);
    _cottages.emplace_back(bn::fixed(0), bn::fixed(60), direction::UP, false);
}

bn::optional<scene_type> play_scene::update(){
    set_visible(true);

    bn::optional<scene_type> result;

    _trogdor->update();
    for(cottage &c : _cottages){
        c.update();
        if(_trogdor->handle_cottage_collision(c)){
            //the above if statement returns true if we hit a treasure hut
            result = scene_type::BONUS;

            //this marks the cottage as visited so that we can no longer return
            c.visit();
            set_visible(false);
        }
    }

    for(peasant &p : _peasants) {
        p.update();
        _trogdor->handle_peasant_collision(p);

        //TODO this nest goes too deep

        if(p.remove_from_map() && p.onfire()){
            //check if it should burn any cottages
            for(cottage &c : _cottages){
                if(p.collides_with(c)){
                    bool cottage_burninated = c.burninate();
                    if(cottage_burninated) {
                        //bonus points if the peasant burns his house down
                        _sesh.score(TROG_COTTAGE_PEASANTBURN_SCORE);
                    }else{
                        //the peasant is still dead so you get points
                        _sesh.score(TROG_PEASANT_STOMP_SCORE);
                    }
                }
            }
        }
    }
    for(archer &a : _archers) {
        a.update();
        _trogdor->handle_arrow_collision(a);
    }


    _blueknight.update();
    _trogdor->handle_knight_collision(_blueknight);

    //despawn any peasants that need to be despawned
    _peasants.remove_if(peasant_deletable);
    _archers.remove_if(archer_deletable);

    _pfact.update();
    _afact.update();

    _hud.update_burninatemeter(_trogdor->get_burninating_time());
    _hud.update_trogmeter(_trogdor->get_trogmeter());

    if(level_complete()){
        result = scene_type::LEVELBEAT;
    }

    if(_trogdor->ready_to_respawn()){
        if(_sesh.get_mans() == 0) {
            result = scene_type::LOSE;
            BN_LOG("YOU HAVE DIED");
        }else{
            _trogdor.reset(new player(TROG_PLAYER_SPAWN_X, TROG_PLAYER_SPAWN_Y, _sesh, true));
            _sesh.die();
        }
    }
    
    return result;
}

bool play_scene::level_complete(){
    return false;
}

void play_scene::set_visible(bool visible){
    _trogdor->set_visible(visible);
    for(cottage &c : _cottages){
        c.set_visible(visible);
    }
    for(archer &a : _archers){
        a.set_visible(visible);
    }
    for(peasant &p : _peasants){
        p.set_visible(visible);
    }

    _blueknight.set_visible(visible);
    _countryside.set_visible(visible);

}

}