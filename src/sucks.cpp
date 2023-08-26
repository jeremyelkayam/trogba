#include "sucks.h"
#include "bn_sprite_items_sucks.h"
#include "bn_sprite_items_sucks_arms.h"
#include "bn_sprite_items_expanding_circle.h"
#include <bn_keypad.h>
#include <bn_log.h>
#include <bn_colors.h>

#define NORM_WLKCL bn::create_sprite_animate_action_forever(_sprite, 5, bn::sprite_items::sucks.tiles_item(), 1, 2, 3, 2, 1, 0)
#define FLMTHW_WLKCL bn::create_sprite_animate_action_forever(_sprite, 5, bn::sprite_items::sucks.tiles_item(), 10, 11, 12, 11, 10, 9)

namespace trog { 

sucks::sucks(bn::fixed xcor, bn::fixed ycor, session_info &sesh, bool iframes, common_stuff &common_stuff, uint8_t initial_trogmeter) : 
    player(xcor, ycor, TROG_SUCKS_WIDTH, TROG_SUCKS_HEIGHT, TROG_SUCKS_SPEED, 
        bn::fixed_point(42, 1),
        sesh, iframes, bn::sprite_items::sucks, 4, common_stuff, initial_trogmeter), 
    _walkcycle(NORM_WLKCL),
    _stomp_timer(0),
    _shockwave(bn::sprite_items::expanding_circle.create_sprite(0,-1)),
    _shockwave_anim(bn::create_sprite_animate_action_once(_shockwave, 1, bn::sprite_items::expanding_circle.tiles_item(),
    0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28,  30)) {
    _shockwave.set_visible(false);
    _shockwave.set_scale(2);
}

void sucks::update(){
    player::update();

    if(!dead() && any_dpad_input() && can_move()){
        _walkcycle.update();
    }

    if(burninating()){
        bn::fixed yoffset = abs(_walkcycle.current_index() - (_walkcycle.graphics_indexes().size()) / 2) - 2;
        _breath_offset.set_y(yoffset);
    }
    
    if(_stomp_timer){
        ++_stomp_timer;
        if(dead()) _stomp_timer = 0; // this prevents us from stomping after we die


        bn::sprite_palette_ptr my_palette = _sprite.palette();
        if(_stomp_timer == TROG_SUCK_STOMP_FRAME){

            my_palette.set_fade(bn::colors::red, 0.6);
            _fade_action.emplace(my_palette, 20, 0.3);

            _sprite.set_tiles(bn::sprite_items::sucks.tiles_item(), 8);
            _shockwave.set_position(foot_pos());
            _shockwave.set_visible(true);
        }
        if(_stomp_timer >= TROG_SUCK_STOMP_FRAME && !_shockwave_anim.done()){
            _shockwave_anim.update();
        }
        if(_shockwave_anim.done()){
            _shockwave.set_visible(false);
        }

        //this is when you regain control of the suck dragon

        
        //This ensures you can't stomp again until after the peasants unfreeze.
        if(_stomp_timer >= TROG_SUCK_STOMP_RECHARGE_TIME){
            _fade_action.reset();
            _stomp_timer = 0;
            my_palette.set_fade_intensity(0);
        }
    }

    if(bn::keypad::a_pressed() && can_stomp()){
        stomp();
    }

    if(_fade_action ){
        _fade_action->update();
    }

}

bn::fixed_point sucks::foot_pos(){
    bn::fixed xoffset = 13 * (_sprite.horizontal_flip() ? -1 : 1);
    return bn::fixed_point(get_x() + xoffset, get_y() + 20);
}

void sucks::update_anim(){
    entity::update_anim();
    if(_stomp_timer){
        _stomp_timer++;
        if(_stomp_timer == 20){            
            _sprite.set_tiles(bn::sprite_items::sucks.tiles_item(), 8);
        }else if(_stomp_timer == 50){
            _stomp_timer = 0;
        }else if(_stomp_timer >= 20){
            if(_stomp_timer % 6 == 0){
                _sprite.set_y(_sprite.y() - 1);
            }else if(_stomp_timer % 6 == 3){
                _sprite.set_y(_sprite.y() + 1);
            }
        }
    }else{
        if((_move_action && !_move_action->done()) || _move_by_action || _update_anim_when_not_moving){
            _walkcycle.update();
        }
    }

    update_firebreath();
}

void sucks::stomp(){
    _stomp_timer = 1;
    _sprite.set_tiles(bn::sprite_items::sucks.tiles_item(), 7);
    _shockwave_anim.reset();
}

void sucks::start_burninating(){
    player::start_burninating();
    change_walkcycle(FLMTHW_WLKCL);
}

void sucks::stop_burninating(){
    player::stop_burninating();
    change_walkcycle(NORM_WLKCL);
}

void sucks::change_walkcycle(const bn::isprite_animate_action &walkcycle){
    int dex = _walkcycle.current_index();
    _walkcycle = walkcycle;
    do{
        //we need to update the walkcycle at least once 
        //otherwise if you don't move, he will stay on a non-burninating frame
        _walkcycle.update();
    }while(_walkcycle.current_index() != dex);
}

void sucks::update_win_anim(){
    player::update_win_anim();
    change_walkcycle(NORM_WLKCL);
}

}