/* play_scene.cpp 
 * Author: Jeremy Elkayam
 * Purpose: Contain logic for the play scene (y'know, the one in the countryside
 * with the cottages and stuff)
 */

#include <bn_keypad.h>
#include <bn_sprite_palettes.h>
#include <bn_bg_palettes.h>
#include <bn_blending.h>

#include "bn_regular_bg_items_day.h"
#include "bn_sprite_items_trogdor_variable_8x16_font_black.h"
#include "bn_sprite_items_trogdor_variable_8x16_font_red.h"
#include "bn_sprite_items_trogdor_variable_8x8_font.h"

#include "play_scene.h"
#include "big_text.h"
#include "burninate_text.h"
#include "level_data.h"

namespace trog {

play_scene::play_scene(session_info& sesh, hud& hud, bn::sprite_text_generator &text_generator, bn::random &rng) : 
        _sesh(sesh),
        _trogdor(new player(40, 0 + 
        //temp fix for f'ed up spawnage
        (_sesh.get_level() == 27 || _sesh.get_level() == 59 || _sesh.get_level() == 91) ? 10 : 0, sesh, false)),
        _hud(hud),
        _pfact(_cottages,_peasants, rng),
        _afact(_archers, sesh.get_level(), rng),
        _burninate_pause_time(0),
        _win_pause_time(0),
        _flashing_text_time(0),
        _player_paused(false),
        _rng(rng),
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


    // 6 = max cottages
    for (int i = 0; i < _cottages.max_size(); i++) {


		int j = (i * 3) + 2;

		if (levels[level_index][j] > 0) {
            //Index 1 in level data refers to the number of the treasure hut from 1-6
            // 0 if no treasure hut             
            bn::fixed xcor = (240 * (((bn::fixed)levels[level_index][j + 1] + 2466) / 5000.0)) + 8 - 120;
            bn::fixed ycor = (160 * (((bn::fixed)levels[level_index][j + 2] + 2183) / 3600.0)) - 11 - 80;

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
				enumdir
			);
		}
	}


    _text_generator.set_center_alignment();
    _text_generator.set_palette_item(bn::sprite_items::trogdor_variable_8x8_font.palette_item());
    _text_generator.generate(0, 55, "paused", _paused_text);
    _text_generator.generate(0, 70, "press 'START' to resume", _paused_text);
    set_paused_text_visible(false);

    _knights.emplace_front(-59, 31, false, rng);
    _knights.emplace_front(33,-50,true, rng);

}

void play_scene::set_paused_text_visible(bool visible){
    for(bn::sprite_ptr &sprite : _paused_text){
            sprite.set_visible(visible);
            sprite.put_above();
    }
}

bn::optional<scene_type> play_scene::update(){

    bn::optional<scene_type> result;

    if(_overlay_text) _overlay_text->update();

    if(_win_pause_time == 1){
        _trogdor->setup_win_pose();
    }


    if(level_complete()){
        _win_pause_time++;
    }else if(_player_paused){
        if(_flashing_text_time == 15){
            set_paused_text_visible(false);
        }else if(_flashing_text_time == 30){
            _flashing_text_time = 0;
        }else if(_flashing_text_time == 1){
            set_paused_text_visible(true);
        }
        ++_flashing_text_time;

    }else if(_burninate_pause_time > 0) {
        _burninate_pause_time++;
        BN_ASSERT(_overlay_text, "If we have burnination, THERE MUST BE TEXT");
    }else{

        //idk now it doesn't pause to burninate. maybe better maybe worse

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
                            _sesh.score(10);
                        }else{
                            //the peasant is still dead so you get points
                            _sesh.score(2);
                        }
                    }
                }
            }
        }
        if(_trogdor->burninating() && !was_burninating){
            _burninate_pause_time = 1;
            _overlay_text.reset(new burninate_text(_text_generator));
        }

        bool was_dead = _trogdor->dead();        
        for(archer &a : _archers) {
            a.update();
            _trogdor->handle_arrow_collision(a);
        }
        if(_trogdor->dead() && !was_dead) {

            _overlay_text.reset(new big_text(true, 0, 0, "ARROWED!", 
                bn::sprite_items::trogdor_variable_8x16_font_red.palette_item(),
                bn::sprite_items::trogdor_variable_8x16_font_black.palette_item(),
                _text_generator));
        }

        was_dead = _trogdor->dead();  
        for(knight &k : _knights){
            k.update();
            _trogdor->handle_knight_collision(k);
        }
        
        if(_trogdor->dead() && !was_dead) {
            bn::string<13> str = "SWORDED!";
            //3% chance to get it wrong
            short rand_num = _rng.get_int(100);
            if(rand_num == 0){
                str = "SORDID!";
            }else if(rand_num == 1){
                str = "SORTED!";
            }
            _overlay_text.reset(new big_text(true, 0, 0, str.c_str(), 
                bn::sprite_items::trogdor_variable_8x16_font_red.palette_item(),
                bn::sprite_items::trogdor_variable_8x16_font_black.palette_item(),
                _text_generator));
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
                _trogdor.reset(new player(40, 
                //temp fix for f'ed up spawnage
               (_sesh.get_level() == 27 || _sesh.get_level() == 59 || _sesh.get_level() == 91) ? 10 : 0, _sesh, true));
                _sesh.die();
            }
        }
    }

    if(_burninate_pause_time >= 60){
        _burninate_pause_time = 0;
        _overlay_text.reset();
    }
    
    if(level_complete() && _win_pause_time > 150){
        result = scene_type::LEVELBEAT;
    }


    if(bn::keypad::a_pressed()) result = scene_type::LEVELBEAT;


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


    if(!_player_paused){
        //had to move this out to fix a bug where cottage fire was visible while paused.
        // since you can't move while paused, we should be fine....
        for(cottage &c : _cottages){
            c.update();
            _trogdor->handle_cottage_collision(c);
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