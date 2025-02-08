/* play_scene.cpp 
 * Author: Jeremy Elkayam
 * Purpose: Contain logic for the play scene (y'know, the one in the countryside
 * with the cottages and stuff)
 */

#include <bn_keypad.h>
#include <bn_sound_items.h>
#include <bn_log.h>
#include <bn_sprite_palettes.h>
#include <bn_bg_palettes.h>
#include <bn_blending.h>
#include <bn_music_items.h>
#include <bn_music.h>
#include <bn_bg_palette_ptr.h>

#include "bn_regular_bg_items_day.h"
#include "bn_regular_bg_items_night.h"
#include "bn_regular_bg_items_dawn.h"
#include "bn_regular_bg_items_dusk.h"
#include "bn_regular_bg_items_pause_menu.h"
#include "bn_regular_bg_items_gray.h"
#include "bn_sprite_items_trogdor_variable_8x16_font_black.h"
#include "bn_sprite_items_trogdor_variable_8x16_font.h"
#include "bn_sprite_items_trogdor_variable_8x16_font_red.h"
#include "bn_sprite_items_voidtower.h"
#include "bn_sprite_items_tutorial_arrow.h"

#include "play_scene.h"
#include "constants.h"
#include "bloody_text.h"
#include "burninate_text.h"
#include "level_data.h"
#include "sb_commentary.h"
#include "sucks.h"
#include "chiaroscuro.h"


