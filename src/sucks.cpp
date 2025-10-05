#include "sucks.h"
#include "dragon_data.h"
#include "bn_sprite_items_sucks.h"
#include "bn_sprite_items_sucksweat.h"
#include "bn_sprite_items_shockwave.h"
#include <bn_keypad.h>
#include <bn_colors.h>
#include <bn_log.h>
#include "common_functions.h"

#define NORM_WLKCL bn::create_sprite_animate_action_forever(_sprite, 5, bn::sprite_items::sucks.tiles_item(), 1, 2, 3, 2, 1, 0)
#define FLMTHW_WLKCL bn::create_sprite_animate_action_forever(_sprite, 5, bn::sprite_items::sucks.tiles_item(), 10, 11, 12, 11, 10, 9)

namespace trog { 

sucks::sucks(bn::fixed xcor, bn::fixed ycor, session_info &sesh, bool iframes, common_stuff &common_stuff, unsigned int  initial_trogmeter) : 
    player(xcor, ycor, SDATA.width, SDATA.height, SDATA.speed, 
        bn::fixed_point(30, 1),
        sesh, iframes, bn::sprite_items::sucks, 4, common_stuff, initial_trogmeter), 
    _walkcycle(NORM_WLKCL),
    _stomp_timer(0),
    _oscillate_time(1) {

    set_y_offset(-2);

}

void sucks::init_sweat(bn::fixed yoffset)
{
    _sweat = bn::sprite_items::sucksweat.create_sprite(0,0);
    _sweat_anim = bn::create_sprite_animate_action_forever(*_sweat, 4, bn::sprite_items::sucksweat.tiles_item(), 0, 0, 0, 1, 2, 3, 0, 0);
    update_sweat_pos(yoffset);
}

void sucks::update(){
    player::update();

    if(!dead() && any_dpad_input() && can_move()){
        _walkcycle.update();
    }

    _breath_offset.set_y(attachments_y_offset());
    
    if(_stomp_timer){
        ++_stomp_timer;
        if(dead()) _stomp_timer = 0; // this prevents us from stomping after we die


        if(_stomp_timer == TROG_SUCK_STOMP_FRAME){
            _sprite.set_tiles(bn::sprite_items::sucks.tiles_item(), 8);
            
            _shockwave = shockwave(foot_pos());
            bn::sound_items::heavy_crash.play(_common_stuff.savefile.options.sound_vol);


            get_palette().set_fade(bn::colors::red, 0);
            _fade_action.emplace(get_palette(), 30, _hi);
        }
        if(_stomp_timer >= TROG_SUCK_STOMP_FRAME && _shockwave && 
            !_shockwave->done()){
            
                _shockwave->update();
        }
        if(_shockwave && _shockwave->done()){
            _shockwave.reset();

            get_palette().set_fade(bn::colors::red, _hi);
            _oscillate_time = 10;
            _fade_action.emplace(get_palette(), _oscillate_time, _lo);
            if(!_iframes) init_sweat(-12);
            
        }

        // if we are here, we have control of the suck dragon again


        if(_iframes == 1){
            init_sweat(attachments_y_offset() - 13);
        }


        if(_fade_action && _fade_action->done()){
            bn::fixed target = _fade_action->final_intensity() == _hi ? _lo : _hi;
            _oscillate_time = (bn::fixed(_oscillate_time) * bn::fixed(1.1)).round_integer();
            _fade_action.emplace(get_palette(), _oscillate_time, target);
        }


        
        //This ensures you can't stomp again until you're recharged.
        if(_stomp_timer >= TROG_SUCK_STOMP_RECHARGE_TIME){
            _stomp_timer = 0;
            reset_fade();
        }
    }

    if(bn::keypad::a_pressed() && can_stomp()){
        stomp();
    }

    if(_fade_action){
        if(_fade_action->done()){
            _fade_action.reset();
        }else{
            BN_LOG("updating fade action. fade intensity ", get_palette().fade_intensity());
            _fade_action->update();
            BN_LOG("fade action updated");
        }
    }
    if(_sweat){
        if(any_dpad_input()){
            update_sweat_pos(attachments_y_offset() - 15);
        }
        _sweat_anim->update();
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

    
    _breath_offset.set_y(attachments_y_offset());
    update_firebreath();
}

void sucks::stomp(){
    _stomp_timer = 1;
    _sprite.set_tiles(bn::sprite_items::sucks.tiles_item(), 7);
}

void sucks::start_burninating(){
    player::start_burninating();
    change_walkcycle(FLMTHW_WLKCL);
    _stomp_timer = 0;
    reset_fade();
}

void sucks::stop_burninating(){
    player::stop_burninating();
    change_walkcycle(NORM_WLKCL);
}

void sucks::enable_breath()
{
    player::enable_breath();
    change_walkcycle(FLMTHW_WLKCL);
}

void sucks::disable_breath()
{
    player::disable_breath();
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
    reset_fade();
}

void sucks::die(const unsigned int &death_index){
    player::die(death_index);
    reset_fade();
}

void sucks::set_visible(const bool &visible){
    player::set_visible(visible);
    if(_sweat) _sweat->set_visible(visible);
    if(_shockwave) _shockwave->set_visible(visible);
    //This makes things look a little weird when you're
    // returning from a submenu or a pause screen, but w/e
    reset_fade();
}

void sucks::reset_fade(){
    _fade_action.emplace(get_palette(), 20, 0);
    _sweat.reset();
    _sweat_anim.reset();
}


void sucks::update_sweat_pos(bn::fixed yoffset){
    _sweat->set_x(_pos.x() + (_sprite.horizontal_flip() ? -1 : 1) * 6);
    _sweat->set_y(_pos.y() + yoffset);
    _sweat->set_horizontal_flip(_sprite.horizontal_flip());    
}

shockwave::shockwave(const bn::fixed_point &pos) {
    for(bn::fixed angle = 0; angle < 360;  angle += 90){
        bn::sprite_ptr sprite = bn::sprite_items::shockwave.create_sprite(0,0);
        sprite.set_rotation_angle(angle);
        _sprites.emplace_back(sprite);
    }
    set_position(pos);
    
    for(bn::sprite_ptr &sprite : _sprites) {
        _anims.emplace_back(bn::create_sprite_animate_action_once(sprite, 0, bn::sprite_items::shockwave.tiles_item(),
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16));
    }
}

void shockwave::update(){

    for(auto &anim : _anims) {
        if(anim.current_index() == 4){
            anim.set_wait_updates(1);
        }
        else if(anim.current_index() == 14){
            anim.set_wait_updates(2);
        }
        anim.update();
    }
}

void shockwave::set_visible(const bool &visible) {
    for(auto &sprite : _sprites) { 
        sprite.set_visible(visible);
    }
}
void shockwave::reset() {
    for(auto &anim : _anims) {
        anim.reset();
        anim.set_wait_updates(0);
    }
}

void shockwave::set_position(const bn::fixed_point &pos){
    // angle 0: 1, -1
    // angle 90: 1, 1
    // 180: -1, -1
    // 270: -1, 1
    _sprites.at(0).set_position(pos.x() + 32, pos.y() - 32);
    _sprites.at(1).set_position(pos.x() - 32, pos.y() - 32);
    _sprites.at(2).set_position(pos.x() - 32, pos.y() + 32);
    _sprites.at(3).set_position(pos.x() + 32, pos.y() + 32);
}

}