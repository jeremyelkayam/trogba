/* play_scene.cpp 
 * Author: Jeremy Elkayam
 * Purpose: Contain logic for the play scene (y'know, the one in the countryside
 * with the cottages and stuff)
 */

#include <bn_keypad.h>
#include <bn_log.h>
#include <bn_sprite_palettes.h>
#include <bn_bg_palettes.h>
#include <bn_sram.h>
#include <bn_blending.h>

#include "bn_regular_bg_items_day.h"
#include "bn_regular_bg_items_night.h"
#include "bn_regular_bg_items_dawn.h"
#include "bn_regular_bg_items_dusk.h"
#include "bn_sprite_items_trogdor_variable_8x16_font_black.h"
#include "bn_sprite_items_trogdor_variable_8x16_font.h"

#include "play_scene.h"
#include "constants.h"
#include "bloody_text.h"
#include "burninate_text.h"
#include "level_data.h"

namespace trog {

play_scene::play_scene(session_info& sesh, hud& hud, bn::sprite_text_generator &text_generator) : 
        _sesh(sesh),
        _trogdor(new player(TROG_PLAYER_SPAWN_X, TROG_PLAYER_SPAWN_Y + 
        //temp fix for f'ed up spawnage
        (_sesh.get_level() == 27 || _sesh.get_level() == 59 || _sesh.get_level() == 91) ? 10 : 0, sesh, false)),
        _hud(hud),
        _pfact(_cottages,_peasants),
        _afact(_archers, sesh.get_level()),
        _burninate_pause_time(0),
        _win_pause_time(0),
        _flashing_text_time(0),
        _player_paused(false),
        _countryside(bn::regular_bg_items::day.create_bg(0, 58)),
        _text_generator(text_generator)
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

    bn::fixed_point void_tower_pos;
    switch(levels[level_index][0]){
        case 1:
            _countryside.set_item(bn::regular_bg_items::day);
            void_tower_pos = TROG_VOIDTOWER_POS_DAY;
            break;
        case 2:
            _countryside.set_item(bn::regular_bg_items::dusk);
            void_tower_pos = bn::fixed_point(10, -61);
            break;
        case 3:
            _countryside.set_item(bn::regular_bg_items::night);
            void_tower_pos = TROG_VOIDTOWER_POS_NIGHT;
            break;
        case 4:
            _countryside.set_item(bn::regular_bg_items::dawn);
            void_tower_pos = bn::fixed_point(-90, -61);
            break;
        default:
            BN_ERROR("Invalid background ID in level_data.h");
    }

    // 6 = max cottages
    for (int i = 0; i < _cottages.max_size(); i++) {


		int j = (i * 3) + 2;

		if (levels[level_index][j] > 0) {
            //Index 1 in level data refers to the number of the treasure hut from 1-6
            // 0 if no treasure hut             
            bool treasurehut = (i == (levels[level_index][1] - 1)) && _sesh.can_visit_treasure_hut();
            BN_LOG("treasure hut? ", treasurehut);            
            BN_LOG("cottage #", i + 1);
            BN_LOG("direction: ", levels[level_index][j]);
            bn::fixed xcor = (240 * (((bn::fixed)levels[level_index][j + 1] + 2466) / 5000.0)) + 8 - 120;
            BN_LOG("xcor ", xcor);
            bn::fixed ycor = (160 * (((bn::fixed)levels[level_index][j + 2] + 2183) / 3600.0)) - 11 - 80;
	    	BN_LOG("ycor ", ycor);

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
				xcor,
                ycor,
				enumdir,
                treasurehut,
                sesh.load_cottage_burnination(i)
			);
		}
	}
    //once it's loaded we are done with it
    _sesh.clear_burnination_array();


    _text_generator.set_center_alignment();
    _text_generator.generate(0, 55, "paused", _paused_text);
    _text_generator.generate(0, 70, "press 'START' to resume", _paused_text);
    set_paused_text_visible(false);

    _knights.emplace_front(-59, 31, false);
    _knights.emplace_front(33,-50,true);

}

void play_scene::set_paused_text_visible(bool visible){
    for(bn::sprite_ptr &sprite : _paused_text){
            sprite.set_visible(visible);
            sprite.put_above();
    }
}

bn::optional<scene_type> play_scene::update(){
    set_visible(true);

    bn::optional<scene_type> result;

    if(_overlay_text) _overlay_text->update();

    if(_win_pause_time == 1){
    }


    if(_burninate_pause_time > 0) {
        _burninate_pause_time++;
        BN_ASSERT(_overlay_text, "If we are paused due to burnination, THERE MUST BE TEXT");
    }else if(level_complete()){
        _win_pause_time++;
        _trogdor->update_win_anim();
    }else if(_player_paused){
        if(_flashing_text_time == 15){
            set_paused_text_visible(false);
        }else if(_flashing_text_time == 30){
            _flashing_text_time = 0;
        }else if(_flashing_text_time == 1){
            set_paused_text_visible(true);
        }
        ++_flashing_text_time;

    }else{
        set_paused_text_visible(false);

        //first update HUD info with trogdor's info from the last frame
        _hud.update_burninatemeter(_trogdor->get_burninating_time(), _trogdor->get_burninating_length());
        _hud.update_trogmeter(_trogdor->get_trogmeter());

        bool was_burninating = _trogdor->burninating();

        _trogdor->update();        
        


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

            _sesh.set_killed_by_archer(true);
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
            short rand_num = rand() % 100;
            if(rand_num % 100 == 0){
                str = "SORDID!";
                //maybe add that line of s.bad saying "A sordid affair"
            }else if(rand_num % 100 == 1){
                str = "SORTED!";
            }
            _sesh.set_killed_by_archer(false);
            _overlay_text.reset(new bloody_text(true, 0, 0, str.c_str(), bn::sprite_items::trogdor_variable_8x16_font_black.palette_item()));
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
                _trogdor.reset(new player(TROG_PLAYER_SPAWN_X, 
                //temp fix for f'ed up spawnage
               (_sesh.get_level() == 27 || _sesh.get_level() == 59 || _sesh.get_level() == 91) ? 10 : 0, _sesh, true));
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


    //START pauses the game
    //but you shouldn't be able to pause during other animations 
    //that block input (e.g. death/burninate!/winning a level)
    if(bn::keypad::start_pressed() && _burninate_pause_time == 0 
       && _win_pause_time == 0 && !_trogdor->dead()){
        _player_paused = !_player_paused;
        if(_player_paused){
            //Apply a dimming effect and display text when the game is paused.
            bn::sprite_palettes::set_fade(bn::color(16, 16, 16), 0.6);
            bn::bg_palettes::set_fade(bn::color(16, 16, 16), 0.6);
            set_paused_text_visible(true);

        }else{
            bn::sprite_palettes::set_fade_intensity(0); 
            bn::bg_palettes::set_fade_intensity(0);
            set_paused_text_visible(false);
        }
    } 


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
            //this marks it as visited if we autosave
            _sesh.visit_treasure_hut();

            set_visible(false);
        }
    }

    if(result && result != scene_type::BONUS){
        _hud.clear_scrolling_text();
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