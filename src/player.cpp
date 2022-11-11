#include <bn_keypad.h>
#include <bn_math.h>
#include <bn_log.h>
#include <bn_sound_items.h>
#include "player.h"
#include "entity.h"
namespace trog {

player::player(session_info &sesh) : 
        entity(bn::fixed(0), bn::fixed(0), bn::fixed(20), bn::fixed(45), bn::sprite_items::player.create_sprite(0, 0)),
        _speed(bn::fixed(1.5)),
        _walkcycle(bn::create_sprite_animate_action_forever(
                    _sprite, 5, bn::sprite_items::player.tiles_item(), 0, 1, 2, 3)),
        _breath(sesh),
        _sesh(sesh){
    _sprite.set_z_order(FRONT_ZORDER);

    _trogmeter = 0;
    _burninate_time = 0;
}

bool player::burninating(){
    return _burninate_time != 0;
}

void player::update(){
    entity::update();
    move();
    check_boundary_collision();


    _sprite.set_position(_pos);


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

void player::check_cottage_collision(cottage &cottage){
    bn::fixed_rect cottagebox = cottage.get_hitbox();
    if(_hitbox.intersects(cottagebox)){
        // BN_LOG("collision lol make him stop");
    }
    if(burninating()){
        _breath.check_cottage_collision(cottage);
    }
}

void player::check_peasant_collision(peasant &peasant){
    bn::fixed_rect pbox = peasant.get_hitbox();
    if(_hitbox.intersects(pbox) && !peasant.dead()){
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
        _breath.check_peasant_collision(peasant);
    }
}


}
