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
        _sesh(sesh){
    _sprite.set_z_order(FRONT_ZORDER);

    _trogmeter = 0;
    burninate_time = 0;
}

bool player::burninating(){
    return burninate_time != 0;
}

void player::update(){
    entity::update();
    move();
    check_boundary_collision();


    _sprite.set_position(_pos);


    if(burninate_time > 0) { 
        burninate_time--;
    }else if(breath.enabled()){
        // if the breath is enabled AND our burninate timer has expired,
        // we need to end burnination
        _trogmeter = 0;
        breath.disable();
    }
    //update fire breath;
    short xoffset = TROG_FIREBREATH_XOFFSET;
    if(_sprite.horizontal_flip()){
        xoffset=-xoffset;
    }
    breath.set_x(_pos.x() + xoffset);
    breath.set_y(_pos.y() + TROG_FIREBREATH_YOFFSET);
    breath.update();

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
        breath.set_horizontal_flip(true);
        _pos.set_x(_pos.x() - _speed);
        moving=true;
    }
    if(bn::keypad::right_held()){
        _sprite.set_horizontal_flip(false);
        breath.set_horizontal_flip(false);
        _pos.set_x(_pos.x() + _speed);
        moving=true;
    }
    if(moving){
        _walkcycle.update();
    }

}

void player::check_boundary_collision(){
    bn::fixed top_bound = -70;
    bn::fixed bottom_bound = 81;
    bn::fixed left_bound = -120;
    bn::fixed right_bound = 120;
    //collision
    if(_hitbox.top() < top_bound){
        _pos.set_y(top_bound + _hitbox.height() / 2);
    }
    if(_hitbox.bottom() > bottom_bound){
        _pos.set_y(bottom_bound - _hitbox.height() / 2);
    }
    if(_hitbox.left() < left_bound){
        _pos.set_x(left_bound + _hitbox.width() / 2);
    }
    if(_hitbox.right() > right_bound){
        _pos.set_x(right_bound - _hitbox.width() / 2);
    }
    
    _hitbox.set_position(_pos);
    
}

void player::check_cottage_collision(cottage &cottage){
    bn::fixed_rect cottagebox = cottage.get_hitbox();
    if(_hitbox.intersects(cottagebox)){
        // BN_LOG("collision lol make him stop");
    }
    if(burninating()){
        breath.check_cottage_collision(cottage);
    }
}

void player::check_peasant_collision(peasant &peasant){
    bn::fixed_rect pbox = peasant.get_hitbox();
    if(_hitbox.intersects(pbox) && !peasant.dead()){
        BN_LOG("stomped.");
        peasant.stomp(_sesh);
        ++_trogmeter;
        if(_trogmeter == trogmeter_max){
            burninate_time = burninate_length;
            breath.enable();

            BN_LOG("aaron burrninate. got milk");
            bn::sound_items::burninate.play(1);
        }
    }

    if(burninating()){
        breath.check_peasant_collision(peasant);
    }
}


}
