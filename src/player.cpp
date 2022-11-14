#include <bn_keypad.h>
#include <bn_math.h>
#include <bn_log.h>
#include <bn_sound_items.h>
#include "player.h"
#include "entity.h"
namespace trog {

player::player(session_info &sesh) : 
        entity(bn::fixed(TROG_SPAWN_X), bn::fixed(TROG_SPAWN_Y), bn::fixed(20), bn::fixed(45), bn::sprite_items::player.create_sprite(0, 0)),
        _speed(bn::fixed(1.5)),
        _walkcycle(bn::create_sprite_animate_action_forever(
                    _sprite, 5, bn::sprite_items::player.tiles_item(), 0, 1, 2, 3)),
        _breath(sesh),
        _sesh(sesh){
    _sprite.set_z_order(FRONT_ZORDER);

    _trogmeter = 0;
    _burninate_time = 0;
    _time_dead = 0;
    _iframes = 0;
}

bool player::burninating(){
    return _burninate_time != 0;
}

void player::update(){
    if(dead()){
        ++_time_dead;
        if(_time_dead == TROG_RESPAWN_TIME){
            respawn();
        }
    }
    // this cannot be an else if, because if the respawn method is called 
    // then trogdor will be alive again.
    if(!dead()){
        entity::update();
        move();
        check_boundary_collision();

        if(_burninate_time > 0) { 
            _burninate_time--;
        }else if(_breath.enabled()){
            // if the breath is enabled AND our burninate timer has expired,
            // we need to end burnination
            _trogmeter = 0;
            _breath.disable();
        }
        //update fire breath;
        short xoffset = TROG_FIREBREATH_XOFFSET;
        if(_sprite.horizontal_flip()){
            xoffset=-xoffset;
        }
        _breath.set_x(_pos.x() + xoffset);
        _breath.set_y(_pos.y() + TROG_FIREBREATH_YOFFSET);
        _breath.update();

        //update trogdor to show iframes
        if(invincible()){
            //if we're on an odd iframe, hide the sprite.
            //this creates a flicker effect
            bool odd = _iframes % 2;
            if(odd){
                _sprite.set_visible(false);
            }else{
                _sprite.set_visible(true);
            }
            ++_iframes;
        }

        //once the iframes are over, return the sprite to normal
        if(_iframes > TROG_RESPAWN_INV_TIME) {
            BN_LOG("end invincibility");
            _sprite.set_visible(true);
            _iframes = 0;
        }

    }

}

void player::move(){

    bool moving = false;
    
    if(bn::keypad::up_held()){
        _pos.set_y(_pos.y() - _speed);
        moving=true;
    }
    if(bn::keypad::down_held()){
        _pos.set_y(_pos.y() + _speed);
        moving=true;
    }
    if(bn::keypad::left_held()){
        _sprite.set_horizontal_flip(true);
        _breath.set_horizontal_flip(true);
        _pos.set_x(_pos.x() - _speed);
        moving=true;
    }
    if(bn::keypad::right_held()){
        _sprite.set_horizontal_flip(false);
        _breath.set_horizontal_flip(false);
        _pos.set_x(_pos.x() + _speed);
        moving=true;
    }
    if(moving){
        _walkcycle.update();
    }

}

void player::check_boundary_collision(){
    //collision
    if(_hitbox.top() < TROG_COUNTRYSIDE_TOP_BOUND){
        _pos.set_y(TROG_COUNTRYSIDE_TOP_BOUND + _hitbox.height() / 2);
    }
    if(_hitbox.bottom() > TROG_COUNTRYSIDE_BOTTOM_BOUND){
        _pos.set_y(TROG_COUNTRYSIDE_BOTTOM_BOUND - _hitbox.height() / 2);
    }
    if(_hitbox.left() < TROG_COUNTRYSIDE_LEFT_BOUND){
        _pos.set_x(TROG_COUNTRYSIDE_LEFT_BOUND + _hitbox.width() / 2);
    }
    if(_hitbox.right() > TROG_COUNTRYSIDE_RIGHT_BOUND){
        _pos.set_x(TROG_COUNTRYSIDE_RIGHT_BOUND - _hitbox.width() / 2);
    }
    
    _hitbox.set_position(_pos);
    
}

void player::handle_cottage_collision(cottage &cottage){
    bn::fixed_rect cottagebox = cottage.get_hitbox();
    if(_hitbox.intersects(cottagebox)){
        // BN_LOG("collision lol make him stop");
    }
    if(burninating()){
        _breath.handle_cottage_collision(cottage);
    }
}

void player::handle_peasant_collision(peasant &peasant){
    if(collides_with(peasant) && !peasant.dead()){
        BN_LOG("stomped.");
        peasant.stomp();
        _sesh.score+=TROG_PEASANT_STOMP_SCORE;
        
        ++_trogmeter;
        if(_trogmeter == _trogmeter_max){
            _burninate_time = _burninate_length;
            _breath.enable();

            BN_LOG("aaron burrninate. got milk");
            bn::sound_items::burninate.play(TROG_DEFAULT_VOLUME);
        }
    }

    if(burninating()){
        _breath.handle_peasant_collision(peasant);
    }
}

void player::handle_knight_collision(knight &knight){
    if(collides_with(knight) && !dead() && !invincible()) { 
        bn::sound_items::death.play(TROG_DEFAULT_VOLUME);
        _time_dead = 1;
    }
}

void player::respawn(){
    _time_dead=0;
    --_sesh.mans;
    _pos.set_x(TROG_SPAWN_X);
    _pos.set_y(TROG_SPAWN_Y);
    _burninate_time = 0;
    _trogmeter = 0;
    _iframes = 1;
}

}