namespace trog {

play_scene::play_scene(session_info& sesh, hud& hud, common_stuff &common_stuff) : 
        _sesh(sesh),
        _common_stuff(common_stuff),
        _hud(hud),
        _pfact(_cottages,_peasants, common_stuff.rand),
        _afact(_archers, sesh.get_level(), common_stuff),
        _burninate_pause_time(0),
        _win_pause_time(0),
        _autosave_visibility_time(0),
        _shake_timer(0),
        _player_paused(false),
        _timer(0),
        _tutorial_cutscene_timer(0),
        _pause_menu_index(0),
        _countryside(bn::regular_bg_items::day.create_bg(0, 58))
{

    respawn(false);

    saved_session &saved_sesh = common_stuff.savefile.session;



    BN_ASSERT(_sesh.get_level() <= 100, "There are only 100 levels");
    //make the background appear underneath all other backgroundlayers
    _countryside.put_below();

    if(_sesh.get_level() == 1 && _sesh.get_mans() == 30){
        bn::sound_items::secret_unlocked.play(_common_stuff.savefile.options.sound_vol);
    }


    //Level loading logic cribbed from Trogdor Reburninated by Mips96.
    uint8_t level_index;
    if (_sesh.get_level() <= 1) {
		level_index = 0;
	} else {
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

    if(_sesh.get_level() != 0){
        // 6 = max cottages
        for (uint8_t i = 0; i < _cottages.max_size(); i++) {


            uint8_t j = (i * 3) + 2;

            if (levels[level_index][j] > 0) {
                //Index 1 in level data refers to the number of the treasure hut from 1-6
                // 0 if no treasure hut            
                bool treasurehut = (i == (levels[level_index][1] - 1));
                if(saved_sesh.exists && !saved_sesh.can_visit_treasure_hut){
                    treasurehut = false;
                }
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
                        BN_ERROR("invalid direction in level_data.h");
                    break;
                }


                _cottages.emplace_back(
                    xcor,
                    ycor,
                    enumdir,
                    treasurehut,
                    saved_sesh.exists && saved_sesh.cottage_burnination_status[i],
                    _common_stuff
                );
            }
        }
    }


    if(_sesh.get_level() != 0){
        _knights.emplace_front(-59, 31, false, common_stuff.rand);
        _knights.emplace_front(33, -50, true, common_stuff.rand);
    }

    if(_sesh.troghammer_enabled()){
        _void_tower = bn::sprite_items::voidtower.create_sprite_optional(void_tower_pos);
        _void_tower->set_z_order(FURTHEST_BACK_ZORDER);
        _void_tower->put_below();

        if(saved_sesh.exists && saved_sesh.thinfo.current_state != troghammer_state::UNALERTED){
            _troghammer.reset(new troghammer(saved_sesh.thinfo, true, _sesh.get_level(), common_stuff.rand));
            _void_tower->set_item(bn::sprite_items::voidtower, 1);
        }
    }

    if(_sesh.get_level() == 0){
        //Initialize tutorial
        _text_box.reset(new text_box(common_stuff.small_generator, "You are TROGDOR, the BURNiNATOR.\nUse the SQUISHY PLUS SIGN (+) to move!!"));
        bn::music_items::skoa.play(common_stuff.savefile.options.music_vol);
        _voices_volume = common_stuff.savefile.options.voice_vol;

        //Temporarily turn voices off.
        common_stuff.savefile.options.voice_vol = 0;
    }
}


bn::optional<scene_type> play_scene::update(){
    if(!_countryside.visible()) set_visible(true);

    bn::optional<scene_type> result;

    if(_overlay_text) _overlay_text->update();

    if(_win_pause_time == 1){
        bool troghammer_arrived = _troghammer && _troghammer->in_play();

        _common_stuff.commentary.level_win_pause(_sesh.get_dragon(), troghammer_arrived);

        //tutorial win level thing 
        if(_sesh.get_level() == 0){
            _text_box.reset(new text_box(_common_stuff.small_generator, "Congrats! You finished the tutorial."));
        }
    }
    if(_autosave_visibility_time != 0){
        ++_autosave_visibility_time;
    }
    if(_autosave_visibility_time > (3 SECONDS)){
        _autosave_visibility_time = 0;
        _common_stuff.set_autosave_text_visible(false);
    }

    if(_burninate_pause_time > 0) {
        _burninate_pause_time++;
        BN_ASSERT(_overlay_text, "If we are paused due to burnination, THERE MUST BE TEXT");
    }else if(level_complete()){
        _win_pause_time++;
        _player->update_win_anim();
    }else if(_player_paused){
        result=update_pause_menu();

    }else if(_tutorial_cutscene_timer > 0){
        ++_tutorial_cutscene_timer;
        if(_archers.empty()){
            for(cottage &c : _cottages) c.update_anim();
            if(_tutorial_cutscene_timer >= 68 && _tutorial_cutscene_timer <= 90){
                if(_tutorial_cutscene_timer % 4 == 0){
                    _countryside.set_y(_countryside.y() + 5);
                }else if(_tutorial_cutscene_timer % 2 == 0){
                    _countryside.set_y(_countryside.y() - 5);                    
                }
            }
        }

        if(!_archers.empty()){
            for(archer &a : _archers) a.update_anim();
            for(knight &k : _knights) k.update_anim();
            if(_tutorial_cutscene_timer == 200) _tutorial_cutscene_timer = 0; 
        }
        
        if(_archers.empty() && _tutorial_cutscene_timer == 100) _tutorial_cutscene_timer = 0;



    }else{
        //screen shake loop
        if(_shake_timer && _shake_interval && _shake_y_offset){
            uint8_t &interval =  *_shake_interval.get();
            bn::fixed &shake_y_offset = *_shake_y_offset.get(); 
            if(_shake_timer % interval == 0){
                bool even_interval = _shake_timer % (interval * 2) == 0;
                bn::fixed yoffset = even_interval ? shake_y_offset : 0;

                int8_t factor = even_interval ? 1 : -1;
                _countryside.set_y(_countryside.y() + factor * shake_y_offset);
                for(entity *e : all_entities()){
                    e->set_y_offset(yoffset);
                }
                if(_void_tower){
                    _void_tower->set_y(_void_tower->y() + factor * shake_y_offset);
                }
            }
            --_shake_timer;

        }

        if(_sesh.get_level() == 0) update_tutorial();

        if(!_player->dead() && _autosave_visibility_time == 0){
            _common_stuff.set_autosave_text_visible(false);
        }

        //first update HUD info with trogdor's info from the last frame
        _hud.update_burninatemeter(_player->get_burninating_time(), _player->get_burninating_length());
        _hud.update_trogmeter(_player->get_trogmeter());

        bool was_burninating = _player->burninating();

        _player->update(); 

        if(_sesh.get_dragon() == dragon::SUCKS){
            sucks *player = (sucks *) _player.get();
            if(player->stomp_timer() == TROG_SUCK_STOMP_FRAME){
                //stomp clause                
                for(freezable *f : all_freezables()){
                    //yes, euclidean distance is an expensive operation here. However, 
                    // we're only doing it on a max of 20 actors so hopefully it's ok
                    bn::fixed_point center(player->get_pos().x(), player->foot_pos().y());
                    bn::fixed dist = _common_stuff.euclidean_dist(center, f->get_pos());
                    if(dist < TROG_SUCK_STOMP_RADIUS){
                        f->freeze();
                    }else{
                        // f->alert(player->get_pos());
                    }
                }
                for(archer &arch : _archers){
                    arch.stomp_on(player->foot_pos(), TROG_SUCK_STOMP_RADIUS);
                }

                
                shake_screen(4, 4, 4);
                bn::sound_items::heavy_crash.play(_common_stuff.savefile.options.sound_vol);
            }
        }       
        


        for(peasant &p : _peasants) {
            p.update();
            _player->handle_peasant_collision(p);

            if(p.remove_from_map()){
                if(p.onfire()){
                    //check if it should burn any cottages
                    bool cottage_burninated = p.get_home().burninate();
                    if(cottage_burninated) {
                        //bonus points if the peasant burns his house down
                        _sesh.score(TROG_COTTAGE_PEASANTBURN_SCORE);
                    }else{
                        //the peasant is still dead so you get points
                        _sesh.score(TROG_PEASANT_STOMP_SCORE);
                    }
                }else if(!p.dead() && !p.onfire()){
                    // if the peasant didn't die, we should decrement the trogmeter
                    // if the option is enabled 
                    if(_sesh.can_lose_trogmeter()) _player->drop_trogmeter();
                }
            }
        }
        if(_player->burninating() && !was_burninating){
            _burninate_pause_time = 1;
            _common_stuff.commentary.burninate(_sesh.get_dragon());
            
            _overlay_text.reset(new burninate_text());
        }else if(!_player->burninating() && was_burninating){
            //our trogmeter is at 0 now, so this is a good time to autosave
            autosave(false);
            if(!_troghammer && _sesh.troghammer_enabled()){
                spawn_troghammer(true);
            }
        }

        bool was_dead = _player->dead();        
        for(archer &a : _archers) {
            a.update();
            _player->handle_arrow_collision(a);
        }
        if(_player->dead() && !was_dead) {
            if(_sesh.get_mans() != 0){
                _common_stuff.commentary.arrowed(_sesh.get_dragon());
            }
            _sesh.set_killed_by_archer(true);
            _overlay_text.reset(new bloody_text(true, 0, 0, "ARROWED!", bn::sprite_items::trogdor_variable_8x16_font_black.palette_item(), _common_stuff.rand));
            
            autosave(true);
        }

        was_dead = _player->dead();  
        for(knight &k : _knights){
            if(k.alerted()) k.update_alert_direction(_player->get_pos());
            k.update();
            _player->handle_knight_collision(k);
        }
        
        if(_player->dead() && !was_dead) {
            if(_sesh.get_mans() != 0){
                _common_stuff.commentary.sworded(_sesh.get_dragon());
            }
            bn::string<13> str = "SWORDED!";
            //2% chance to get it wrong
            short rand_num = _common_stuff.rand.get_int(100);
            if(rand_num == 0){
                str = "SORDID!";
                //maybe add that line of s.bad saying "A sordid affair"
            }else if(rand_num == 1){
                str = "SORTED!";
            }
            _sesh.set_killed_by_archer(false);
            _overlay_text.reset(new bloody_text(true, 0, 0, str.c_str(), bn::sprite_items::trogdor_variable_8x16_font_black.palette_item(), _common_stuff.rand));

            autosave(true);
        }

        if(_troghammer){
            if(_troghammer->alerted()) _troghammer->update_alert_direction(_player->get_pos());
            was_dead = _player->dead();  
            _troghammer->update();

            if(_troghammer->in_play()){
                _player->handle_troghammer_collision(*_troghammer.get());
            }

            if(_player->dead() && !was_dead){
                //todo: add a secret animation where he's passed out drunk
                _overlay_text.reset(new bloody_text(true, 0, 0, "HAMMERED!", bn::sprite_items::trogdor_variable_8x16_font_black.palette_item(), _common_stuff.rand));
                autosave(true);
            }

            if(_troghammer->awake_alert()){
                _hud.scroll_text("THE TROGHAMMER HAS AWOKEN...");   
                _th_sound.reset(new troghammer_sound(1, _common_stuff.savefile.options.sound_vol));                      
            }else if(_troghammer->coming_alert()){
                _hud.scroll_text("THE TROGHAMMER APPROACHES...");   
                _th_sound.reset(new troghammer_sound(2, _common_stuff.savefile.options.sound_vol));                      
            }else if(_troghammer->arrived_alert()){
                _hud.scroll_text("THE TROGHAMMER ARRIVES!");   
                _th_sound.reset(new troghammer_sound(3, _common_stuff.savefile.options.sound_vol));      
            }
        }
        if(_th_sound){
            _th_sound->update();
            if(_th_sound->done()) _th_sound.reset();
        }


        //despawn any peasants that need to be despawned
        _peasants.remove_if(peasant_deletable);
        _archers.remove_if(archer_deletable);

        //only spawn peasants if there are cottages
        if(_cottages.size() > 0)
            _pfact.update();
        
        //only spawn archers if there are knights
        if(_knights.size() > 0)
            _afact.update();

        //the above 2 cases are mainly for the tutorial



        if(_player->ready_to_respawn()){
            _overlay_text.reset();
            if(_sesh.get_mans() == 0) {
                result = scene_type::LOSE;
            }else{
                uint8_t init_trogmeter = 0;
                if(_sesh.get_level() == 0 && _player->get_trogmeter() != 10){
                    init_trogmeter = _player->get_trogmeter();
                }
                respawn(true, init_trogmeter);
                _sesh.die();
                
                if(!_troghammer && _sesh.troghammer_enabled()) spawn_troghammer(true);
            }
        }

        //TUTORIAL BLOCK
        if(_sesh.get_level() == 0) update_tutorial();
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
       && _win_pause_time == 0 && !_player->dead() && _sesh.get_level() != 0){
        _player_paused = !_player_paused;
        if(_player_paused){
            _pause_menu_index = 3;
        }else{
            unpause();
        }
    } 


    //had to move this out to fix a bug where cottage fire was visible while paused.
    // since you can't move while paused, we should be fine....
    for(cottage &c : _cottages){
        if(!_tutorial_cutscene_timer){
            c.update();
        }

        // only run the collision check while unpaused
        if(!_burninate_pause_time && _player->handle_cottage_collision(c)){
            //the above if statement returns true if we hit a treasure hut
            result = scene_type::BONUS;
            //this marks the cottage as visited so that we can no longer return
            c.visit();
            _common_stuff.savefile.stats.cottages_entered++;

            set_visible(false);
        }
    }

    if(result && result != scene_type::BONUS){
        _hud.clear_scrolling_text();
        if(bn::music::playing()) bn::music::stop();
        if(_voices_volume) _common_stuff.savefile.options.voice_vol = *_voices_volume.get();
    }

    return result;
}

//assumptions: if just_died is true, you have died but the lives counter has not yet been decremented
void play_scene::autosave(bool just_died){
    //No autosaving during the tutorial.
    _common_stuff.savefile.session = _sesh.export_save();
    saved_session &saved_sesh = _common_stuff.savefile.session;
    
    if(_sesh.get_level() != 0){
        if(_sesh.get_mans() == 0){
            saved_sesh.exists = false;
        }else{
            if(_sesh.troghammer_enabled()){
                if(_troghammer){
                    saved_sesh.thinfo = _troghammer->get_status();
                }else{
                    // if you just died/finished burnin and there's no 
                    // troghammer yet, we need to send in the hammer, bro.
                    saved_sesh.thinfo = {troghammer_state::ALERT, 0, _void_tower->position()};
                }
            }

            for(int z = 0; z < _cottages.size(); ++z){
                saved_sesh.cottage_burnination_status[z] = _cottages.at(z).burninated();
                if(_cottages.at(z).has_treasure()){            
                    saved_sesh.can_visit_treasure_hut = true;    
                }
            }
            if(just_died){
                saved_sesh.mans--;
            }
            _common_stuff.set_autosave_text_visible(true);
            if(!just_died){
                _autosave_visibility_time = 1;
            }
        }
        _common_stuff.save();
    }
}

bool play_scene::level_complete(){
    bool result = _cottages.size() > 0;
    for(cottage &c : _cottages) {
        result = result & c.burninated();
    }

    
    return result;
}

bn::optional<scene_type> play_scene::update_pause_menu(){
    bn::optional<scene_type> result;

    if(!_gray_bg || !_pause_menu){
        setup_pause_menu();
    }

    if(bn::keypad::up_pressed()){
        _pause_menu_index -= 2;
        if(_pause_menu_index < 0) _pause_menu_index +=2;
    }else if(bn::keypad::down_pressed()){
        _pause_menu_index += 2;
        if(_pause_menu_index > 3) _pause_menu_index -=2;
    }else if(bn::keypad::left_pressed() && _pause_menu_index != 2){
        _pause_menu_index -= 1;
        if(_pause_menu_index < 0) _pause_menu_index +=1;
    }else if(bn::keypad::right_pressed() && _pause_menu_index != 1){
        _pause_menu_index += 1;
        if(_pause_menu_index > 3) _pause_menu_index -=1;
    }

    if(bn::keypad::any_pressed()) redraw_pause_menu_option();

    if(bn::keypad::b_pressed() ) unpause();

    if(bn::keypad::a_pressed()){
        //idk why but this bugs out if we dont do it this way
        switch(_pause_menu_index){
        case 0:
            //options
            result = scene_type::OPTIONS;
            set_visible(false);
        break;
        case 1:
            result = scene_type::HISCORES;
            set_visible(false);
        break;
        case 2:
            result = scene_type::MENU;
            unpause();
        break;
        case 3:
            unpause();
        break;
        default:
            BN_ERROR("invalid pause menu index");
        break;

        }
    }

    // Blinking animation
    // it ended up being a bit distracting so i've disabled it for now
    if(_timer == 1){
        _common_stuff.set_sprite_arr_visible(_paused_label, true);
    }else if(_timer == 60){
        _common_stuff.set_sprite_arr_visible(_paused_label, false);
    }if(_timer == 80){
        _timer = 0;
    }
    _timer++;

    return result;
}

void play_scene::set_visible(bool visible){
    _player->set_visible(visible);
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
    if(_troghammer && !_troghammer->inside_void_tower()){
        _troghammer->set_visible(visible);
    }
    if(_void_tower){
        _void_tower->set_visible(visible);
    }

    _countryside.set_visible(visible);

    if(_player_paused){

        _common_stuff.set_sprite_arr_visible(_paused_selected_option, visible);
        _common_stuff.set_sprite_arr_visible(_paused_label, visible);

        if(!visible){
            _pause_menu.reset();
            _gray_bg.reset();
        }
    }

}

troghammer_sound::troghammer_sound(const unsigned short &phrase, const bn::fixed &volume) : 
    _phrase(phrase), 
    _timer(0),
    _length(70),
    _vol(volume) {
    bn::sound_items::troghammer.play(volume);
}

void troghammer_sound::update(){
    ++_timer;
    if(_timer == _length){
        switch(_phrase){
            case 0:
                bn::sound_items::troghammer_alert.play(_vol);
            break;
            case 1:
                bn::sound_items::troghammer_awake.play(_vol);
            break;
            case 2:
                bn::sound_items::troghammer_coming.play(_vol);
            break;
            case 3:
                bn::sound_items::troghammer_arrived.play(_vol);
            break;
            default:
                BN_ERROR("Invalid phrase ID passed into troghammer_sound: ", _phrase);
            break;
        }
    }
}

void play_scene::spawn_troghammer(bool alert){
    _troghammer.reset(new troghammer(_void_tower->position(), true, _sesh.get_level(), _common_stuff.rand));  
    _void_tower->set_item(bn::sprite_items::voidtower, 1);

    if(alert){
        if(_troghammer->get_status().current_state == troghammer_state::ARRIVED){
            //If the troghammer spawns in immediately, 
            // use the "ARRIVES" notification instead of the "STIRS" one
            _hud.scroll_text("THE TROGHAMMER ARRIVES!");
            _th_sound.reset(new troghammer_sound(3, _common_stuff.savefile.options.sound_vol));          
        }else{
            _hud.scroll_text("THE TROGHAMMER STIRS...");   
            _th_sound.reset(new troghammer_sound(0, _common_stuff.savefile.options.sound_vol));          
        }
    }
}

void play_scene::update_tutorial(){
    BN_ASSERT(_sesh.get_level() == 0, "Tutorial can only play on level 0");
    
    if(_cottages.size() == 0){
        //tutorial phase 1
        if(_timer == 0 && (bn::keypad::up_pressed() || bn::keypad::down_pressed() ||
            bn::keypad::left_pressed() || bn::keypad::right_pressed())){
            _timer = 1;
        }
        if(_timer) _timer++;

        if(_timer == 240){
            _cottages.emplace_back(65, 5, direction::LEFT, false, false, _common_stuff);
            _cottages.emplace_back(-65, -45, direction::DOWN, false, false, _common_stuff);
            _text_box.reset(new text_box(_common_stuff.small_generator, "Terrorize the populace by squishing PEASANTS as they leave their homes! To STOMP a peasant, move Trogdor into it."));
            _timer = 0;
            _tutorial_cutscene_timer = 1;

            _cottages.at(0).drop();
            _cottages.at(1).drop();
            bn::sound_items::heavylanding.play(_common_stuff.savefile.options.sound_vol);
        }

    }else if(_knights.size() == 0){
        //tutorial phase 2
        if(_peasants.size() > 0 && _player->get_trogmeter() == 0){
            bn::fixed arrow_ycor = _peasants.front().get_y();
            bn::fixed arrow_xcor = _peasants.front().get_x() - 18;
            if(!_tutorial_arrow){
                _tutorial_arrow.reset(new tutorial_arrow(arrow_xcor, arrow_ycor, direction::RIGHT));
            }else{
                _tutorial_arrow->set_y(arrow_ycor);
                _tutorial_arrow->set_x(arrow_xcor);
            }
        }

        if(_player->get_trogmeter() >= 1 && 
        //this direction clause is just to make sure this only runs once
                _tutorial_arrow->get_direction() != direction::UP){
            
            _text_box.reset(new text_box(_common_stuff.small_generator, "Stomping peasants fills your TROG-METER. Try and fill the Trog-Meter to its limit!"));
            _tutorial_arrow.reset(new tutorial_arrow(-60, -62, direction::UP));
        }

        if(_player->get_trogmeter() >= 5){
            _knights.emplace_front(-85, -15, true, _common_stuff.rand);
            _knights.front().move_from(200, -145, -15);
            _knights.emplace_front(85,-15,false, _common_stuff.rand);
            _knights.front().move_from(200, 145, -15);
            _archers.emplace_front(-50, true, _common_stuff);
            _archers.front().move_from(60, 135, -50);

            _text_box.reset(new text_box(_common_stuff.small_generator, "Archers and knights can, like, hurt you real bad! \nAvoid their arrows and swords."));

            _tutorial_cutscene_timer = 1;

        }
    }else if(_player->burninating()){
        _tutorial_arrow.reset();
        //burnination / level winning tutorial
        if(_player->get_burninating_time() == _player->get_burninating_length()){
            _text_box.reset();
            _text_box.reset(new text_box(_common_stuff.small_generator, "Filling the Trog-Meter grants you BURNINATION. In this state, you gain fire-breathing and invicibility."));
        }
        if(_player->get_burninating_time() == 1) _text_box.reset();
    }else if(!_text_box){
        _text_box.reset(new text_box(_common_stuff.small_generator, "Fill the Trog-Meter and burninate all cottages to win the level."));
    }

    if(_tutorial_arrow){
        _tutorial_arrow->update();
        if(_tutorial_arrow->get_direction() == direction::UP){
            if(_player->get_trogmeter() == 0){
                _tutorial_arrow->set_x(-64);
            }else{
                _tutorial_arrow->set_x(-73 + 9 * _player->get_trogmeter());
            }
        }
    }

}

void play_scene::setup_pause_menu(){
    _pause_menu = bn::regular_bg_items::pause_menu.create_bg_optional(0, 0);
    _gray_bg = bn::regular_bg_items::gray.create_bg_optional(0, 0);
    _gray_bg->set_blending_enabled(true);
    _gray_bg->set_priority(0);
    _pause_menu->set_z_order(1);
    _pause_menu->set_priority(0);
    _pause_menu->set_z_order(0);
    redraw_pause_menu_option();
    _paused_label.clear();
    _common_stuff.small_generator.set_center_alignment();
    _common_stuff.small_generator.set_palette_item(WHITE_PALETTE);    
    _common_stuff.small_generator.generate(0,65, "paused.", _paused_label);
    _timer = 0;
}

void play_scene::unpause(){
    _player_paused = false;

    _pause_menu.reset();
    _gray_bg.reset();

    _paused_selected_option.clear();
    _paused_label.clear();

}

void play_scene::redraw_pause_menu_option(){
    _paused_selected_option.clear();
    _common_stuff.text_generator.set_center_alignment();

    _common_stuff.text_generator.set_palette_item(RED_PALETTE);
    _common_stuff.text_generator.set_bg_priority(0);
    bn::fixed_point pos;
    bn::string<16> text;

    switch(_pause_menu_index){
        case 0:
            _common_stuff.text_generator.generate(-49,-31, "OPTIONS", _paused_selected_option);
        break;
        case 1:
            _common_stuff.text_generator.generate(50,-37, "VIEW", _paused_selected_option);
            _common_stuff.text_generator.generate(50,-25, "HI-SCORES", _paused_selected_option);
        break;
        case 2:
            _common_stuff.text_generator.generate(-49,22, "QUIT", _paused_selected_option);
        break;
        case 3:
            _common_stuff.text_generator.generate(51,22, "RESUME", _paused_selected_option);
        break;
        default:
            BN_ERROR("invalid pause menu index");
        break;
    }

    
    _common_stuff.text_generator.generate(pos, text, _paused_selected_option);

}

void play_scene::respawn(const bool &iframes, const uint8_t &init_trogmeter){
    switch(_sesh.get_dragon()){
        //gotta be a better way to do this 
        case dragon::TROGDOR:
            _player.reset(new trogdor(TROG_PLAYER_SPAWN_X, TROG_PLAYER_SPAWN_Y + 
            (_sesh.get_level() == 27 || _sesh.get_level() == 59 || _sesh.get_level() == 91) ? 10 : 0, 
            _sesh, iframes, _common_stuff, init_trogmeter));
        break;
        case dragon::SUCKS:
            _player.reset(new sucks(TROG_PLAYER_SPAWN_X, TROG_PLAYER_SPAWN_Y + 
            (_sesh.get_level() == 27 || _sesh.get_level() == 59 || _sesh.get_level() == 91) ? 10 : 0, 
            _sesh, iframes, _common_stuff, init_trogmeter));
        break;
        case dragon::CHIAROSCURO:
            _player.reset(new chiaroscuro(TROG_PLAYER_SPAWN_X, TROG_PLAYER_SPAWN_Y + 
            (_sesh.get_level() == 27 || _sesh.get_level() == 59 || _sesh.get_level() == 91) ? 10 : 0, 
            _sesh, iframes, _common_stuff, init_trogmeter));
        break;
        default:
            BN_ERROR("Invalid dragon type found in session info");
        break;
    }

}

bn::vector<freezable *, 23> play_scene::all_freezables(){
    bn::vector<freezable *, 23> result;

    if(_troghammer){
        result.emplace_back(_troghammer.get());
    }
    for(knight &k : _knights){
        result.emplace_back(&k);
    }
    for(peasant &p : _peasants){
        result.emplace_back(&p);
    }
    return result;
}

bn::vector<entity *, 33> play_scene::all_entities(){
    bn::vector<entity *, 33> result;
    for(entity *e : all_freezables()){
        result.emplace_back(e);
    }
    for(archer &a : _archers){
        result.emplace_back(&a);
    }
    for(cottage &c : _cottages){
        result.emplace_back(&c);
    }
    
    return result;
}

void play_scene::shake_screen(bn::fixed yoffset, uint8_t interval, uint8_t num_shakes){
    _shake_timer = interval * num_shakes;
    _shake_interval = interval;
    _shake_y_offset = yoffset;
}

}