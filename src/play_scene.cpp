/* play_scene.cpp 
 * Author: Jeremy Elkayam
 * Purpose: Contain logic for the play scene (y'know, the one in the countryside
 * with the cottages and stuff)
 */

#include <bn_keypad.h>
#include <bn_sound_items.h>
#include <bn_log.h>

#include "bn_regular_bg_items_day.h"
#include "bn_regular_bg_items_night.h"
#include "bn_regular_bg_items_dawn.h"
#include "bn_regular_bg_items_dusk.h"
#include "bn_sprite_items_trogdor_variable_8x16_font_black.h"

#include "play_scene.h"
#include "constants.h"
#include "bloody_text.h"
#include "burninate_text.h"
#include "level_data.h"

namespace trog {

play_scene::play_scene(session_info& sesh, hud& hud) : 
        _sesh(sesh),
        _trogdor(new player(TROG_PLAYER_SPAWN_X, TROG_PLAYER_SPAWN_Y, sesh, false)),
        _hud(hud),
        _pfact(_cottages,_peasants),
        _afact(_archers, sesh.get_level()),
        _burninate_pause_time(0),
        _win_pause_time(0),
        _countryside(bn::regular_bg_items::day.create_bg(0, 58))
{
    BN_ASSERT(_sesh.get_level() <= 100, "There are only 100 levels");
    //make the background appear underneath all other backgroundlayers
    _countryside.put_below();


    //Level loading logic cribbed from Trogdor Reburninated by Mips96.
    int level_index;
    if (_sesh.get_level() == 1) {
		level_index = 0;
	} else {
        //weird, idk 
		level_index = ((_sesh.get_level() - 2) % 32 + 2) - 1;
	}
    switch(levels[level_index][0]){
        case 1:
            _countryside.set_item(bn::regular_bg_items::day);
            break;
        case 2:
            _countryside.set_item(bn::regular_bg_items::dusk);
            break;
        case 3:
            _countryside.set_item(bn::regular_bg_items::night);
            break;
        case 4:
            _countryside.set_item(bn::regular_bg_items::dawn);
            break;
        default:
            BN_ERROR("Invalid background ID in level_data.h");
    }

    // 6 = max cottages
    for (int i = 0; i < _cottages.max_size(); i++) {
        BN_LOG("cottage #", i);

        //Index 1 in level data refers to the number of the treasure hut from 1-6
        // 0 if no treasure hut 
        bool treasurehut = (i == (levels[level_index][1] - 1));
        BN_LOG("treasure hut? ", treasurehut);

		int j = (i * 3) + 2;
        BN_LOG("direction: ", levels[level_index][j]);

		if (levels[level_index][j] > 0) {

            direction enumdir;
            switch(levels[level_index][j]){
                case 1:
                    enumdir = direction::UP;
                break;
                case 2:
                    enumdir = direction::DOWN;
                break;
                case 3:
                    enumdir = direction::LEFT;
                break;
                case 4:
                    enumdir = direction::RIGHT;
                break;
                default:
                    BN_ERROR("invalid direction in levelData.h");
                break;
            }


			_cottages.emplace_back(
				(240 * (((bn::fixed)levels[level_index][j + 1] + 2466) / 5000.0)) + 8 - 120,
				(160 * (((bn::fixed)levels[level_index][j + 2] + 2183) / 3600.0)) - 11 - 80,
				enumdir,
                treasurehut
			);
		}
	}


    _knights.emplace_front(-59, 31, false);
    _knights.emplace_front(43,-40,true);
    // _troghammer = troghammer(0, 0, false);
}

bn::optional<scene_type> play_scene::update(){
    set_visible(true);

    bn::optional<scene_type> result;

    if(_overlay_text) _overlay_text->update();

    if(_burninate_pause_time > 0) {
        _burninate_pause_time++;
        BN_ASSERT(_overlay_text, "If we are paused due to burnination, THERE MUST BE TEXT");
    }else if(level_complete()){
        _win_pause_time++;
        _trogdor->update_win_anim();
    }else{
        //first update HUD info with trogdor's info from the last frame
        _hud.update_burninatemeter(_trogdor->get_burninating_time());
        _hud.update_trogmeter(_trogdor->get_trogmeter());

        
        _trogdor->update();        
        
        bool was_burninating = _trogdor->burninating();

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
        if(_trogdor->burninating() && !was_burninating){
            _burninate_pause_time = 1;
            _overlay_text.reset(new burninate_text());
        }

        bool was_dead = _trogdor->dead();        
        for(archer &a : _archers) {
            a.update();
            _trogdor->handle_arrow_collision(a);
        }
        if(_trogdor->dead() && !was_dead) {
            _overlay_text.reset(new bloody_text(true, 0, 0, "ARROWED!", bn::sprite_items::trogdor_variable_8x16_font_black.palette_item()));
        }

        was_dead = _trogdor->dead();  
        for(knight &k : _knights){
            k.update();
            _trogdor->handle_knight_collision(k);
        }
        
        if(_trogdor->dead() && !was_dead) {
            bn::string<13> str = "SWORDED!";
            //3% chance to get it wrong
            if(rand() % 33 == 0){
                str = "SORDID!";
            }
            _overlay_text.reset(new bloody_text(true, 0, 0, str.c_str(), bn::sprite_items::trogdor_variable_8x16_font_black.palette_item()));
        }

        if(_troghammer){
            was_dead = _trogdor->dead();  
            _troghammer->update();
            // _trogdor->handle_knight_collision(_troghammer);
        }


        //despawn any peasants that need to be despawned
        _peasants.remove_if(peasant_deletable);
        _archers.remove_if(archer_deletable);

        _pfact.update();
        _afact.update();



        if(_trogdor->ready_to_respawn()){
            _overlay_text.reset();
            if(_sesh.get_mans() == 0) {
                result = scene_type::LOSE;
            }else{
                _trogdor.reset(new player(TROG_PLAYER_SPAWN_X, TROG_PLAYER_SPAWN_Y, _sesh, true));
                _sesh.die();
            }
        }
    }

    if(_burninate_pause_time >= TROG_BURNINATE_PAUSETIME){
        _burninate_pause_time = 0;
        _overlay_text.reset();
    }
    
    if(level_complete() && _win_pause_time > TROG_WIN_PAUSETIME){
        result = scene_type::LEVELBEAT;
    }

    #ifdef DEBUG 
        //Instantly win level by pressing A
        if(bn::keypad::a_pressed()) result = scene_type::LEVELBEAT;
    #endif


    //had to move this out to fix a bug where cottage fire was visible while paused.
    // since you can't move while paused, we should be fine....
    for(cottage &c : _cottages){
        c.update();

        // only run the collision check while unpaused
        if(!_burninate_pause_time && _trogdor->handle_cottage_collision(c)){
            //the above if statement returns true if we hit a treasure hut
            result = scene_type::BONUS;

            //this marks the cottage as visited so that we can no longer return
            c.visit();
            set_visible(false);
        }
    }

    
    return result;
}

bool play_scene::level_complete(){
    bool result = true;
    for(cottage &c : _cottages) {
        result = result & c.burninated();
    }

    
    return result;
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
    for(knight &k : _knights){
        k.set_visible(visible);
    }

    _countryside.set_visible(visible);

}


}