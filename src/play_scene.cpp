#include <bn_keypad.h>
#include <bn_sound_items.h>
#include <bn_log.h>

#include "constants.h"
#include "bn_regular_bg_items_day.h"
#include "bn_sprite_items_trogdor_variable_8x16_font_black.h"
#include "bn_sprite_items_trogdor_variable_8x16_font.h"
// #include "bn_affine_bg_items_burninatefire.h"
#include "play_scene.h"

namespace trog {

play_scene::play_scene(session_info& sesh, hud& hud, bn::sprite_text_generator &big_text_generator) : 
        _sesh(sesh),
        _trogdor(new player(TROG_PLAYER_SPAWN_X, TROG_PLAYER_SPAWN_Y, sesh, false)),
        _hud(hud),
        _pfact(_cottages,_peasants),
        _afact(_archers, sesh.get_level()),
        _big_text_generator(big_text_generator),
        _countryside(bn::regular_bg_items::day.create_bg(0, 58))
{
    _cottages.emplace_back(bn::fixed(-30), bn::fixed(-40), direction::DOWN, true);
    _cottages.emplace_back(bn::fixed(60), bn::fixed(-20), direction::LEFT, false);
    _cottages.emplace_back(bn::fixed(0), bn::fixed(60), direction::UP, false);


    _knights.emplace_front(-59, 31, false);
    _knights.emplace_front(43,-40,true);

    _big_text_generator.set_center_alignment();
    _big_text_generator.set_palette_item(bn::sprite_items::trogdor_variable_8x16_font_black.palette_item());
    _big_text_generator.generate(0, 0, "BURNINATE!", _burninate_text_sprites);

    _hud.set_sprite_arr_visible(_burninate_text_sprites, false);

}

bn::optional<scene_type> play_scene::update(){
    set_visible(true);

    bn::optional<scene_type> result;
    
    if(_burninate_pause_time > 0) {
        _burninate_pause_time++;
    }else{
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
            _hud.set_sprite_arr_visible(_burninate_text_sprites, true);
        }

        for(archer &a : _archers) {
            a.update();
            _trogdor->handle_arrow_collision(a);
        }


        for(knight &k : _knights){
            k.update();
            _trogdor->handle_knight_collision(k);
        }

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
    }
    if(_burninate_pause_time >= TROG_BURNINATE_PAUSETIME){
        _burninate_pause_time = 0;
        _hud.set_sprite_arr_visible(_burninate_text_sprites, false);
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